#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define FILE_PATH_LEN           2048
#define MAX_BUF_LEN             2048
   
#define OK                      0
#define ERR_NULL_PARAM          1
#define ERR_FAILED_OPEN_FILE    2

#define LOG_LVL_FATAL_ERR       5
#define LOG_LVL_ERR             4
#define LOG_LVL_WARN            3
#define LOG_LVL_INFO            2
#define LOG_LVL_TRACE           1
#define LOG_LVL_DEBUG           0

#define LOG_FATAL_ERR(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_FATAL_ERR, format, ## __VA_ARGS__)
#define LOG_ERR(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_ERR, format, ## __VA_ARGS__)
#define LOG_WARN(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_WARN, format, ## __VA_ARGS__)
#define LOG_INFO(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_INFO, format, ## __VA_ARGS__)
#define LOG_TRACE(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_TRACE, format, ## __VA_ARGS__)
#define LOG_DEBUG(format, ...) print_log(__FILE__, __LINE__, LOG_LVL_DEBUG, format, ## __VA_ARGS__)

void set_log_level(int level);
void set_log_fmt(const char *log_path_fmt);
void log_init(const char *log_path_fmt, int level);
void log_stream_close();
void log_destroy();
void print_log(const char* file_path, int line_no, int log_level, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif
