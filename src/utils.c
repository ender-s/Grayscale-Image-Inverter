#include "utils.h"
#include <sys/stat.h>

int check_if_file_exists(const char* path)
{
    struct stat file_stat;
    int result = stat(path, &file_stat);
    return result == 0;
}