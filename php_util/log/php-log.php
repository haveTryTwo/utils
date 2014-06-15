<?php 

require_once dirname(__FILE__) . "/aop-proxy/proxy-factory.php";
require_once dirname(__FILE__) . "/aop-proxy/proxy-handler.php";
require_once dirname(__FILE__) . "/aop-proxy/proxy.php";

class PhpLogConfHandler implements ProxyHandler
{
    /**
     * the varibles that need to be reset
     */
    private $error_reporting;
    private $display_errors;
    private $log_errors;
    private $error_log;
    
    private $cur_log_path;              // the log path of current API
    public static $cur_log_level;

    /**
     * construct function: get the php log infomation
     *
     * @param path: the path of current log 
     */
    public function __construct($path)
    {
        $this->cur_log_path = $path;
        self::$cur_log_level = E_ERROR | E_WARNING | E_CORE_ERROR | E_CORE_WARNING 
            | E_USER_ERROR | E_USER_WARNING | E_RECOVERABLE_ERROR;

        // get the php log information from php.ini
        $this->display_errors = ini_get("display_errors");
        $this->log_errors = ini_get("log_errors");
        $this->error_log = ini_get("error_log");
        $this->error_reporting = ini_get("error_reporting");
        
   //     echo $this->display_errors . " : " . $this->log_errors . " : " . $this->error_log . ".\n";
    }

    public function __destruct()
    {}

    public function before_working($param = null)
    {
        ini_set("display_errors", "Off");
        ini_set("log_errors", "On");
        ini_set("error_log", $this->cur_log_path);
        ini_set("error_reporting", self::$cur_log_level);
    }

    public function after_working($param = null)
    {
        ini_set("display_errors", $this->display_errors);
        ini_set("log_errors", $this->log_errors);
        ini_set("error_log", $this->error_log);
        ini_set("error_reporting", $this->error_reporting);
    }
}

?>
