<?php

require_once dirname(__FILE__) . "/InfoUtils.php";

class DB
{
    private $index_fp = NULL;
    private $data_fp = NULL;
    private $db_bucktes_capbility;

    public function __construct()
    {
        $this->db_bucktes_capbility = DB_BUCKETS_SIZE;;
    }

    public function open($pathname)
    {
        $index_path = $pathname . ".idx";
        $data_path = $pathname . ".dat";
        $need_init = FALSE; 
        
        // open index file
        if (file_exists($index_path))
        {
            $mode = "r+b";  // read and write
        }
        else
        {
            $mode = "w+b";  // read and write, create if not exists, truncate
            $need_init = TRUE; 
        }

        $this->index_fp = fopen($index_path, $mode);
        if ($this->index_fp === FALSE)
        {
            echo "Open $index_path failed!" . PHP_EOL;
            return DB_ERR_OPEN_FILE_FAILED;
        }
        if ($need_init)
        {
            $d = pack("L", 0x00000000);
            for ($i = 0; $i < $this->db_bucktes_capbility; $i++)
            {
                $ret = fwrite($this->index_fp, $d, 4);
                if($ret === FALSE)
                {
                    echo "Write $index_path failed!" . PHP_EOL;
                    return DB_ERR_WRITE_FILE_FAILED;
                }
            }
        }

        // open data file
        if (file_exists($data_path))
        {
            $mode = "r+b";  // read and write
        }
        else
        {
            $mode = "w+b";  // read and write, create if not exists
        }
        $this->data_fp = fopen($data_path, $mode);
        if ($this->data_fp === FALSE)
        {
            echo "Open $data_path failed!" . PHP_EOL;
            return DB_ERR_OPEN_FILE_FAILED;
        }

        return DB_SUCCESS;
    }

    public function insert($key, $value)
    {
        $key_len = strlen($key);
        if ($key_len > DB_KEY_SIZE)
        {
            echo "The len of key is large then " . DB_KEY_SIZE ."; key:$key" . PHP_EOL;
            return DB_ERR_KEY_EXCEED;
        }
        $value_len = strlen($value);

        // Get the index new pos
        $index_stat = fstat($this->index_fp);
        $index_new_pos = $index_stat["size"];

        // Get the data new pos
        $data_stat = fstat($this->data_fp);
        $data_new_pos = $data_stat["size"];

        // Create a new index block
        $index_block = pack("L", 0x00000000);
        $index_block .= $key;
        $left_len = DB_KEY_SIZE - $key_len;
        for ($i = 0; $i < $left_len; $i++)
        {
            $index_block .= pack("C", 0x00);
        }
        $index_block .= pack("L", $data_new_pos);
        $index_block .= pack("L", $value_len);

        $index_offset = ($this->hash_func($key) % $this->db_bucktes_capbility) * 4;
        fseek($this->index_fp, $index_offset, SEEK_SET);
        $index_pos = fread($this->index_fp, 4);
        $index_pos = unpack("L", $index_pos);
        $index_pos = $index_pos[1];

        if ($index_pos === 0)
        {
            fseek($this->index_fp, $index_offset, SEEK_SET);
            fwrite($this->index_fp, pack("L", $index_new_pos), 4);

            fseek($this->index_fp, 0, SEEK_END);
            fwrite($this->index_fp, $index_block, DB_INDEX_SIZE);

            fseek($this->data_fp, 0, SEEK_END);
            fwrite($this->data_fp, $value, $value_len); 

            return DB_SUCCESS;
        }

        $key_exist = FALSE;
        while ($index_pos)
        {
            fseek($this->index_fp, $index_pos, SEEK_SET);
            $index_content = fread($this->index_fp, DB_INDEX_SIZE);    
            $in_key = substr($index_content, 4, DB_KEY_SIZE);
            if (0 === strncmp($in_key, $key, $key_len))
            {
                $key_exist = TRUE;
                break;
            }

            $index_prev = $index_pos;
            $index_pos = substr($index_content, 0, 4);
            $index_pos = unpack("L", $index_pos);
            $index_pos = $index_pos[1];
        }

        if ($key_exist)
        {
            return DB_KEY_EXIST;
        }

        fseek($this->index_fp, $index_prev, SEEK_SET);
        fwrite($this->index_fp, pack("L", $index_new_pos), 4);
        
        fseek($this->index_fp, 0, SEEK_END);
        fwrite($this->index_fp, $index_block, DB_INDEX_SIZE);

        fseek($this->data_fp, 0, SEEK_END);
        fwrite($this->data_fp, $value, $value_len); 

        return DB_SUCCESS;
    }

    public function remove($key)
    {
        $index_offset = ($this->hash_func($key) % $this->db_bucktes_capbility) * 4;
        $ret = fseek($this->index_fp, $index_offset, SEEK_SET);
        if (-1 === $ret)
        {
            echo "fseek $index_offset failed!" . PHP_EOL;
            return DB_ERR_FSEEK_FAILED;
        }
        
        $index_pos = fread($this->index_fp, 4);
        $index_pos = unpack("L", $index_pos);
        $index_pos = $index_pos[1];

        $key_exist = FALSE;
        $index_prev = FALSE;
        while ($index_pos)
        {
            fseek($this->index_fp, $index_pos, SEEK_SET);
            $index_content = fread($this->index_fp, DB_INDEX_SIZE);

            $index_next = substr($index_content, 0, 4);
            $index_next = unpack("L", $index_next);
            $index_next = $index_next[1];

            $in_key = substr($index_content, 4, DB_KEY_SIZE);
            if (0 === strncmp($key, $in_key, strlen($key)))
            {
                $key_exist = TRUE;
                break;
            } 

            $index_prev = $index_pos;
            $index_pos = $index_next;
        }

        if (!$key_exist)
        {
            return DB_KEY_NOT_EXIST;
        }

        if ($index_prev === FALSE)
        {
            fseek($this->index_fp, $index_offset, SEEK_SET);
            fwrite($this->index_fp, pack("L", $index_next), 4);
        }
        else
        {
            fseek($this->index_fp, $index_prev, SEEK_SET);
            fwrite($this->index_fp, pack("L", $index_next), 4);
        }

        return DB_SUCCESS;
    }

    public function get($key)
    {
        $index_offset = ($this->hash_func($key) % $this->db_bucktes_capbility) * 4;
        $ret = fseek($this->index_fp, $index_offset, SEEK_SET);
        if (-1 === $ret)
        {
            echo "fseek $index_offset failed!" . PHP_EOL;
            return DB_ERR_FSEEK_FAILED;
        }
        
        $index_pos = fread($this->index_fp, 4);
        $index_pos = unpack("L", $index_pos);
        $index_pos = $index_pos[1];

        while ($index_pos)
        {
            fseek($this->index_fp, $index_pos, SEEK_SET);
            $index_content = fread($this->index_fp, DB_INDEX_SIZE);
            
            $in_key = substr($index_content, 4, DB_KEY_SIZE);
            if (0 === strncmp($key, $in_key, strlen($key)))
            {
                $data_pos = substr($index_content, 4 + DB_KEY_SIZE, 4);
                $data_pos = unpack("L", $data_pos);
                $data_pos = $data_pos[1];
                $data_len = substr($index_content, 8 + DB_KEY_SIZE, 4);
                $data_len = unpack("L", $data_len);
                $data_len = $data_len[1];

                fseek($this->data_fp, $data_pos, SEEK_SET);
                $value = fread($this->data_fp, $data_len);
                return $value;
            }
            
            $index_pos = substr($index_content, 0, 4);
            $index_pos = unpack("L", $index_pos);
            $index_pos = $index_pos[1];
        }

        return FALSE;
    }

    public function close()
    {
        if ($this->index_fp !== NULL)
        {
            fclose($this->index_fp);
            $this->index_fp = NULL;
        }

        if ($this->data_fp != NULL)
        {
            fclose($this->data_fp);
            $this->data_fp = NULL;
        }
    }

    public function hash_func($key)
    {
        $len = strlen($key);
        $hash = 0;
        for ($i = 0; $i < $len; $i++)
        {
            $hash += $hash * 33 + ord($key[$i]);
        }

        return $hash & 0x7FFFFFFF;
    }

}

?>
