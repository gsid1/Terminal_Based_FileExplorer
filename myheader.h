#ifndef MyHeader_H
#define MyHeader_H

#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string>
#include <sstream>
#include <termios.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <stack>
#include <fcntl.h>
#include <unordered_map>
#include <map>

using namespace std;

void listFilesAndDirectories(char *);

#endif