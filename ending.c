#include "game.h"
#include <raylib.h>
#include <stdio.h>

char endingScreen(void) 
{
    Sound endingSound = LoadSound("audio/gameoversound.mp3");
    PlaySound(endingSound);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw ending text
        DrawText("Game Over", screenWidth / 2 - MeasureText("Game Over", 80) / 2 , 100, 80, RED);

        // Display final score
        char scoreText[20];
        sprintf(scoreText, "Your Score: %d", scoresPoint);
        DrawText(scoreText, screenWidth / 2 - MeasureText(scoreText, 60) / 2, screenHeight / 2, 60, DARKGRAY);

        // Draw any other relevant information or graphics for the ending screen

        // Draw a prompt to restart or exit
        DrawText("Press R to Restart", screenWidth / 2 - MeasureText("Press R to Restart", 60) / 2, screenHeight / 2 - 100, 60, DARKGRAY);
        DrawText("Press Q to Quit", screenWidth / 2 - MeasureText("Press Q to Quit", 60) / 2, screenHeight / 2 + 90, 60, DARKGRAY);

        // Check for user input
        if (IsKeyPressed(KEY_R))
        {
            EndDrawing();
            return 'R';
        }
        else if (IsKeyPressed(KEY_Q))
        {
            EndDrawing();
            return 'Q';
        }
        EndDrawing();
    }
}
