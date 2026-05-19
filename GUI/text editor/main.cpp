#include "raylib.h"

#include <cstring>
#include <fstream>

using namespace std;

#define MAX_TEXT 100000

//====================================================
// FILE I/O
//====================================================

void SaveFile(const char* filename, const char* text)
{
    ofstream file(filename);
    if (file.is_open()) file << text;
}

void LoadFile(const char* filename, char* text, int maxSize)
{
    ifstream file(filename);
    if (!file.is_open()) return;

    file.read(text, maxSize - 1);
    text[file.gcount()] = '\0';
}

string OpenFileDialog()
{
    FILE* pipe = popen(
        "zenity --file-selection "
        "--file-filter='All files | *' "
        "--file-filter='Code files | *.cpp' "
        "--file-filter='Text files | *.txt'",
        "r"
    );

    if (!pipe) return "";

    char path[1024];
    string result;

    if (fgets(path, sizeof(path), pipe))
    {
        result = path;

        if (!result.empty() && result.back() == '\n')
            result.pop_back();
    }

    pclose(pipe);
    return result;
}

string SaveFileDialog()
{
    FILE* pipe = popen(
        "zenity --file-selection --save --confirm-overwrite "
        "--filename=file.txt "
        "--file-filter='Text files | *.txt'",
        "r"
    );

    if (!pipe) return "";

    char path[1024];
    string result;

    if (fgets(path, sizeof(path), pipe))
    {
        result = path;

        if (!result.empty() && result.back() == '\n')
            result.pop_back();
    }

    pclose(pipe);

    return result;
}

//====================================================
// ENTRY
//====================================================

int main() {
    const int screenWidth  = 1000;
    const int screenHeight = 600;
    const int linewidth=80;

    const float REPEAT_DELAY = 0.05f;



    InitWindow(screenWidth, screenHeight, "custom text editor");
    SetTargetFPS(60);

    char text[MAX_TEXT] = "Type here!";
    int cursorIndex = strlen(text);

    float scrollY = 0.0f;

    while (!WindowShouldClose())
    {
        //================================================
        // INPUT
        //================================================

        int len = strlen(text);

        float dt = GetFrameTime();

        int key = GetCharPressed();

        while (key > 0)
        {
            int len = strlen(text);

            int lineStart = cursorIndex;

            for (int i = cursorIndex - 1; i >= 0; i--)
            {
                if (text[i] == '\n') break;
                lineStart = i;
            }

            int currentLineLen = 0;
            for (int i = lineStart; i < len; i++)
            {
                if (text[i] == '\n') break;
                currentLineLen++;
            }

            if (currentLineLen >= linewidth && key != '\n')
            {
                key = GetCharPressed();
                continue;
            }

            if (len < MAX_TEXT - 1)
            {
                for (int i = len; i >= cursorIndex; i--)
                {
                    text[i + 1] = text[i];
                }

                text[cursorIndex] = (char)key;
                cursorIndex++;

                text[len + 1] = '\0';
            }

            key = GetCharPressed();
        }

        //================================================
        // BACKSPACE
        //================================================

        static float backspaceTimer = 0.0f;
        static bool backspaceFirst = true;

        if (IsKeyDown(KEY_BACKSPACE))
        {
            backspaceTimer += GetFrameTime();

            float delay = backspaceFirst ? 0.0f : REPEAT_DELAY;

            if (backspaceTimer >= delay)
            {
                int len = strlen(text);

                if (cursorIndex > 0)
                {
                    for (int i = cursorIndex - 1; i < len; i++)
                        text[i] = text[i + 1];

                    cursorIndex--;
                }

                backspaceTimer = 0.0f;
                backspaceFirst = false;
            }
        }
        else
        {
            backspaceTimer = 0.0f;
            backspaceFirst = true;
        }

        //================================================
        // ENTER
        //================================================

        static float enterTimer = 0.0f;
        static bool enterFirst = true;

        if (IsKeyDown(KEY_ENTER))
        {
            enterTimer += GetFrameTime();

            float delay = enterFirst ? 0.0f : REPEAT_DELAY;

            if (enterTimer >= delay)
            {
                int len = strlen(text);

                if (len < MAX_TEXT - 1 && cursorIndex <= len)
                {
                    for (int i = len; i >= cursorIndex; i--)
                        text[i + 1] = text[i];

                    text[cursorIndex] = '\n';
                    cursorIndex++;

                    text[len + 1] = '\0';
                }

                enterTimer = 0.0f;
                enterFirst = false;
            }
        }
        else
        {
            enterTimer = 0.0f;
            enterFirst = true;
        }

        //================================================
        // ARROWS
        //================================================

        static float arrowTimer = 0.0f;
        static bool arrowFirst = true;

        bool left = IsKeyDown(KEY_LEFT);
        bool right = IsKeyDown(KEY_RIGHT);

        if (left || right)
        {
            arrowTimer += GetFrameTime();

            float delay = arrowFirst ? 0.0f : REPEAT_DELAY;

            if (arrowTimer >= delay)
            {
                if (left && cursorIndex > 0) cursorIndex--;
                if (right && cursorIndex < (int)strlen(text)) cursorIndex++;

                arrowTimer = 0.0f;
                arrowFirst = false;
            }
        }
        else
        {
            arrowTimer = 0.0f;
            arrowFirst = true;
        }

        //================================================
        // UP / DOWN
        //================================================

        static float vertTimer = 0.0f;
        static bool vertFirst = true;

        bool up = IsKeyDown(KEY_UP);
        bool down = IsKeyDown(KEY_DOWN);

        if (up || down)
        {
            vertTimer += GetFrameTime();

            float delay = vertFirst ? 0.0f : REPEAT_DELAY;

            if (vertTimer >= delay)
            {
                int lineStart = 0;
                int lineIndex = 0;

                for (int i = 0; i < cursorIndex; i++)
                {
                    if (text[i] == '\n')
                    {
                        lineStart = i + 1;
                        lineIndex++;
                    }
                }

                int col = cursorIndex - lineStart;

                if (up)
                {
                    if (lineIndex > 0)
                    {
                        int prevLineStart = 0;
                        int currentLine = 0;

                        for (int i = 0; i < cursorIndex; i++)
                        {
                            if (text[i] == '\n')
                            {
                                if (currentLine == lineIndex - 1)
                                    break;

                                prevLineStart = i + 1;
                                currentLine++;
                            }
                        }

                        int target = prevLineStart + col;

                        int end = prevLineStart;
                        while (text[end] != '\0' && text[end] != '\n') end++;

                        if (target > end) target = end;

                        cursorIndex = target;
                    }
                }

                if (down)
                {
                    int currentLine = 0;
                    int nextLineStart = -1;

                    for (int i = 0; text[i] != '\0'; i++)
                    {
                        if (text[i] == '\n')
                        {
                            if (currentLine == lineIndex)
                                nextLineStart = i + 1;

                            currentLine++;
                        }
                    }

                    if (nextLineStart != -1)
                    {
                        int target = nextLineStart + col;

                        int end = nextLineStart;
                        while (text[end] != '\0' && text[end] != '\n') end++;

                        if (target > end) target = end;

                        cursorIndex = target;
                    }
                }

                vertTimer = 0.0f;
                vertFirst = false;
            }
        }
        else
        {
            vertTimer = 0.0f;
            vertFirst = true;
        }

        //================================================
        // SCROLL
        //================================================

        scrollY -= GetMouseWheelMove() * 50.0f;
        if (scrollY < 0) scrollY = 0;

        //================================================
        // FILE OPS
        //================================================

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S))
        {
            string filename = SaveFileDialog();
            if (!filename.empty()) SaveFile(filename.c_str(), text);
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_O))
        {
            string filename = OpenFileDialog();
            if (!filename.empty())
            {
                LoadFile(filename.c_str(), text, MAX_TEXT);
                cursorIndex = strlen(text);
            }
        }

        //================================================
        // LINE / COL
        //================================================

        int line = 0;
        int col = 0;

        for (int i = 0; i < cursorIndex; i++)
        {
            if (text[i] == '\n')
            {
                line++;
                col = 0;
            }
            else col++;
        }

        //================================================
        // DRAW
        //================================================

        BeginDrawing();
        ClearBackground(WHITE);

        DrawRectangle(10, 25, 940, 492, WHITE);

        DrawTextEx(
            GetFontDefault(),
            text,
            (Vector2){15, 30 - scrollY},
            20,
            1,
            BLACK
        );

        int barHeight = 25;
        DrawRectangle(0, screenHeight - barHeight, screenWidth, barHeight, GRAY);
        DrawLine(0, screenHeight - barHeight, screenWidth, screenHeight - barHeight, GRAY);

        char status[64];
        sprintf(status, "Ln %d, Col %d", line + 1, col);

        DrawTextEx(
            GetFontDefault(),
            status,
            (Vector2){10, screenHeight - barHeight + 5},
            18,
            1,
            BLACK
        );

        EndDrawing();
    }

    CloseWindow();
}