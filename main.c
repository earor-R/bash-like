#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sched.h>
#include <threads.h>
#include <wait.h>
#include "utils/string.h"

# define TRUE 1

void prompt();

void readCommand(char*, char*);

void execute(char *command, char *params);

int main() {
    while (TRUE) {
        // 命令
        char command[MAX_SIZE] = "";
        // 参数
        char params[MAX_SIZE] = "";
        // 输入命令前的提示
        prompt();
        // 读取命令
        readCommand(command, params);
        // 啥也没输
        if (!strcmp(command, "")) {
            continue;
        }
        // 退出
        if (!strcmp(command, "quit")) {
            _exit(1);
        }
        // 执行命令
        execute(command, params);
    }
    return 0;
}

/**
 * 执行命令
 * @param command 命令： run、list、kill
 * @param params
 */
void execute(char *command, char *params) {
    if (!strcmp(command, "run")) {
        // run
        if (!strcmp(params, "")) {
            printf("nothing to do!\n");
            return;
        }
        trim(params);
        // 如果指定CPU
        int childPid = 0;
        if (params[0] == '-' && params[1] == 'c') {
            char* coreParam = strtok(params, " ");
            char* core = strtok(NULL, " ");
            // 要执行的命令和参数
            char* paramsTmp[16][MAX_SIZE] = {0};
            // 要执行的命令
            char commandTmp[MAX_SIZE] = "";
            char * buffer = strtok(NULL, " ");
            strcpy(commandTmp, buffer);
            int paramIdx = 0;
            for (int i = 0; i < MAX_SIZE && buffer != NULL; ++i) {
                strcpy(paramsTmp[i], buffer);
                buffer = strtok(NULL, " ");
                paramIdx = i;
            }
            childPid = fork();
            if (childPid == 0) {
                printf("before\n");
                cpu_set_t mask;
                CPU_ZERO(&mask);    //置空
                CPU_SET(core - 48,&mask);   // 将当前线程和CPU绑定
                sched_setaffinity(0, sizeof(mask), &mask);
                if (execvp(commandTmp, paramsTmp) < 0) {
                    printf("fail to execv\n");
                    exit(1);
                }
                printf("after\n");
                exit(1);
            } else {
                wait(childPid);
            }
        } else {
            system(params);
        }

    } else if (!strcmp(command, "list")) {
        // list
        system("ps -aux");
    } else if (!strcmp(command, "kill")) {
        // kill 后需要接pid
        if (!strcmp(params, "")) {
            printf("please add the pid after the command!\n");
            return;
        }
        char tmp[16] = "kill -9 ";
        strcat(tmp, params);
        system(tmp);
    } else {
        // 未知命令 提示
        printf("%s", "unknown command. please enter as follows:\n");
        printf("  run [-r] command: run a command, -r specify a cpu core.\n");
        printf("  list:             lists the currently running processes.\n");
        printf("  kill <pid>:       stop the process numbered pid.\n");
    }
}

/**
 * 读取输入的命令
 * @param command 命令
 * @param params 命令参数
 */
void readCommand(char * command, char* params) {
    char input[MAX_SIZE + 1] = "";
    gets(input);
    if (!assertLength(input)) {
        printf("Command entered is too long!\n");
        return ;
    }
    int flag = 0;
    // 空格前面的是command
    for (int i = 0; input[i] != 32 && input[i] != '\0'; i++) {
        flag = i;
        command[i] = input[i];
    }
    flag += 1;
    command[flag] = '\0';
    flag += 1;
    // 空格后面的是params
    for (int i = flag; input[i] != '\0' ; i++) {
        params[i - flag] = input[i];
        params[i - flag + 1] = '\0';
    }
}

/**
 * 显示提示符，格式为：
 *  用户名:当前目录<$|#>
 */
void prompt() {
    struct passwd *user = getpwuid(getuid());
    // 普通用户的分隔符为 $
    char spacer = '$';
    char *dir = NULL;
    // 获取当前目录
    dir = getcwd(NULL, 0);
    // root 用户的分隔符为 #
    if (strcmp(user->pw_name, "root") == 0) {
        spacer = '#';
    }
    // 用户主目录
    char* homePath = user->pw_dir;
    // 当前工作目录包含用户主目录就替换成~
    replace(dir, homePath, "~");
    printf("%s:%s%c ", user->pw_name, dir, spacer);
}
