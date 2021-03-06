//
// Created by qkr on 2021/5/15.
//


#include "init.h"

void triggerRunlevel(char *argv)
{
    if (argv[0] == '0') {
	kill(1, SIGINT);
    } else if (argv[0] == '6') {
	kill(1, SIGUSR1);
    } else {
	printf("cannot find runlevel %s", argv);
    }
}

void init_sethostname()
{

    int fd;
    char *hostname = "linux";
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

	printf("sethostname %s %ld", string, fsize);
	sethostname(string, fsize);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2) {
	triggerRunlevel(argv[1]);
	return 0;
    }

    if (getpid() != 1) {
	fputs("Never use this command as a single program", stderr);
	return 1;
    }

    signal(SIGUSR1, handler);
    signal(SIGINT, handler);
    signal(SIGCHLD, handler);

    mountvirtfs();

    init_sethostname();

    execute("/bin/sh");
}

static void execute(char *path)
{
    char *command[] = { path, NULL };
    pid_t pid = fork();

    if (pid == 0)
	execv(path, command);
    else if (pid)
	waitpid(pid, NULL, 0);
}


char *trim_space(char *str)
{
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


static void handler(int signal)
{
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

void mountvirtfs()
{
    mount("proc", "/proc", "proc", MS_NOEXEC | MS_NOSUID | MS_NODEV, NULL);
    mount("sys", "/sys", "sysfs", MS_NOEXEC | MS_NOSUID | MS_NODEV, NULL);
    mount("dev", "/dev", "devtmpfs", MS_NOSUID, "mode=0755");
}
