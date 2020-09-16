#include "global.h"

void commandMode()
{
    fflush(stdin);
    // printf("inside command mode");
    vector<string> source;
    string destination;
    char command[1024];
    cursorforward(1);
    scanf("%s", command);
    if (strcmp(command, "copy") == 0)
    {
        stringstream ss;
        string s;
        getline(cin, s);
        ss << s;
        string out;
        while (ss >> out)
        {
            source.push_back(out);
        }
        destination = source.back();
        source.pop_back();
        copyFiles(source, destination);
    }
    else if (strcmp(command, "move") == 0)
    {
        stringstream ss;
        string s;
        getline(cin, s);
        ss << s;
        string out;
        while (ss >> out)
        {
            source.push_back(out);
        }
        destination = source.back();
        source.pop_back();
        moveFiles(source, destination);
    }
    else if (strcmp(command, "rename") == 0)
    {
        stringstream ss;
        string s;
        getline(cin, s);
        ss << s;
        string out;
        while (ss >> out)
        {
            source.push_back(out);
        }
        destination = source.back();
        source.pop_back();
        renameFiles(source, destination);
    }
    else if (strcmp(command, "create_file") == 0)
    {
        string source, destination;
        cin >> source >> destination;
        createFile(source, destination);
    }
    else if (strcmp(command, "create_dir") == 0)
    {
        string source, destination;
        cin >> source >> destination;
        createDir(source, destination);
    }
    else if (strcmp(command, "delete_file") == 0)
    {
        string target;
        cin >> target;
        int fd = remove(target.c_str());
        if (fd == 0)
            printf("file removed successfully");
        else
            printf("file not deleted");
    }
    else if (strcmp(command, "delete_dir") == 0)
    {
        string target;
        cin >> target;
        int fd = remove(target.c_str());
        if (fd == 0)
            printf("directory removed successfully");
        else
            printf("directory not deleted");
    }
    else if (strcmp(command, "goto") == 0)
    {
        string target;
        cin >> target;
        char *src = realpath(target.c_str(), NULL);
        cout << src << "$$$";
        int fd = chdir(src);
        if (fd == 0)
        {
            initializeAll();
            listFilesAndDirectories(src);
            printf("directory changed successfully");
        }
        else
            printf("directory not changed");
    }
    else if (strcmp(command, "search") == 0)
    {
        string target;
        cin >> target;
        const char *src = target.c_str();
        // cout << src << "$$$";
        search(src);
    }
    else if (strcmp(command, "snapshot") == 0)
    {
        char dir[1024], file[1024];
        cin >> dir >> file;
        // cout << src << "$$$";
        createSnapshot(dir,file);
    }
    else
    {
        printf("else");
    }
    // listFilesAndDirectories(currentDirectory);
}
