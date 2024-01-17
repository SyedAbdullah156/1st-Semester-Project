// main.c
#include "game.h"
#include <stdio.h>
#include <raylib.h>

int screenWidth = 1792 * SCREEN_FACTOR;
int screenHeight = 1024 * SCREEN_FACTOR;
char assess;
int score[5];

void writeScoresToFile() 
{
    FILE *File = fopen("scores.txt", "w");
    if (File == NULL) 
    {
        printf("Error opening txt file for writing.\n");
        return;
    }
    for(int i = 0; i < 5; i++)
        fprintf(File, "%d\n", score[i]);
    fclose(File);
}

void loadScoresToFile() 
{
    FILE *File = fopen("scores.txt", "r");
    if (File == NULL) 
    {
        printf("Error opening txt file for writing.\n");
        return;
    }

    for(int i = 0; !feof(File); i++)
        fscanf(File, "%d", &score[i]);
    fclose(File);
}

int main() 
{
    InitWindow(screenWidth, screenHeight, "Doraemon's Dreamy Quest");
    InitAudioDevice();

    loadScoresToFile();
    SetTargetFPS(60);
    GameScreen currentScreen = TITLE;

    while (!WindowShouldClose())
    {
        switch (currentScreen) 
        {
            case TITLE: 
            {
                titleScreen();
                currentScreen = GAMEPLAY;
            } break;

            case GAMEPLAY: 
            {
                // Updating scores int he reverse order
                for (int i = 4; i > 0; i--) 
                { 
                    score[i] = score[i-1]; 
                } 
                score[0] = gameplayScreen();
                currentScreen = ENDING;
                writeScoresToFile(); 
            } break;

            case ENDING: 
            {
                assess = endingScreen();
                if (assess == 'R') 
                {
                    currentScreen = GAMEPLAY;
                } 
                else if (assess == 'Q') 
                {
                    currentScreen = TITLE;
                }
            } break;

            default: break;
        }
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
