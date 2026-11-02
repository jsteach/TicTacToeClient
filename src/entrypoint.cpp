#include "entrypoint.h"
#include "raylib.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 600;
constexpr int BOARD_SIZE = 3;
constexpr int CELL_SIZE = 160;
constexpr int BOARD_X = (WINDOW_WIDTH - BOARD_SIZE * CELL_SIZE) / 2;
constexpr int BOARD_Y = (WINDOW_HEIGHT - BOARD_SIZE * CELL_SIZE) / 2;
std::string player = "X";// X or O depending on what the server decides 
std::string Trim(const std::string& text)
{
    auto first = std::find_if_not(text.begin(), text.end(),
        [](unsigned char c) { return std::isspace(c); });
    auto last = std::find_if_not(text.rbegin(), text.rend(),
        [](unsigned char c) { return std::isspace(c); }).base();

    if (first >= last) return {};
    return std::string(first, last);
}

std::vector<std::vector<char>> ParseBoard(const std::string& text)
{
    std::vector<std::vector<char>> board;
    std::istringstream lines(text);
    std::string line;

    while (std::getline(lines, line) && board.size() < BOARD_SIZE)
    {
        std::vector<char> row;
        std::stringstream cells(line);
        std::string cell;

        while (std::getline(cells, cell, ';') && row.size() < BOARD_SIZE)
        {
            cell = Trim(cell);
            row.push_back(cell.empty() ? ' ' : cell[0]);
        }

        while (row.size() < BOARD_SIZE)
            row.push_back(' ');

        board.push_back(row);
    }

    while (board.size() < BOARD_SIZE)
        board.push_back(std::vector<char>(BOARD_SIZE, ' '));

    return board;
}

void DrawX(int centerX, int centerY)
{
    constexpr int halfMark = 45;

    DrawLineEx(
        { float(centerX - halfMark), float(centerY - halfMark) },
        { float(centerX + halfMark), float(centerY + halfMark) },
        8, RED);

    DrawLineEx(
        { float(centerX + halfMark), float(centerY - halfMark) },
        { float(centerX - halfMark), float(centerY + halfMark) },
        8, RED);
}

void DrawO(int centerX, int centerY)
{
    constexpr float radius = 45;

    DrawCircleLines(centerX, centerY, radius, BLUE);
    DrawCircleLines(centerX, centerY, radius - 1, BLUE);
}

void DrawBoard(int bx , int by){
    for (int i = 1; i < BOARD_SIZE; ++i){
        int x = bx + i * CELL_SIZE;
        int y = by + i * CELL_SIZE;

        DrawLine(x, by, x, by + BOARD_SIZE * CELL_SIZE, DARKGRAY);
        DrawLine(bx, y, bx + BOARD_SIZE * CELL_SIZE, y, DARKGRAY);
    }
}
int raylib_start(void){

    const std::string input =
        "x;o; ;\n"
        " ;x; ;\n"
        " ; ;o;";

    const auto board = ParseBoard(input);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tic-Tac-Toe");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        std::string Player = "Playing as: " + player; 
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText(Player.c_str(),0,0,CELL_SIZE / BOARD_SIZE,RED);// Color should be the color for the shape, e.g. RED or BLUE

        DrawBoard(BOARD_X,BOARD_Y);

        for (int row = 0; row < BOARD_SIZE; ++row)
        {
            for (int col = 0; col < BOARD_SIZE; ++col)
            {
                int centerX = BOARD_X + col * CELL_SIZE + CELL_SIZE / 2;
                int centerY = BOARD_Y + row * CELL_SIZE + CELL_SIZE / 2;

                if (board[row][col] == 'x' || board[row][col] == 'X')
                    DrawX(centerX, centerY);
                else if (board[row][col] == 'o' || board[row][col] == 'O')
                    DrawO(centerX, centerY);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
