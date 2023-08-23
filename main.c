#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <raylib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CELL_SIZE_PX 10

#define FPS 60
#define INTERVAL 10

//assert((SCREEN_WIDTH)%(CELL_SIZE_PX)==0) && "ERROR: Invalid screen width");
//assert((SCREEN_HEIGHT)%(CELL_SIZE_PX)==0) && "ERROR: Invalid screen height");

#define GRID_WIDTH (SCREEN_WIDTH/CELL_SIZE_PX)
#define GRID_HEIGHT (SCREEN_HEIGHT/CELL_SIZE_PX)

int board[GRID_HEIGHT][GRID_WIDTH] = {0};
int temp[GRID_HEIGHT][GRID_WIDTH] = {0};

void board_random(void)
{
    for (size_t y = 0; y < GRID_HEIGHT; ++y) {
        for (size_t x = 0; x < GRID_WIDTH; ++x) {
            if (rand()%2 == 0) board[y][x] = 1;
        }
    }
}


#define EMOD(x, a) (((x%a)+a)%a)

int count_neighbors(int x, int y)
{
    int n = 0;
    for (int j = -1; j <= 1; ++j) {
        for (int i = -1; i <= 1; ++i) {
            if (board[EMOD(y+j,GRID_HEIGHT)][EMOD(x+i,GRID_WIDTH)] && !(i==0&&j==0)) n+= 1;
        }
    }
    return n;
}

void board_next_gen(void)
{

    for (size_t y = 0; y < GRID_HEIGHT; ++y) {
        for (size_t x = 0; x < GRID_WIDTH; ++x) {
            int n = count_neighbors(x,y);
            if (board[y][x]==0) temp[y][x] = n == 3;
            else temp[y][x] = (n == 2 || n == 3);
        }
    }
    for (size_t y = 0; y < GRID_HEIGHT; ++y) {
        for (size_t x = 0; x < GRID_WIDTH; ++x) {
            board[y][x] = temp[y][x];
        }
    }
}


void board_draw(void)
{
    // DrawRectangle(int posX, int posY, int width, int height, Color color);
    for (size_t y = 0; y < GRID_HEIGHT; ++y) {
        for (size_t x = 0; x < GRID_WIDTH; ++x) {
            Rectangle cell = {x*CELL_SIZE_PX, y*CELL_SIZE_PX, CELL_SIZE_PX, CELL_SIZE_PX};
            DrawRectangleRec(cell, board[y][x] ? RAYWHITE : DARKPURPLE);
            DrawRectangleLinesEx(cell, 1, board[y][x] ? PURPLE : VIOLET);
            if (IsKeyDown(KEY_T)) DrawText(TextFormat("%i",count_neighbors(x,y)),x*CELL_SIZE_PX,y*CELL_SIZE_PX,CELL_SIZE_PX, BLACK);
        }
    }
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "game of life");
    SetTargetFPS(FPS);

    Vector2 mouse_pos;

    board_random();
    // for (size_t y = 0; y < GRID_HEIGHT; ++y) {
    //     for (size_t x = 0; x < GRID_WIDTH; ++x) {
    //         printf("%s", board[y][x] ? "#" : " ");
    //     }
    //     printf("\n");
    // }

    int paused = 1;
    int interval = 0;

    while (!WindowShouldClose()) {
        // update
        // ----------

        mouse_pos = GetMousePosition();
        int hovered_grid_x = (int)mouse_pos.x/CELL_SIZE_PX;
        int hovered_grid_y = (int)mouse_pos.y/CELL_SIZE_PX;
        Rectangle hover_rect = {hovered_grid_x*CELL_SIZE_PX, hovered_grid_y*CELL_SIZE_PX, CELL_SIZE_PX, CELL_SIZE_PX};

        if (IsKeyPressed(KEY_SPACE)) paused = 1-paused;

        if (!paused) {
            interval++;
            interval += IsKeyDown(KEY_F);

            if (interval >= INTERVAL) {
                interval = 0;
                board_next_gen();
            }
        } else {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            board[hovered_grid_y][hovered_grid_x] = 1 - board[hovered_grid_y][hovered_grid_x];
        }

        // draw
        // ----------
        BeginDrawing();
            ClearBackground(DARKPURPLE);
            board_draw();
            // DrawCircleV(mouse_pos, 10, RED);

            if (paused) {
                DrawText("| | paused",5,5,30,BLACK);
                DrawRectangleLinesEx(hover_rect, 3, BLUE);
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
