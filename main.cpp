#include "global.h"


int main(int argc, char *argv[])
{
    // printf("inside main") ;
    initializeAll();
    getWindowSize();
    string defaultDir = ".";
    char *currentDirectory = new char[defaultDir.length() + 1];
    strcpy(currentDirectory, defaultDir.c_str());
    // printf("%s",currentDirectory) ;
    listFilesAndDirectories(currentDirectory);
    
}
