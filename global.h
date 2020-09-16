#include "myheader.h"
#ifndef Global_H
#define Global_H

#define setcursor(x, y) printf("\033[%d;%dH", x, y);
#define cursorup(x) printf("\033[%dA", (x))
#define cursordown(x) printf("\033[%dB", (x))
#define cursorforward(x) printf("\033[%dC", (x))
#define cursorbackward(x) printf("\033[%dD", (x))
#define clearscreen() printf("\033[H\033[J")

#define KEY_ESCAPE 0x001b
#define KEY_ENTER 0x000a
#define KEY_UP 0x0105
#define KEY_DOWN 0x0106
#define KEY_LEFT 0x0107
#define KEY_RIGHT 0x0108
struct S
{
    char file[256];
};
extern char currentDirectory[1024];
extern struct termios initialrsettings;
extern struct termios newrsettings;
extern FILE *input;
extern FILE *output;
extern vector<S> files;
extern struct winsize w;
extern int topleft;
extern int fileIndex;
extern int numberOfFiles;
extern stack<S> rollbackward;
extern stack<S> rollforward;
extern vector<string> printfiles;
extern int startIndex;
extern int endIndex;
extern unordered_map<string,bool> visited ; 

void initializeAll();
void reset();
int getch();
int kbhit();
int kbesc();
int kbget();
void getWindowSize();
void commandMode();
string convertToString(double num);
double roundOff(double num);
string convertSize(size_t size);
void display(int start, int end);
void movecursor();
void copyFiles(vector<string> source, string destination);
void moveFiles(vector<string> source, string destination);
void renameFiles(vector<string> source, string destination);
void createFile(string source, string destination);
void createDir(string source, string destination);
void deleteFilesAndDirectories(string target);
int mycompare(char *a, char *b);
void searchFile(char *directory,const char *filename);
void search(const char *filename);
void createSnapshot(char *dir,char *file);

#endif