#ifdef WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#endif

#include <iostream>

using namespace std;

namespace Screen
{
    int row = 0, col = 0;

    void Init()
    {
        CheckScreen();
    };

    void Render()
    {
        for(int i = 0; i < row; i++)
        {
            string line = "";
            for(int j = 0; j < col; j++)
            {
                line+=screen[i][j];
            }
            cout << line;
        }
    };

    namespace
    {
        char** screen = nullptr;

        bool ValidateSize(int rows = 0, int columns = 0)
        {
            bool didChange = false;
            int tmp_row, tmp_col;
            #ifdef WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            tmp_col = csbi.srWindow.Right - csbi.srWindow.Left + 1;
            tmp_row = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
            #else
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            tmp_row = w.ws_row;
            tmp_col = w.ws_col;
            #endif

            if(tmp_col != col)
            {
                col = tmp_col;
                didChange = true;
            }

            if(tmp_row != row)
            {
                row = tmp_row;
                didChange = true;
            }

            return didChange;
        };

        void Resize()
        {
            if(screen != nullptr)
            {
                for(int i = 0; i < row; i++)
                {
                    delete[] screen[i];
                }
                delete[] screen;
                screen = nullptr;
            }

            screen = new char*[row];
            for(int i = 0; i < row; i++)
            {
                screen[i] = new char[col];
            }
        };

        void CheckScreen(int rows = 0, int columns = 0)
        {
            if(ValidateSize(rows, columns))
            {
                Resize();
            }
        }
    }
}