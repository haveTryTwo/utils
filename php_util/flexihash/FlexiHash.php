<?php

class FlexiHash
{
    private $serv_list = array();
    private $is_sorted = FALSE;

    public function addServer($serv)
    {
        $hash = $this->myHash($serv);
        if (!isset($this->serv_list[$hash]))
        {
            $this->serv_list[$hash] = $serv;
        }    
        
        $this->is_sorted = FALSE;

        return TRUE;
    }
    
    public function removeServer($serv)
    {
        $hash = $this->myHash($serv);
        if (isset($this->serv_list[$hash]))
        {
            unset($this->serv_list[$hash]);
        }

        $this->is_sorted = FALSE;

        return TRUE;
    }

    public function findServer($key)
    {
        $hash = $this->myHash($key);
        if ($this->is_sorted === FALSE)
        {
            krsort($this->serv_list);
            $this->is_sorted = TRUE;
        }

        foreach ($this->serv_list as $pos => $serv)
        {
            if ($hash >= $pos)
            {
                return $serv;
            }
        }

        return $this->serv_list[count($this->serv_list) - 1];
    }

    public function listServer()
    {
        if ($this->is_sorted === FALSE)
        {
            krsort($this->serv_list);
            $this->is_sorted = TRUE;
        }

        var_dump($this->serv_list);

        return TRUE;
    }

    private function myHash($key)
    {
        $myHash = substr(md5($key), 0, 8);
        $seed = 31;
        $hash = 0;

        for ($i = 0; $i < 8; $i++)
        {
            $hash = $hash * $seed + ord($myHash[$i]);
            $i++;
        }

        return $hash & 0x7FFFFFFF;
    }
}

?>
