#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>

int list_dir(const char *path, int depth);
int print_path_info(const char* dir, const char *file_name, int depth);

int main(int argc, char* argv[])
{
    int i = 0;

    if (argc == 1)
    {
        list_dir(".", 1);
    }
    else if (argc >= 2)
    {
        for (i = 1; i < argc; i++)
        {
            list_dir(argv[i], 1);
        }
    }

    return 0;
}

int list_dir(const char *path, int depth)
{
    DIR *dir = NULL;
    struct dirent* file = NULL;
    char *file_name = NULL;
    char buf[1024 * 100]; // length is larger than 10k, so don't initialize the array
    struct stat st;
    int ret = 0;

    memset(&st, 0, sizeof(struct stat));

    ret = lstat(path, &st);
    if (ret == -1)
    {
        printf("can access path:%s, err:%s\n", path, strerror(errno));
        return -1;
    }

    if (S_ISDIR(st.st_mode) == 0)
    {
        printf("path:%s is not diretory!\n", path);
        return -1;
    }

    dir = opendir(path);
    if (dir == NULL)
    {
        printf("path:%s open failed! error:%s\n", path, strerror(errno));
        return -1;
    }
    while ((file = readdir(dir)) != NULL)
    {
        file_name = file->d_name;
        if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0)
        {
            continue;
        }

        print_path_info(path, file_name, depth);

        if (file->d_type == DT_DIR)
        {
            snprintf(buf, sizeof(buf), "%s/%s", path, file_name);
            list_dir(buf, depth + 1);
        }
    }
    closedir(dir);

    return 0;
}

int print_path_info(const char* dir, const char *file_name, int depth)
{
    char show[1024 * 10] = {0};
    struct stat st;
    int ret = 0;
    char path[1024 * 100]; // the length is lager than 10k, so don't initialize the array
    
    memset(&st, 0, sizeof(struct stat));
    snprintf(path, sizeof(path), "%s/%s", dir, file_name);

    // set the show
    show[0] = '|';
    if (depth == 1)
    {
        memset(show + 1, '-', 2 * depth);
    }
    else
    {
        memset(show + 1, ' ', 3 * depth - 4);
        show[3 * depth - 3] = '|';
        memset(show + 3 * depth - 2, '-', 2);
    }

    ret = lstat(path, &st);
    if (ret == -1)
    {
        printf("can access path:%s, err:%s\n", path, strerror(errno));
        return -1;
    }

    if (S_ISDIR(st.st_mode))
    {
        printf("%s\033[1;34m%s\033[0m\n", show, file_name);
    }
    else if (S_ISREG(st.st_mode))
    {
        printf("%s%s \n", show, file_name);
    }
    else if (S_ISCHR(st.st_mode))
    {
        printf("%s\033[1;33m%s\033[0m\n", show, file_name);
    }
    else if (S_ISBLK(st.st_mode))
    {
        printf("%s\033[1;32m%s\033[0m\n", show, file_name);
    }
    else if (S_ISFIFO(st.st_mode))
    {
        printf("%s\033[1;35m%s\033[0m\n", show, file_name);
    }
    else if (S_ISLNK(st.st_mode))
    {
        printf("%s\033[1;36m%s\033[0m\n", show, file_name);
    }
    else if (S_ISSOCK(st.st_mode))
    {
        printf("%s\033[1;37m%s\033[0m\n", show, file_name);
    }
    else
    {
        printf("unknown file type of path:%s\n", path);
    }

    return ret;
}
