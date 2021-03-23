//
// Created by era on 2021/3/21.
//

#ifndef W_STRING_H
#define W_STRING_H
#define MAX_SIZE 256

#include <string.h>

int assertLength(char *);

/**
 * 字符串替换函数
 * @param str 需要操作的字符串
 * @param oriStr 待替换
 * @param newStr 用来替换的字符串
 * @return 0 失败， 1 成功
 */
int replace (char* str, char* oriStr, char* newStr) {
    if (!assertLength(str)) {
        return 0;
    }
    int oriLen = strlen(oriStr);

    char *idx = strstr(str, oriStr);
    if (idx == NULL) {
        return 0;
    }
    char str1[MAX_SIZE] = "";
    char str2[MAX_SIZE] = "";
    int flag = -1;
    for (int i = 0; i < strlen(str); ++i) {
        if (idx == &str[i]) {
            flag = i;
            i += strlen(oriStr);
        }
        if (flag == -1) {
            str1[i] = str[i];
        } else {
            str2[i - flag - oriLen] = str[i];
        }
    }
    strcat(str1, newStr);
    strcat(str1, str2);
    strcpy(str, str1);
    return 1;
}

/**
 * 允许最长字符串为256
 * @param str
 * @return 1：小于256, 0：大于256
 */
int assertLength (char* str) {
    if (strlen(str) > MAX_SIZE) {
        return 0;
    } else {
        return 1;
    }
}

/**
 * 将字符串切成小片
 * @param str 要切片的字符串
 * @param delim 分隔符
 * @param slice 切成的小片
 * @return 切分的片数， 0表示出错
 */
int split (char* str, char* delim, char** slice) {
    int idx = 0;
    char* tmp = strtok(str, delim);
    while (tmp != NULL) {
        slice[idx] = tmp;
        idx += 1;
        tmp = strtok(NULL, delim);
    }
    slice[idx+1] = NULL;
    return idx;
}

/**
 * 将字符串数组拼接成一个字符串
 * @param slice 要拼接的字符串
 * @param delim 分隔符
 * @param rst 拼接后的字符串
 */
void join (char** slice, char* delim, char* rst) {
    strcpy(rst, slice[0]);
    for (int i = 1; slice[i] != NULL; i++) {
        if (delim != NULL && i > 1) {
            strcat(rst, delim);
        }
        strcat(rst, slice[i]);
    }
}
/**
 * 去掉字符串两端的空格
 * @param str
 */
void trim (char* str) {
    int start = 0, end = 0;
    int n = strlen(str);
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] != 32) {
            start = i;
            break;
        }
    }
    for (int i = n; i > 0; --i) {
        if (str[i] != 32) {
            end = i;
            break;
        }
    }
    for (int i = 0; i < end; ++i) {
        str[i] = str[start + i];
    }
    for (int i = end; i < n; ++i) {
        str[i] = '\0';
    }
}
#endif //W_STRING_H
