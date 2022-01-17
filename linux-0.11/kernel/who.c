#include<unistd.h>
#include<errno.h>
#include<asm/segment.h>
#include<string.h>

char name_in_kernel[24];

/**
 * @brief 系统调用iam，将name存入内核空间，最多23个字符(不包括\0)
 * 
 * @param name 用户态传过来的字符串指针
 * @return int 返回name的长度
 */
int sys_iam(const char *name) {
    
    // 高效防止 '\0' bug
    memset(name_in_kernel, '\0', sizeof(name_in_kernel));

    int cnt = 0;        // name的长度
    char c;             // 要一个字符(byte)一个字符的通过fs寄存器从用户态读

    while ((c = get_fs_byte(name + cnt)) != '\0') {
        name_in_kernel[cnt++] = c;
        if (cnt > 23) {
            memset(name_in_kernel, '\0', sizeof(name_in_kernel));
            errno = EINVAL;
            return -1;
        }
    }
    
    errno = 0;
    return cnt;
}

int sys_whoami(char *name, unsigned int size) {

    unsigned int len = strlen(name_in_kernel);
    if (size < len) {
        put_fs_byte('\0', name + 0);
        errno = EINVAL;
        return -1;
    }

    unsigned int i;

    for (i = 0; i <= len; i++) {
        put_fs_byte(name_in_kernel[i], name + i);
    }

    errno = 0;
    return len;
}




