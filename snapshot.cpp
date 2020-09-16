#include "global.h"
unordered_map<string, bool> visit;
vector<S> directories;

void findDirectories(char *directory)
{
    DIR *dir;
    struct dirent *entries;
    struct stat thestat;
    if (!visit[directory])
    {
        if ((dir = opendir(directory)) != NULL)
        {
            char buf[1024];
            visit[directory] = true;
            while ((entries = readdir(dir)) != NULL)
            {
                if (strcmp(entries->d_name, "..") == 0 || (entries->d_name)[0] == '.')
                    continue;
                sprintf(buf, "%s/%s", directory, entries->d_name);
                stat(buf, &thestat);
                if ((thestat.st_mode & S_IFMT) == S_IFDIR)
                {
                    S s;
                    strcpy(s.file, buf);
                    directories.push_back(s);
                    if (!visit[buf])
                        findDirectories(buf);
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

vector<S> findFiles(char *directory)
{
    vector<S> child;
    DIR *dir;
    struct dirent *entries;
    if ((dir = opendir(directory)) != NULL)
    {
        while ((entries = readdir(dir)) != NULL)
        {
            if (strcmp(entries->d_name, "..") == 0)
            continue ;
            // printf("%s\t", entries->d_name);
            // sprintf(buf, "%s/%s", directory, entries->d_name);
            // stat(buf, &thestat);
            S s;
            strcpy(s.file, entries->d_name);
            child.push_back(s);
        }
        closedir(dir);
    }
    else
    {
        perror("");
        exit(1);
    }
    return child;
}

int mycmp(S s1, S s2)
{
    return (strcmp(s1.file, s2.file) > 0) ? 0 : 1;
}

void createSnapshot(char *dir, char *filename)
{
    directories.clear();
    S cr;
    strcpy(cr.file, ".");
    directories.push_back(cr);
    findDirectories(dir);
    sort(directories.begin(), directories.end(), mycmp);
    createFile(filename, dir);
    cout<<"\t";
    int inputFd;
    char buf[1024];
    sprintf(buf, "%s/%s", dir, filename);

    inputFd = open(realpath(buf, NULL), O_WRONLY | O_CREAT);
    if (inputFd == -1)
    {
        printf("cannot open file %s\n", buf);
    }
    else
    {
        // char copybuf[1024];
        int sz = directories.size();
        string file = "\n";
        for (int i = 0; i < sz; i++)
        {
            file += directories[i].file;
            file += ":\n";
            vector<S> children;
            children = findFiles(directories[i].file);

            for (auto i = children.begin(); i != children.end(); ++i)
            {
                file += (*i).file;
                file += "\t";
            }
            file+="\n";
          
        }
        if (write(inputFd, file.c_str(), file.length()) <= 0)
        {
            printf("error coping file\n");
        }
    }
    close(inputFd);
    printf("snapshot saved");
}