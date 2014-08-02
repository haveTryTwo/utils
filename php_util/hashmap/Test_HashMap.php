<?php

require_once dirname(__FILE__) . "/HashMap.php";

$map = new HashMap();

$map->insert("zhangsan", "1231,good study");
echo "map size: " . $map->size(). PHP_EOL;
$map->insert("test_key1", "test_value1");
echo "map size: " . $map->size(). PHP_EOL;

echo $map->get("test_key1") . PHP_EOL;
echo $map->get("zhangsan") . PHP_EOL;

$map->remove("test_key1");
echo "map size: " . $map->size(). PHP_EOL;

for ($i = 0; $i < 100; $i++)
{
    $map->insert("map_key_$i", "map_value_$i");
}

for ($i = 0; $i < 100; $i++)
{
    echo $map->get("map_key_$i") . PHP_EOL;
}

echo $map->get("map_key_0") . PHP_EOL;
echo "map size: " . $map->size(). PHP_EOL;
echo $map->insert("map_key_0", "map_map_map_key_test0") . PHP_EOL;
echo $map->get("map_key_0") . PHP_EOL;
echo "map size: " . $map->size(). PHP_EOL;

echo $map->insert_or_update("map_key_0", "map_map_map_key_test0") . PHP_EOL;
echo $map->get("map_key_0") . PHP_EOL;
echo "map size: " . $map->size(). PHP_EOL;

echo $map->buckets_size() . PHP_EOL;

?>
