LOG UTIL
===

Log util is a log library for C/C++ designed to record log information;

There are three kinds of Log files, which are:

1. stderr stream
    If the programer does not set log file, then log information will be 
    put into stderr stream

2. regular file
    If the programer set log file that is a regular file, then information 
    will be put into this file

3. time file
    If the programmer set log file using time format, then the log file will
    be changed according the time; for example, set the path: 

```c
    set_log_fmt("b.log.%Y-%m-%d");
```
 
    Then the log file may be b.log-2015-01-15 if current time is 2015/01/15

There are six kinds of log level, which are:
1. FATAL ERROR
2. ERROR
3. WARNING
4. INFO
5. TRACE
6. DEBUG
