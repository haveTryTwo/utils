<?php

define("DB_BUCKETS_SIZE", 262144);
define("DB_KEY_SIZE", 128);
define("DB_INDEX_SIZE", DB_KEY_SIZE + 12);


// define return status
define("DB_SUCCESS", 0);
define("DB_FAILED", -1);
define("DB_KEY_EXIST", 1);
define("DB_KEY_NOT_EXIST", 2);
define("DB_ERR_KEY_EXCEED", 3);

define("DB_ERR_OPEN_FILE_FAILED", 11);
define("DB_ERR_WRITE_FILE_FAILED", 12);
define("DB_ERR_READ_FILE_FAILED", 13);
define("DB_ERR_FSEEK_FAILED", 14);

?>
