<?php

interface ProxyHandler
{
    public function before_working($param = null);

    public function after_working($param = null);
}

?>
