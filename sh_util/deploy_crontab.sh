function start_php_monitor()
{
    php_bin=$1
    if [ "$php_bin" = "" ]; then
        php_bin="/usr/local/php/bin/php"
        echo "Using default php_bin: $php_bin"
    else
        echo "Input php bin is: $php_bin"
    fi
    
    cron_root="/var/spool/cron/root"
    cmd="cd /home/test/monitor && $php_bin monitor.php >> /dev/null 2>&1";
    status=`sudo grep "$cmd" $cron_root`
    if [ -z "$status" ]; then
        echo "'$cmd' is not in the corntab"
        sudo echo "*/1 * * * * $cmd" >> $cron_root
        echo "insert '$cmd' into the crontab SUCCESS!"
    else 
        status=`sudo grep "$cmd" $cron_root | sudo grep "^#"`
        if [ -z "$status" ]; then
            echo "'$cmd' is already in the crontab"
        else
            echo 
            echo ' *********************** '
            echo "Notice: '$cmd' has been annotated, You should deal with it! Using 'sudo crontab -e'!"
            echo ' *********************** '
            echo
        fi
    fi

    exit 0
}

# add the entry of monitor into crontab
start_php_monitor $1
