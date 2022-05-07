#ifndef CMUCH2
#define CMUCH2

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mush.h>
#include <sys/wait.h>

#define READ_END  0
#define WRITE_END 1

int cd_cmd(clstage stage);

int pipeline_cmd(pipeline cl);

#endif
