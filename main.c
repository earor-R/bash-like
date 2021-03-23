#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include "utils/string.h"

# define TRUE 1

void prompt();

void readCommand(char*, char*);

void execute(char *command, char *params);

int main() {
    int PID = getpid();
    while (TRUE) {
        char command[MAX_SIZE] = "";
        char params[MAX_SIZE] = "";
        // display prompt on the screen
        prompt();
        readCommand(command, params);
        // nothing to do
        if (!strcmp(command, "")) {
            continue;
        }
        // exit
        if (!strcmp(command, "quit")) {
            _exit(1);
        }
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
        if (!strcmp(params, "")) {
            printf("nothing to do!\n");
            return;
        }
        system(params);
    } else if (!strcmp(command, "list")) {
        system("ps -aux");
    } else if (!strcmp(command, "kill")) {
        if (!strcmp(params, "")) {
            printf("please add the pid after the command!\n");
            return;
        }
        char tmp[16] = "kill -9 ";
        strcat(tmp, params);
        system(tmp);
    } else {
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
    for (int i = 0; input[i] != 32 && input[i] != '\0'; i++) {
        flag = i;
        command[i] = input[i];
    }
    flag += 1;
    command[flag] = '\0';
    flag += 1;
    for (int i = flag; input[i] != '\0' ; i++) {
        params[i - flag] = input[i];
        params[i - flag + 1] = '\0';
    }
}

/**
 * 显示提示符
 */
void prompt() {
    struct passwd *user = getpwuid(getuid());
    char spacer = '$';
    char *dir = NULL;
    dir = getcwd(NULL, 0);
    // root
    if (strcmp(user->pw_name, "root") == 0) {
        spacer = '#';
    }
    char* homePath = user->pw_dir;
    replace(dir, homePath, "~");
    printf("%s:%s%c ", user->pw_name, dir, spacer);
}
