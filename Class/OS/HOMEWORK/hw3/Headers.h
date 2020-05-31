#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "fs.h"
#define MODE 0644
#define BLOCK_NUM 512
enum {FAILD=-1, SUCCESS};
