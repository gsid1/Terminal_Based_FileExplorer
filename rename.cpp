#include "global.h"

void renameFiles(vector<string> source, string destination)
{
    while (!source.empty())
    {
        struct stat thestat;
        char srcFile[1024];
        strcpy(srcFile, source.back().c_str());
        stat(srcFile, &thestat);
        char *src = realpath(srcFile, NULL);
        // char *des = realpath(destination.c_str(), NULL);
        // cout<<src<<" "<<des ;
        int success=rename(src, destination.c_str());
        if(success==-1)
        {
            printf("cannot rename the file") ;
        }
        source.pop_back();
    }
    printf("files moved successfully");
}
