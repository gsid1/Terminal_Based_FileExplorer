#include "global.h"

void listFilesAndDirectories(char *currentDirectory)
{
    DIR *dir;
    struct dirent *entries;
    struct stat thestat;
    struct passwd *tf;
    struct group *gf;
    clearscreen();
    startIndex = 0;
    fileIndex = 0;
    files.clear();
    printfiles.clear();
    if ((dir = opendir(currentDirectory)) != NULL)
    {
        /* print all the files and directories within directory */
        char buf[1024];
        while ((entries = readdir(dir)) != NULL)
        {
            string file = "";
            sprintf(buf, "%s/%s", currentDirectory, entries->d_name);
            stat(buf, &thestat);
            switch (thestat.st_mode & S_IFMT)
            {
            case S_IFBLK:
                file += "b";
                break;
            case S_IFCHR:
                file += "c";
                break;
            case S_IFDIR:
                file += "d";
                break;
            case S_IFIFO:
                file += "p";
                break;
            case S_IFLNK:
                file += "l";
                break;
            case S_IFSOCK:
                file += "s";
                break;
            default:
                file += "-";
                break;
            }
            file += ((thestat.st_mode & S_IRUSR) ? "r" : "-");
            file += ((thestat.st_mode & S_IWUSR) ? "w" : "-");
            file += ((thestat.st_mode & S_IXUSR) ? "x" : "-");
            file += ((thestat.st_mode & S_IRGRP) ? "r" : "-");
            file += ((thestat.st_mode & S_IWGRP) ? "w" : "-");
            file += ((thestat.st_mode & S_IXGRP) ? "x" : "-");
            file += ((thestat.st_mode & S_IROTH) ? "r" : "-");
            file += ((thestat.st_mode & S_IWOTH) ? "w" : "-");
            file += ((thestat.st_mode & S_IXOTH) ? "x" : "-");
            char temp[1024];
            sprintf(temp, "%2d", (int)thestat.st_nlink);
            file += temp;
            tf = getpwuid(thestat.st_uid);
            sprintf(temp, "%8s", tf->pw_name);
            file += temp;
            gf = getgrgid(thestat.st_gid);
            sprintf(temp, "%8s", gf->gr_name);
            file += temp;
            sprintf(temp, "%8s\t", convertSize(thestat.st_size).c_str());
            file += temp;
            sprintf(temp, "%10s\t", entries->d_name);
            file += temp;
            char buffer[26];
            time(&thestat.st_mtime);
            struct tm *tm_info;
            tm_info = localtime(&thestat.st_mtime);
            strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
            file += buffer;
            S s;
            strcpy(s.file, entries->d_name);
            char filename[1024];
            sprintf(filename, "%s/%s", currentDirectory, s.file);
            strcpy(s.file, filename);
            files.push_back(s);
            printfiles.push_back(file);
        }
        numberOfFiles = files.size();
        endIndex = w.ws_row - 3;
        display(startIndex, endIndex);
        setcursor(0, 0);
        closedir(dir);
        movecursor();
    }
    else
    {
        perror("");
        exit(1);
    }
}

void display(int start, int end)
{
    clearscreen();
    if (startIndex < 0)
        startIndex = 0;
    if (endIndex > (numberOfFiles - 1))
        endIndex = numberOfFiles - 1;
    for (int i = startIndex; i <= endIndex; i++)
        printf("%s\n", printfiles[i].c_str());
}

string convertToString(double num)
{
    ostringstream convert;
    convert << num;
    return convert.str();
}

double roundOff(double n)
{
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string convertSize(size_t size)
{
    // printf("inside convertSize") ;
    static const char *SIZES[] = {"", "K", "M", "G"};
    int div = 0;
    size_t rem = 0;

    while (size >= 1024 && (unsigned)div < (sizeof SIZES / sizeof *SIZES))
    {
        rem = (size % 1024);
        div++;
        size /= 1024;
    }

    double size_d = (float)size + (float)rem / 1024.0;

    string result = convertToString(roundOff(size_d)) + "" + SIZES[div];
    // printf("%s",result.c_str()) ;
    return result.c_str();
}

void movecursor()
{
    int c;
    bool flag=true ;
    fileIndex=0;
    setcursor(0,0);
    while (flag)
    {
        c = kbget();
        cursorbackward(10);
        if (fileIndex < 0)
            fileIndex = 0;
        if (fileIndex >= numberOfFiles)
            fileIndex = numberOfFiles - 1;

        // printf("%d", fileIndex);
        switch (c)
        {
        case KEY_ENTER:
            write(STDOUT_FILENO, "\x1b[2J", 4);
            char buf[1024];
            struct stat thestat;
            sprintf(buf, "%s", files[fileIndex].file);
            stat(buf, &thestat);
            if ((thestat.st_mode & S_IFMT) == S_IFDIR)
            {
                clearscreen();
                S current;
                strcpy(current.file, currentDirectory);
                rollbackward.push(current);
                strcpy(currentDirectory, realpath(buf, NULL));
                listFilesAndDirectories(currentDirectory);
            }
            else
            {
                pid_t pid = fork();
                setcursor(0, 0);
                strcpy(currentDirectory, realpath(buf, NULL));

                if (pid == 0)
                {
                    close(2);
                    execlp("xdg-open", "xdg-open", currentDirectory, NULL);
                    exit(0);
                    setcursor(0, 0);
                }
                else
                {
                    display(startIndex, endIndex);
                    setcursor(fileIndex - startIndex + 1, 0);
                }
            }
            break;
        case KEY_ESCAPE:
            break;
        case KEY_UP:
            if (fileIndex >= 0)
            {
                fileIndex--;

                if (fileIndex < startIndex && startIndex != 0)
                {
                    startIndex--;
                    endIndex--;
                    display(startIndex, endIndex);
                    setcursor(0, 0);
                }
                else
                    cursorup(1);
            }
            break;
        case KEY_DOWN:
            if (fileIndex < (numberOfFiles))
            {
                fileIndex++;

                if (fileIndex > (endIndex) && fileIndex < (numberOfFiles))
                {
                    printf("if");
                    startIndex++;
                    endIndex++;
                    display(startIndex, endIndex);
                    cursorup(1);
                }
                else if (fileIndex < numberOfFiles)
                    cursordown(1);
            }
            break;
        case KEY_RIGHT:
            S next;
            if (!rollforward.empty())
            {

                next = rollforward.top();
                rollforward.pop();
                S prev;
                strcpy(prev.file, currentDirectory);
                strcpy(currentDirectory, next.file);
                rollbackward.push(prev);
                listFilesAndDirectories(currentDirectory);
                printf("inside if");
            }
            break;
        case KEY_LEFT:
            S prev;
            if (!rollbackward.empty())
            {

                prev = rollbackward.top();
                rollbackward.pop();
                printf("file=%s", prev.file);
                S next;
                strcpy(next.file, currentDirectory);
                strcpy(currentDirectory, prev.file);
                rollforward.push(next);
                listFilesAndDirectories(currentDirectory);
                printf("inside if 2");
            }

            break;
        default:
            // if (c == ':')
            setcursor(w.ws_row-1,0);

            putchar(c);
            cursorforward(1) ;
            if (c == ':')
            {
                // printf("inside");
                setcursor(w.ws_row-1, 0);

                commandMode();
                // break ;
                flag=false ;
                printf("after");
            }
            fflush(stdin);

            break;
        }
    }
}
