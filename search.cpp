#include "global.h"
char *directory = currentDirectory;

void search(const char *filename)
{
    printf("inside search");
    startIndex = 0;
    endIndex = w.ws_row - 3;
    fileIndex = 0;
    files.clear();
    printfiles.clear();
    clearscreen();
    searchFile(directory, filename);
    cout << directory << "..";
    numberOfFiles = files.size();
    cout << numberOfFiles;
    S prev;
    strcpy(prev.file, currentDirectory);
    cout << currentDirectory;
    rollbackward.push(prev);
    display(startIndex, endIndex);
    setcursor(0, 0);
    movecursor();
}

void searchFile(char *directory, const char *filename)
{
    printf("searching inside %s\n", directory);
    DIR *dir;
    struct dirent *entries;
    struct stat thestat;
    struct passwd *tf;
    struct group *gf;
    if (!visited[directory])
    {
        if ((dir = opendir(directory)) != NULL)
        {
            char buf[1024];
            while ((entries = readdir(dir)) != NULL)
            {
                if (strcmp(entries->d_name, ".") == 0 || strcmp(entries->d_name, "..") == 0)
                    continue;
                visited[directory] = true;
                sprintf(buf, "%s/%s", directory, entries->d_name);
                stat(buf, &thestat);
                if ((thestat.st_mode & S_IFMT) == S_IFDIR)
                {
                    if (!visited[buf])
                        searchFile(realpath(buf, NULL), filename);
                }
                else if (strcmp(entries->d_name, filename) == 0)
                {
                    string file = "";
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
                    S s;
                    strcpy(s.file, entries->d_name);
                    char filename[1024];
                    sprintf(filename, "%s/%s", directory, s.file);
                    // cout<<filename ;
                    strcpy(s.file, realpath(filename, NULL));
                    // cout << "####" << s.file << endl;
                    sprintf(temp, "%10s\t", filename);
                    file += temp;
                    char buffer[26];
                    time(&thestat.st_mtime);
                    struct tm *tm_info;
                    tm_info = localtime(&thestat.st_mtime);
                    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
                    file += buffer;
                    files.push_back(s);
                    printfiles.push_back(file);
                }
            }
            closedir(dir);
        }
        else
        {
            perror("");
            exit(1);
        }
    }
}