#include "global.h"

void moveFiles(vector<string> source, string destination)
{
    while (!source.empty())
    {
        struct stat thestat;
        char srcFile[1024];
        strcpy(srcFile, source.back().c_str());
        stat(srcFile, &thestat);
        char *src = realpath(srcFile, NULL);
        char *des = realpath(destination.c_str(), NULL);
        char destDirectory[1024];
        strcpy(destDirectory, destination.c_str());
        strcat(destDirectory, "/");
        int pos = mycompare(src, des);
        int len = strlen(src) - pos;
        string s = string(src);
        cout << s.substr(pos + 1, len).c_str();
        strcat(destDirectory, s.substr(pos + 1, pos + len).c_str());
        rename(src, destDirectory);
        source.pop_back();
    }
    printf("files moved successfully");
    
}
