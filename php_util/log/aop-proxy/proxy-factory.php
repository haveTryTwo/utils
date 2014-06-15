<?php

require_once dirname(__FILE__) . "/proxy.php";

class ProxyFactory
{
    public static $proxy;

    public static function create_proxy()
    {
        self::$proxy = new Proxy();

        return self::$proxy;
    }

}

?>
