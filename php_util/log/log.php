<?php

class Log
{
    /**
     * log level
     */
    const FATAL_ERROR = 0;  // fatal error
    const ERROR = 1;        // error
    const WARNING = 2;      // warning 
    const NOTICE = 3;       // notice
    const INFO = 4;         // info
    const TRACE = 5;        // trace
    const DEBUG = 6;        // debug

    /**
     * define return type
     */
    const LOG_SUCCESS = 0;              // log success
    const LOG_FILE_OPREATION_ERROR = -1;// log operation file error
    const LOG_LEVEL_NOT_ENOUGH = 1;     // log: the level is not enough

    public static $LEVEL_MAP = array("FATAL_ERROR"=>0, "ERROR"=>1, "WARNING"=>2, 
        "NOTICE"=>3, "INFO"=>4, "TRACE"=>5, "DEBUG"=>6);

    public static $LOG_LEVEL_ARR = array("FATAL_ERROR", "ERROR", "WARNING", 
        "NOTICE", "INFO", "TRACE", "DEBUG");

    private static $log_level = self::DEBUG;    // defautl log level 
    private $log_path;
    private $log_fp = null;

    private static $log = null;

    private function init()
    {
        date_default_timezone_set("PRC");
        
        $this->log_path = dirname(__FILE__) . "/logs/zk.log.";
    }

    private function __construct()
    {
        self::init();
    }

    public function __destruct()
    {
        if ($this->log_fp != null)
        {
            fclose($this->log_fp);
        }
    }

    public static function get_instance()
    {
        if (self::$log === null)
        {
            self::$log = new Log();
        }

        return self::$log;
    }

    /**
     * fatal error log, level : 0
     */
    public function fatal_error($log_message)
    {
        return $this->print_log(self::FATAL_ERROR, $log_message);
    }

    /**
     * error log, level : 1
     */
    public function error($log_message)
    {
        return $this->print_log(self::ERROR, $log_message);       
    }

    /**
     * information log, level : 4
     */
    public function info($log_message)
    {
        return $this->print_log(self::INFO, $log_message);
    }

    /**
     * warning log, level : 2
     */
    public function warning($log_message)
    {
        return $this->print_log(self::WARNING, $log_message);
    }

    /**
     * notice log, level : 3
     */
    public function notice($log_message)
    {
        return $this->print_log(self::NOTICE, $log_message);
    }

    /**
     * debug log, level : 6
     */
    public function debug($log_message)
    {
        return $this->print_log(self::DEBUG, $log_message);
    }

    /**
     * trace log, level : 5
     */
    public function trace($log_message)
    {
        return $this->print_log(self::TRACE, $log_message);
    }

    public function print_log($cur_level, $log_message)
    {
        if ($cur_level <= self::$log_level && $cur_level >= 0)
        {
 //           echo $cur_level . " : " . self::$log_level . "\n";
            if ($this->create_or_readfile())
            { // write the message into the file
                $message = date("Y-m-d H:i:s") . " - [" . self::$LOG_LEVEL_ARR[$cur_level] . "] " 
                    . $log_message . "\n";
                
                flock($this->log_fp, LOCK_EX);
                fwrite($this->log_fp, $message, strlen($message));
                flock($this->log_fp, LOCK_UN);       
             
                return self::LOG_SUCCESS; // write success
            }
            else
            { // cann't create or read the log file
                return LOG_FILE_OPREATION_ERROR; // create or read the log file false
            }
        }

        return LOG_LEVEL_NOT_ENOUGH; // current level can't write
    }

    public function create_or_readfile()
    {
        $date = date("Ymd"); 
        $path = $this->log_path . $date;
        
        if (file_exists($path))
        { // if file exist
            return $this->log_fp = fopen($path, "a+");
        }
        else
        {
            if (!file_exists(dirname($path)))
            {
                mkdir(dirname($path));
            }
   //         echo "not exists\n";
            if (touch($path))
            {
                return $this->log_fp = fopen($path, "a+");
            }
            else
            {
   //             echo "touch faile\n";
                return false;
            }
        }
    }

    /**
     * get the current log path
     */
    public function get_log_path()
    {
        $date = date("Ymd");
        return $this->log_path . $date;
    }

    /**
     * set the log level
     */
    public static function set_log_level($level)
    {
        self::$log_level = $level;
    }

    public static function get_log_level()
    {
        return self::$log_level;
    }
}
?>
