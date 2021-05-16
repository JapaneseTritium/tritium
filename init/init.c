//
// Created by qkr on 2021/5/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <linux/reboot.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <ctype.h>

#include "init.h"

void mountvirtfs(){

    mount("proc", "/proc", "proc", MS_NOEXEC | MS_NOSUID | MS_NODEV, NULL);
    mount("sys", "/sys", "sysfs", MS_NOEXEC | MS_NOSUID | MS_NODEV, NULL);
    mount("dev", "/dev", "devtmpfs", MS_NOSUID, "mode=0755");
}
int main(int argc, char **argv) {
    if (getpid() != 1) {
        fputs("Never use this command as a single program", stderr);
        return EXIT_FAILURE;
    }

    signal(SIGUSR1, handler);
    signal(SIGINT, handler);
    signal(SIGCHLD, handler);

mountvirtfs();

    int fd;
    struct stat status;
    char *hostname = "linux";
/*
    FILE *f = fopen("/etc/hostname", "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        rewind(f);
        char *string = malloc(fsize + 1);
        fread(string, 1, fsize, f);
        fclose(f);

        string = trim_space(string);
        fsize = strlen(string);

        printf("sethostname %s %d", string, fsize);
        sethostname(string, fsize);
    }
*/

//
//    if ((fd = open("/etc/hostname", O_RDONLY)) < 0 ||
//        fstat(fd, &status) < 0)
//        goto set_hostname;
//
//    hostname = (char *) mmap(0, status.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
//    close(fd);
//
//    if (hostname == MAP_FAILED) hostname = "linux";
//
//    set_hostname:
//    fd = open("/proc/sys/kernel/hostname", O_WRONLY);
//    write(fd, (void *) hostname, strlen(hostname));
//    close(fd);

//    sethostname(hostname, strlen(hostname));

//    munmap(hostname, status.st_size);

    execute("/bin/sh");
}

static void execute(char *path) {
    char *command[] = {path, NULL};
    pid_t pid = fork();

    if (pid == 0)
        execv(path, command);
    else if (pid)
        waitpid(pid, NULL, 0);
}


char *trim_space(char *str) {
    char *end;
    /* skip leading whitespace */
    while (isspace(*str)) {
        str = str + 1;
    }
    /* remove trailing whitespace */
    end = str + strlen(str) - 1;
    while (end > str && isspace(*end)) {
        end = end - 1;
    }
    /* write null character */
    *(end + 1) = '\0';
    return str;
}


static void handler(int signal) {
    int flag;

    switch (signal) {
        case SIGINT:
            flag = LINUX_REBOOT_CMD_RESTART;
            break;
        case SIGUSR1:
            flag = LINUX_REBOOT_CMD_POWER_OFF;
            break;
        case SIGCHLD:
            waitpid(-1, NULL, WNOHANG);
            return;
        default:
            return;
    }

    sync();
    reboot(flag);
}

