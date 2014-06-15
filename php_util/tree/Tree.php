<?php

class Tree
{
    public $reso = NULL;
    public $index_map = NULL;
    public $tree = NULL;

    public function init($node_id, $parent_id, $node)
    {
        $this->reso[$node_id] = $node;
        $this->index_map[$parent_id][] = $node_id;
    }

    public function getChild($parent_id)
    {
        if($this->index_map[$parent_id] === NULL)
        {
            $node = $this->reso[$parent_id];
            $node["childs"] = "";
            return $node;
        }

        $parent = $this->reso[$parent_id];
        foreach ($this->index_map[$parent_id] as $child_id)
        {
            $parent["childs"][] = self::getChild($child_id);
        }

        return $parent;
    }

    public function getTree()
    {
        $childs = self::getChild("0");
        $this->tree = $childs["childs"];

        return $this->tree;
    }
}

?>

<?php

$nodes = array(
    "1" => array("node_id" => 1,
                 "parent_id" => 0,
                 "value" => "1111"
             ),
    "2" => array("node_id" => 2,
                 "parent_id" => 0,
                 "value" => "2222"
                ), 
    "3" => array("node_id" => 3,
                 "parent_id" => 0,
                 "value" => "3333"
             ),
    "4" => array("node_id" => 4,
                 "parent_id" => 1,
                 "value" => "44444"
             ),
    "5" => array("node_id" => 5,
                 "parent_id" => 1,
                 "value" => "55555"
             ),
    "6" => array("node_id" => 6,
                 "parent_id" => 2,
                 "value" => "66666"
             )
    );

$tree = new Tree();
foreach ($nodes as $node)
{
    $tree->init($node['node_id'], $node["parent_id"], $node);    
}

$t = $tree->getTree();
var_dump($t);

?>
