#include "global.h"

char currentDirectory[1024];
struct termios initialrsettings, newrsettings;
FILE *input;
FILE *output;
struct winsize w;
vector<S> files;
int topleft ;
int fileIndex=0 ;
int numberOfFiles=0 ;
stack<S> rollbackward;
stack<S> rollforward;
vector<string> printfiles;
int startIndex=0 ;
int endIndex ;
unordered_map<string,bool> visited ; 


void intializeTerminal();
void reset();
int getch();
int kbhit();
int kbesc();
int kbget();
void getWindowSize();

void initializeAll()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    getcwd(currentDirectory, sizeof(currentDirectory));
    files.clear();
    printfiles.clear();
}
void getWindowSize()
{
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
}

int getch()
{
    int c = 0;

    tcgetattr(0, &initialrsettings);
    memcpy(&newrsettings, &initialrsettings, sizeof(newrsettings));
    newrsettings.c_lflag &= ~(ICANON | ECHO);
    newrsettings.c_cc[VMIN] = 1;
    newrsettings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newrsettings);
    c = getchar();
    tcsetattr(0, TCSANOW, &initialrsettings);
    return c;
}
int kbhit()
{
    int c = 0;

    tcgetattr(0, &initialrsettings);
    memcpy(&newrsettings, &initialrsettings, sizeof(newrsettings));
    newrsettings.c_lflag &= ~(ICANON | ECHO);
    newrsettings.c_cc[VMIN] = 1;
    newrsettings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &newrsettings);
    c = getchar();
    tcsetattr(0, TCSANOW, &initialrsettings);
    if (c != -1)
        ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}

int kbesc(void)
{
    int c;

    if (!kbhit())
        return KEY_ESCAPE;
    c = getch();
    if (c == '[')
    {
        switch (getch())
        {
        case 'A':
            c = KEY_UP;
            break;
        case 'B':
            c = KEY_DOWN;
            break;
        case 'C':
            c = KEY_RIGHT;
            break;
        case 'D':
            c = KEY_LEFT;
            break;
        default:
            c = 0;
            break;
        }
    }
    else
    {
        c = 0;
    }
    if (c == 0)
        while (kbhit())
            getch();
    return c;
}

int kbget(void)
{
    int c;

    c = getch();
    return (c == KEY_ESCAPE) ? kbesc() : c;
}

