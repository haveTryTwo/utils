<?php

require_once dirname(__FILE__) . "/InfoUtils.php";

class HashNode
{
    public $key;
    public $value;
    public $nextNode;

    public function __construct($key, $value, $nextNode = NULL)
    {
        $this->key = $key;
        $this->value = $value;
        $this->nextNode = $nextNode;
    }

}

class HashMap
{
    private $buckets;
    private $capbility;
    private $size;

    public function __construct($map_size = MAP_DEFAULT_SIZE)
    {
        // $this->buckets = array($this->capbility);
        $this->buckets = new SplFixedArray($map_size);
        $this->size = 0;
        $this->capbility = $map_size;
    }

    public function size()
    {
        return $this->size;
    }

    public function buckets_size()
    {
        return $this->buckets->getSize();
    }

    public function insert($key, $value)
    {
        if ($this->size >= $this->capbility / 2)
        {
            $this->balance_map();
        }

        $index = $this->hash_func($key) % $this->capbility;
        if ($this->get($key) !== NULL)
        {
            return MAP_KEY_EXISTS;
        }

        if ($this->buckets[$index] === NULL)
        {
            $newNode = new HashNode($key, $value, NULL);
        }
        else
        {
            $newNode = new HashNode($key, $value, $this->buckets[$index]);
        }

        $this->buckets[$index] = $newNode;
        $this->size += 1;
    }

    public function insert_or_update($key, $value)
    {
        $index = $this->hash_func($key) % $this->capbility;

        $node = $this->buckets[$index];
        while ($node !== NULL)
        {
            if ($key === $node->key)
            {
                $node->value = $value;
                return MAP_SUCCESS;
            }
            $node = $node->nextNode;
        }

        if ($this->buckets[$index] === NULL)
        {
            $newNode = new HashNode($key, $value, NULL);
        }
        else
        {
            $newNode = new HashNode($key, $value, $this->buckets[$index]);
        }

        $this->buckets[$index] = $newNode;
        $this->size += 1;

        return MAP_SUCCESS;
    }

    public function get($key)
    {
        $index = $this->hash_func($key) % $this->capbility;

        $node = $this->buckets[$index];
        while ($node !== NULL)
        {
            if ($key === $node->key)
            {
                return $node->value;
            }

            $node = $node->nextNode;
        }

        return NULL;
    }

    public function remove($key)
    {
        $index = $this->hash_func($key) % $this->capbility;

        $node = $this->buckets[$index];
        $isFirst = TRUE;
        while ($node !== NULL)
        {
            if ($key === $node->key)
            {
                if ($isFirst)
                {
                    $this->buckets[$index] = $node->nextNode;
                    $this->size -= 1;
                    return MAP_SUCCESS;
                }
                else
                {
                    $pre->$nextNode = $node->nextNode; 
                    $this->size -= 1;
                    return MAP_SUCCESS;
                }
            }

            $isFirst = FALSE;
            $pre = $node;
            $node = $node->nextNode;
        }

        return MAP_KEY_NOT_EXISTS;
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

    private function balance_map()
    {
        $new_capbility = $this->capbility * 2;
        $new_buckets = new SplFixedArray($new_capbility);
        
        for ($i = 0; $i < $this->capbility; $i++)
        {
            $node = $this->buckets[$i];
            while ($node !== NULL)
            {
                $next = $node->nextNode;
                $node->nextNode = NULL;
                $index = $this->hash_func($node->key) % $new_capbility;
                if ($new_buckets[$index] === NULL)
                {
                    $new_buckets[$index] = $node;
                }
                else
                {
                    $node->nextNode = $new_buckets[$index];
                    $new_buckets[$index] = $node;
                }
                $node = $next;
            }
            $this->buckets[$i] = NULL;
        }

        $this->buckets = $new_buckets;
        $this->capbility = $new_capbility;
    }
}

?>
