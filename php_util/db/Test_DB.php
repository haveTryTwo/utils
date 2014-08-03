<?php

require_once dirname(__FILE__) . "/DB.php";

$db = new DB();

$path = "mytest";
$ret = $db->open($path);
if (DB_SUCCESS !== $ret)
{
    echo "open $path failed" . PHP_EOL;
    return DB_FAILED;
}
echo "open $path success" . PHP_EOL;

$key_1 = "key1";
$value_1 = "value1";

$db->insert($key_1, $value_1);
$v = $db->get($key_1);
echo "key_1: $key_1, value_1: $value_1" . PHP_EOL;
echo "DB => key_1: $key_1, value_1: $v" . PHP_EOL;

$key_2 = "key2";
$value_2 = "value2";
$db->insert($key_2, $value_2);
$v = $db->get($key_2);
echo "key_2: $key_2, value_2: $value_2" . PHP_EOL;
echo "DB => key_2: $key_2, value_2: $v" . PHP_EOL;

$key_3 = "key3";
$value_3 = "value3";
$db->insert($key_3, $value_3);
$v = $db->get($key_3);
echo "key_3: $key_3, value_3: $value_3" . PHP_EOL;
echo "DB => key_3: $key_3, value_3: $v" . PHP_EOL;

$db->remove($key_3);
$v = $db->get($key_3);
echo "DB => key_3: $key_3, value_3: $v" . PHP_EOL;

for ($i = 0; $i < 10000; $i++)
{
    $test_key = "test_key_$i";
    $test_value = "test_value_$i";
    $db->insert($test_key, $test_value);
}

for ($i = 0; $i < 10000; $i++)
{
    $test_key = "test_key_$i";
    $test_value = "test_value_$i";
    $v = $db->get($test_key);
    if ($v !== $test_value)
    {
        echo "Err; key: $test_key, value: $test_value, db_v: $v" . PHP_EOL;
    }
}

$db->close();

?>
