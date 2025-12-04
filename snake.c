#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define screenHeight 800
#define screenWidth 800
#define SNAKE_LENGTH 256
#define SQUARE_SIZE 31

typedef struct
{
    Vector2 position, size, speed;
    Color color;
} Snake;
typedef struct
{
    Vector2 position, size;
    bool active;
    Color color;
} Food;
typedef struct
{
    Color bg, grid, head, body, fruit, text;
    const char *name;
} Theme;

static int framesCounter = 0, counterTail = 0, currentTheme = 0;
static bool gameOver = false, pause = false, allowMove = false;
static Food fruit = {0};
static Snake snake[SNAKE_LENGTH] = {0};
static Vector2 snakePosition[SNAKE_LENGTH] = {0}, offset = {0};

static Theme themes[] = {
    {{46, 52, 64, 255}, {67, 76, 94, 255}, {94, 129, 172, 255}, {129, 161, 193, 255}, {191, 97, 106, 255}, {216, 222, 233, 255}, "NORD"},
    {{29, 32, 33, 255}, {50, 48, 47, 255}, {254, 128, 25, 255}, {250, 189, 47, 255}, {251, 73, 52, 255}, {235, 219, 178, 255}, "GRUVBOX"},
    {{0, 0, 170, 255}, {0, 170, 170, 255}, {255, 255, 85, 255}, {85, 255, 255, 255}, {255, 85, 85, 255}, {255, 255, 255, 255}, "TEMPLEOS"}};

void InitGame(void)
{
    framesCounter = counterTail = 0;
    gameOver = pause = false;
    counterTail = 1;
    allowMove = false;
    offset.x = screenWidth % SQUARE_SIZE;
    offset.y = screenHeight % SQUARE_SIZE;
    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        snake[i].position = (Vector2){offset.x / 2, offset.y / 2};
        snake[i].size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
        snake[i].speed = (Vector2){SQUARE_SIZE, 0};
        snake[i].color = i == 0 ? themes[currentTheme].head : themes[currentTheme].body;
        snakePosition[i] = (Vector2){0.0f, 0.0f};
    }
    fruit.size = (Vector2){SQUARE_SIZE, SQUARE_SIZE};
    fruit.color = themes[currentTheme].fruit;
    fruit.active = false;
}

void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed(KEY_SPACE))
            pause = !pause;
        if (IsKeyPressed(KEY_TAB))
        {
            currentTheme = (currentTheme + 1) % 3;
            InitGame();
        }
        if (!pause)
        {
            if (IsKeyPressed(KEY_RIGHT) && snake[0].speed.x == 0 && allowMove)
            {
                snake[0].speed = (Vector2){SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_LEFT) && snake[0].speed.x == 0 && allowMove)
            {
                snake[0].speed = (Vector2){-SQUARE_SIZE, 0};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_UP) && snake[0].speed.y == 0 && allowMove)
            {
                snake[0].speed = (Vector2){0, -SQUARE_SIZE};
                allowMove = false;
            }
            if (IsKeyPressed(KEY_DOWN) && snake[0].speed.y == 0 && allowMove)
            {
                snake[0].speed = (Vector2){0, SQUARE_SIZE};
                allowMove = false;
            }

            for (int i = 0; i < counterTail; i++)
                snakePosition[i] = snake[i].position;
            if ((framesCounter % 5) == 0)
            {
                for (int i = 0; i < counterTail; i++)
                {
                    if (i == 0)
                    {
                        snake[0].position.x += snake[0].speed.x;
                        snake[0].position.y += snake[0].speed.y;
                        allowMove = true;
                    }
                    else
                        snake[i].position = snakePosition[i - 1];
                }
            }
            if (snake[0].position.x >= screenWidth - offset.x || snake[0].position.y >= screenHeight - offset.y ||
                snake[0].position.x < 0 || snake[0].position.y < 0)
                gameOver = true;
            for (int i = 1; i < counterTail; i++)
                if (snake[0].position.x == snake[i].position.x && snake[0].position.y == snake[i].position.y)
                    gameOver = true;

            if (!fruit.active)
            {
                fruit.active = true;
                fruit.position = (Vector2){GetRandomValue(0, screenWidth / SQUARE_SIZE - 1) * SQUARE_SIZE + offset.x / 2,
                                           GetRandomValue(0, screenHeight / SQUARE_SIZE - 1) * SQUARE_SIZE + offset.y / 2};
                for (int i = 0; i < counterTail; i++)
                {
                    while (fruit.position.x == snake[i].position.x && fruit.position.y == snake[i].position.y)
                    {
                        fruit.position = (Vector2){GetRandomValue(0, screenWidth / SQUARE_SIZE - 1) * SQUARE_SIZE + offset.x / 2,
                                                   GetRandomValue(0, screenHeight / SQUARE_SIZE - 1) * SQUARE_SIZE + offset.y / 2};
                        i = 0;
                    }
                }
            }
            if (snake[0].position.x < fruit.position.x + fruit.size.x && snake[0].position.x + snake[0].size.x > fruit.position.x &&
                snake[0].position.y < fruit.position.y + fruit.size.y && snake[0].position.y + snake[0].size.y > fruit.position.y)
            {
                snake[counterTail].position = snakePosition[counterTail - 1];
                counterTail++;
                fruit.active = false;
            }
            framesCounter++;
        }
    }
    else if (IsKeyPressed(KEY_ENTER))
    {
        InitGame();
        gameOver = false;
    }
    if (IsKeyPressed('T'))
    {
        currentTheme = (currentTheme + 1) % 3;
        InitGame();
    }
}

void DrawGame(void)
{
    Theme t = themes[currentTheme];
    BeginDrawing();
    ClearBackground(t.bg);
    if (!gameOver)
    {
        for (int i = 0; i < screenWidth / SQUARE_SIZE + 1; i++)
            DrawLineV((Vector2){SQUARE_SIZE * i + offset.x / 2, offset.y / 2}, (Vector2){SQUARE_SIZE * i + offset.x / 2, screenHeight - offset.y / 2}, t.grid);
        for (int i = 0; i < screenHeight / SQUARE_SIZE + 1; i++)
            DrawLineV((Vector2){offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, (Vector2){screenWidth - offset.x / 2, SQUARE_SIZE * i + offset.y / 2}, t.grid);
        for (int i = 0; i < counterTail; i++)
            DrawRectangleV(snake[i].position, snake[i].size, snake[i].color);
        DrawRectangleV(fruit.position, fruit.size, fruit.color);
        DrawText(TextFormat("%s (T) | Move (U D L R) | Pause (SPACE) ", t.name), 10, 10, 18, t.text);
        if (pause)
            DrawText("PAUSED [SPACE]", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, t.text);
    }
    else
    {
        DrawText("PRESS [ENTER]", screenWidth / 2 - 100, screenHeight / 2 - 20, 20, t.text);
        DrawText(TextFormat("%s (T) | Move (U D L R) | Pause (SPACE) ", t.name), 10, 10, 18, t.text);
    }
    EndDrawing();
}

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Snake");
    InitGame();
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateGame();
        DrawGame();
    }
    CloseWindow();
    return 0;
}
