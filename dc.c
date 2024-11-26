#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 450
#define GROUND_HEIGHT 50
#define DINO_WIDTH 50
#define DINO_HEIGHT 50
#define OBSTACLE_WIDTH 30
#define OBSTACLE_HEIGHT 50

typedef struct {
    int x;
    int y;
    int width;
    int height;
    bool isJumping;
    int jumpSpeed;
    int groundLevel;
} Dino;

typedef struct {
    int x;
    int width;
    int height;
    int speed;
} Obstacle;

int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "DC");
    SetTargetFPS(60);

    // Dino initial setup
    Dino dino = {
        .x = 100,
        .y = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT,
        .width = DINO_WIDTH,
        .height = DINO_HEIGHT,
        .isJumping = false,
        .jumpSpeed = 0,
        .groundLevel = WINDOW_HEIGHT - GROUND_HEIGHT - DINO_HEIGHT
    };

    // Obstacle initial setup
    Obstacle obstacle = {
        .x = WINDOW_WIDTH,
        .width = OBSTACLE_WIDTH,
        .height = OBSTACLE_HEIGHT,
        .speed = 5
    };

    int score = 0;
    bool gameOver = false;

    while (!WindowShouldClose()) {
        // Jump controls (Space for PC, Touch for Mobile)
        if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || GetTouchPointCount() > 0) && !dino.isJumping) {
            dino.isJumping = true;
            dino.jumpSpeed = 15;
        }

        // Jumping mechanics
        if (dino.isJumping) {
            dino.y -= dino.jumpSpeed;
            dino.jumpSpeed -= 1;

            if (dino.y >= dino.groundLevel) {
                dino.y = dino.groundLevel;
                dino.isJumping = false;
                dino.jumpSpeed = 0;
            }
        }

        // Obstacle movement
        if (!gameOver) {
            obstacle.x -= obstacle.speed;

            // Reset obstacle when it goes off-screen
            if (obstacle.x < -obstacle.width) {
                obstacle.x = WINDOW_WIDTH;
                score++;
            }

            // Collision detection
            if (obstacle.x < dino.x + dino.width &&
                obstacle.x + obstacle.width > dino.x &&
                dino.y + dino.height > WINDOW_HEIGHT - GROUND_HEIGHT - obstacle.height) {
                gameOver = true;
            }
        }

        // Rendering
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw ground
        DrawRectangle(0, WINDOW_HEIGHT - GROUND_HEIGHT, WINDOW_WIDTH, GROUND_HEIGHT, GRAY);

        // Draw dino
        DrawRectangle(dino.x, dino.y, dino.width, dino.height, GREEN);

        // Draw obstacle
        DrawRectangle(obstacle.x, WINDOW_HEIGHT - GROUND_HEIGHT - obstacle.height, 
                      obstacle.width, obstacle.height, RED);

        // Score and game over display
        if (!gameOver) {
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);
        } else {
            DrawText("GAME OVER", WINDOW_WIDTH/2 - MeasureText("GAME OVER", 40)/2, 
                     WINDOW_HEIGHT/2 - 20, 40, RED);
            DrawText(TextFormat("Final Score: %d", score), 
                     WINDOW_WIDTH/2 - MeasureText(TextFormat("Final Score: %d", score), 20)/2, 
                     WINDOW_HEIGHT/2 + 30, 20, BLACK);
            
            // Restart option
            if (IsKeyPressed(KEY_R) || GetTouchPointCount() > 0) {
                // Reset game state
                dino.y = dino.groundLevel;
                obstacle.x = WINDOW_WIDTH;
                score = 0;
                gameOver = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
