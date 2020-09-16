#include "global.h"

void copyFile(string source, string destination);
void copyDir(string source, string destination);
int mycompare(char *a, char *b);

void copyFiles(vector<string> source, string destination)
{
    while (!source.empty())
    {
        struct stat thestat;
        char srcFile[1024];
        strcpy(srcFile, source.back().c_str());
        stat(srcFile, &thestat);
        char *src = realpath(srcFile, NULL);
        char *des = realpath(destination.c_str(), NULL);

        if ((thestat.st_mode & S_IFMT) == S_IFDIR)
        {
            char destDirectory[1024];
            strcpy(destDirectory, destination.c_str());
            strcat(destDirectory, "/");
            int pos = mycompare(src, des);
            int len = strlen(src) - pos;
            string s = string(src);
            cout << s.substr(pos + 1, len).c_str();
            strcat(destDirectory, s.substr(pos + 1, pos + len).c_str());
            mkdir(destDirectory, 0777);
            copyDir(string(src), string(destDirectory));
        }
        else
        {
            char destDirectory[1024];
            strcpy(destDirectory, destination.c_str());
            strcat(destDirectory, "/");
            strcat(destDirectory, srcFile);
            copyFile(string(src), string(destDirectory));
        }
        source.pop_back();
    }
    printf("files copied successfully");
    
}

void copyDir(string source, string destination)
{
    struct dirent *entries;
    DIR *dir;
    struct stat thestat;
    if ((dir = opendir(source.c_str())) != NULL)
    {
        while ((entries = readdir(dir)) != NULL)
        {
            if (strcmp(entries->d_name, "..") == 0 || strcmp(entries->d_name, ".") == 0)
                continue;
            char filename[1024];
            strcpy(filename, source.c_str());
            strcat(filename, "/");
            strcat(filename, entries->d_name);
            stat(filename, &thestat);
            char *src = realpath(filename, NULL);
            char destDirectory[1024];
            strcpy(destDirectory, destination.c_str());
            strcat(destDirectory, "/");
            strcat(destDirectory, entries->d_name);
            char *des = destDirectory;
            if ((thestat.st_mode & S_IFMT) == S_IFDIR)
            {
                mkdir(des, 0777);
                copyDir(string(src), string(des));
            }
            else
            {
                copyFile(string(src), string(des));
            }
        }
        closedir(dir);
    }
}

void copyFile(string source, string destination)
{
    int inputFd;
    char temp[1024];
    strcpy(temp, source.c_str());
    inputFd = open(realpath(temp, NULL), O_RDONLY);

    if (inputFd == -1)
    {
        printf("cannot open file %s\n", temp);
    }
    else
    {
        int openFlags;
        int outputFd;
        mode_t filePerms;
        ssize_t numRead;
        char buf[1024];
        openFlags = O_CREAT | O_WRONLY | O_TRUNC;
        filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
        outputFd = open(destination.c_str(), openFlags, filePerms);

        if (outputFd == -1)
            printf("error opening output file %s\n", destination.c_str());
        else
        {
            struct stat fst;
            fstat(inputFd, &fst);
            fchown(outputFd, fst.st_uid, fst.st_gid);
            fchmod(outputFd, fst.st_mode);
            while ((numRead = read(inputFd, buf, 1024)) > 0)
            {
                if (write(outputFd, buf, numRead) != numRead)
                {
                    printf("error coping file\n");
                }
            }
            close(outputFd);
        }
        close(inputFd);
    }
}

int mycompare(char *a, char *b)
{
    int ind = 0;
    for (int i = 0; unsigned(i) < strlen(a); i++)
    {
        if (a[i] == '/')
            ind = i;
    }
    return ind;
}