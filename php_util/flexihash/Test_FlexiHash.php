<?php

require_once dirname(__FILE__) . "/FlexiHash.php";

$flexHash = new FlexiHash();
$flexHash->addServer("192.168.1.1");
$flexHash->addServer("192.168.1.2");
$flexHash->addServer("192.168.1.3");
$flexHash->addServer("192.168.1.4");
$flexHash->addServer("192.168.1.5");

$flexHash->listServer();
echo "key1 in serv: " . $flexHash->findServer("key1") . PHP_EOL;
echo "key2 in serv: " . $flexHash->findServer("key2") . PHP_EOL;

$flexHash->removeServer("192.168.1.4");
$flexHash->listServer();

echo "key1 in serv: " . $flexHash->findServer("key1") . PHP_EOL;
echo "key2 in serv: " . $flexHash->findServer("key2") . PHP_EOL;

$flexHash->addServer("192.168.1.6");
$flexHash->listServer();
echo "key1 in serv: " . $flexHash->findServer("key1") . PHP_EOL;
echo "key2 in serv: " . $flexHash->findServer("key2") . PHP_EOL;

?>
