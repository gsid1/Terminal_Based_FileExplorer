#include "global.h"

void createFile(string source, string destination)
{
    char srcFile[1024];
    strcpy(srcFile, destination.c_str());
    strcat(srcFile, "/");
    strcat(srcFile, source.c_str());
    // char *src = realpath(srcFile, NULL);
    int fd = open(srcFile, O_RDONLY | O_CREAT);
    if (fd == -1)
        printf("cannot create file");
    else
    {
        printf("file created successfully");
        close(fd);
    }
}

void createDir(string source, string destination)
{
    char srcDir[1024];
    strcpy(srcDir, destination.c_str());
    strcat(srcDir, "/");
    strcat(srcDir, source.c_str());
    // char *src = realpath(srcFile, NULL);
    cout << "###" << srcDir << "###";
    int fd = mkdir(srcDir, 0777);
    if (fd == -1)
        printf("cannot create directory");
    else if (fd == 0)
    {
        printf("directory created successfully");
        close(fd);
    }
}
