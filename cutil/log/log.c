#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#include "log.h"

static const char *get_log_level(int level);
static int check_log(const struct tm *cur_tm);


static char             _log_path[FILE_PATH_LEN] = {0};
static char             _log_path_fmt[FILE_PATH_LEN] = {0};
static int              _log_level = LOG_LVL_ERR; 
static FILE             *_log_fp = NULL;
static pthread_mutex_t  _log_mutex = PTHREAD_MUTEX_INITIALIZER;


void set_log_level(int level)
{
    if (level >= LOG_LVL_DEBUG || level <= LOG_LVL_FATAL_ERR)
    {
        _log_level = level;
    }
}

void set_log_fmt(const char *log_path_fmt)
{
    if (log_path_fmt != NULL && log_path_fmt[0] != '\0')
    {
        snprintf(_log_path_fmt, sizeof(_log_path_fmt), "%s", log_path_fmt);
    }
}

void log_init(const char *log_path_fmt, int level)
{
    set_log_fmt(log_path_fmt);
    set_log_level(level);
}

void log_stream_close()
{
    if (_log_fp != NULL)
    {
        fclose(_log_fp);
        _log_fp = NULL;
    }
}

void log_destroy()
{
    log_stream_close();
    pthread_mutex_destroy(&_log_mutex);
}

void print_log(const char* file_path, int line_no, int log_level, const char* format, ...)
{
    if (log_level < _log_level || NULL == file_path || NULL == format)
        return;

    int n = 0;
    int ret = OK;
    time_t cur;
    struct tm cur_tm = {0};
    const char* file_name = NULL;
    char log_buf[MAX_BUF_LEN] = {0};

    file_name = strrchr(file_path, '/');
    file_name = (file_name == NULL) ? file_path : (file_name + 1);

    pthread_mutex_lock(&_log_mutex);

    time(&cur);
    localtime_r(&cur, &cur_tm);

    ret = check_log(&cur_tm);
    if (OK == ret)
    {
        n = snprintf(log_buf, sizeof(log_buf),
                     "%4d/%02d/%02d %02d:%02d:%02d - [pid: %d][%s][%s %d] - ",
                     cur_tm.tm_year + 1900,
                     cur_tm.tm_mon + 1,
                     cur_tm.tm_mday,
                     cur_tm.tm_hour,
                     cur_tm.tm_min,
                     cur_tm.tm_sec,
                     getpid(),
                     get_log_level(log_level),
                     file_name,
                     line_no);

        va_list arg_ptr;
        va_start(arg_ptr, format);
        vsnprintf(log_buf + n, sizeof(log_buf) - n - 2, format, arg_ptr);
        va_end(arg_ptr);
        strcat(log_buf, "\n");

        fwrite(log_buf, sizeof(char), strlen(log_buf), _log_fp);
        fflush(_log_fp);
    }

    pthread_mutex_unlock(&_log_mutex);
}

static int check_log(const struct tm *cur_tm)
{
    int ret = OK;
    size_t len = 0;
    char log_path[FILE_PATH_LEN] = {0};

    if ('\0' == _log_path_fmt[0])
    {
        _log_fp = _log_fp == NULL ? stderr : _log_fp;
        return OK;
    }

    len = strftime(log_path, sizeof(log_path), _log_path_fmt, cur_tm); 

    if (strncmp(log_path, _log_path, len) != 0)
    {
        if (_log_fp != NULL && _log_fp != stderr)
        {
            fclose(_log_fp);
            _log_fp = NULL;
        }

        snprintf(_log_path, sizeof(_log_path), "%s", log_path);
        _log_fp = fopen(_log_path, "a");
        ret = (_log_fp == NULL) ? ERR_FAILED_OPEN_FILE : ret;
    }
    else
    {
        if (NULL == _log_fp)
        {
            _log_fp = fopen(_log_path, "a");
            ret = (_log_fp == NULL) ? ERR_FAILED_OPEN_FILE : ret;
        }
    }

    return ret;
}

static const char *get_log_level(int level)
{
    switch (level)
    {
    case LOG_LVL_FATAL_ERR:
        return "FATAL ERROR";
    case LOG_LVL_ERR:
        return "ERROR";
    case LOG_LVL_WARN:
        return "WARNING";
    case LOG_LVL_INFO:
        return "INFO";
    case LOG_LVL_TRACE:
        return "TRACE";
    case LOG_LVL_DEBUG:
        return "DEBUG";
    default:
        return "UNKNOWN";
    }
}

#ifdef __LOG_MAIN_TEST
int main(int argc, char *argv[])
{
    LOG_FATAL_ERR("fatal err now");
    LOG_ERR("err now");
    LOG_INFO("info now");

    set_log_level(LOG_LVL_INFO);
    LOG_INFO("info now");

    set_log_fmt("./a.log");
    LOG_FATAL_ERR("fatal err now");
    LOG_ERR("err now");
    LOG_WARN("warning now");
    LOG_INFO("info now");
    LOG_DEBUG("warning now");

    log_init("./b.log.%Y-%m-%d-%H", LOG_LVL_DEBUG);
    LOG_FATAL_ERR("fatal err now");
    LOG_ERR("err now");
    LOG_WARN("warning now");
    LOG_INFO("info now");
    LOG_TRACE("trace now");
    LOG_DEBUG("debug now");

    return 0;
}
#endif
