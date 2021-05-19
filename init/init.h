//
// Created by qkr on 2021/5/15.
//

#ifndef CHUAN_INIT_H
#define CHUAN_INIT_H

#include <stdio.h>
#include <signal.h>
#include <sys/mount.h>
#include <linux/reboot.h>
#include <linux/wait.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/reboot.h>


static void handler(int signal);

char *trim_space(char *str);

static void execute(char *path);
void mountvirtfs();
#endif				//CHUAN_INIT_H
