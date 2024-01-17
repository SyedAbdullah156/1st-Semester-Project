#include "gadgets.h"
#include "game.h"
#include <raylib.h>
#include <stdio.h>

float gravity = 5 * SCREEN_FACTOR;
int worldWidth = 1792 * 10;
int scoreIncrement = 25, scoreBarWidth = 200, scoreBarHeight = 20;
int scoresPoint = 0;
float gravity;
int worldWidth;
int scoreIncrement;
int scoreBarWidth;
int scoreBarHeight;

// Texture Loading function
void InitializeGameTextures(Texture2D characterTextures[], Texture2D *floorTexture, Texture2D platformTextures[], Texture2D *background, Sound stepSounds[], Texture2D *point, Texture2D *progressBar, Sound points[], Texture2D gadgets[], Sound gadgetsSound[], Texture2D *health, Texture2D *doraCake)
{
    // Load character textures
    characterTextures[0] = LoadTexture("picture/doraemonwalk1.png");
    characterTextures[1] = LoadTexture("picture/doraemonwalk2.png");
    characterTextures[2] = LoadTexture("picture/doraemonwalk3.png");
    characterTextures[3] = LoadTexture("picture/doraemonwalk4.png");
    characterTextures[4] = LoadTexture("picture/doraemonjumping1.png");

    // Load the gadgets textures
    gadgets[0] = LoadTexture("picture/gadget0.png");// Bambocopter
    gadgets[1] = LoadTexture("picture/gadget1.png");// Cannon
    gadgets[2] = LoadTexture("picture/gadget2.png");// Gate
    gadgets[3] = LoadTexture("picture/gadget0running.png");// Bambocopter running
    gadgets[4] = LoadTexture("picture/gadget2opening.png");// Gate opening texture

    // Loading the gadgets sound
    gadgetsSound[0] = LoadSound("audio/gadget1.mp3");// bambucoptor running sound 
    gadgetsSound[1] = LoadSound("audio/gadget2.mp3");// cannon sound
    gadgetsSound[2] = LoadSound("audio/gadget3.mp3");// anywhere door sound

    // Load floor texture
    *floorTexture = LoadTexture("picture/floor.png");

    // Loadinf the dora cake texture
    *doraCake = LoadTexture("picture/doraCake.png");

    // Load platform textures
    platformTextures[0] = LoadTexture("picture/platform1.png");
    platformTextures[1] = LoadTexture("picture/platform2.png");
    platformTextures[2] = LoadTexture("picture/platform3.png");

    // Load background texture
    *background = LoadTexture("picture/background.png");

    // Load point texture
    *point = LoadTexture("picture/point50.png");

    // Load progress Bar texture
    *progressBar = LoadTexture("picture/progressbar.png");

    // Load health texture
    *health = LoadTexture("picture/health.png");

    // Load sound effects
    stepSounds[0] = LoadSound("audio/jumpdownsound.mp3");
    stepSounds[1] = LoadSound("audio/jumpupsound.mp3");
    stepSounds[2] = LoadSound("audio/gameplaysound.mp3");

    // Load point sound
    points[0] = LoadSound("audio/pointgainsound.mp3");
    points[1] = LoadSound("audio/pointlosesound.mp3");
}

void makeFloor(int floorPieceCount,Platform platforms[])
{
    // Defining the floor things
    int floorPieceWidth = 490, floorPieceHeight = 196, floorWhiteSpace = 33, floorX = 0;
    for (int i = 0; i < floorPieceCount; i++)
    {
        platforms[i] = (Platform)
        {
            .x = floorX,
            .y = screenHeight - floorPieceHeight + floorWhiteSpace,
            .width = floorPieceWidth,
            .height = floorPieceHeight,
            .type = FLOOR,
            .isVisible = true,
            .isChecked = true,
        };
        floorX += platforms[i].width;
    }
}

void makePlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[])
{
    int floorPieceHeight = 196;
    int platformWidth = 180, platformHeight = screenHeight * 0.05;
    float platformSpacing = 0.15 * screenWidth;
    int platformCount = worldWidth / (platformWidth + platformSpacing), platform1WhiteSpace = 46, platform2WhiteSpace = 17;
    int platformX = platformSpacing, platformMaxY = screenHeight - floorPieceHeight - 3 * platformHeight, platformMinY = screenHeight * 0.2;

    for (int i = floorPieceCount ; i < platformTotalCount - pointCount - gadgetsCount; i++)
    {
        if(i % 5 == 0)
        {
            platforms[i] = (Platform)
            {
                .x = platformX,
                .y = GetRandomValue(platformMinY, platformMaxY),
                .width = platformWidth,
                .height = screenHeight * 0.05,
                .type = PLATFORM_DEFECTED,
                .isVisible = true,
                .isChecked = false,
            };
        }
        else
        {
            platforms[i] = (Platform)
            {
                .x = platformX,
                .y = GetRandomValue(platformMinY, platformMaxY),
                .width = platformWidth,
                .height = screenHeight * 0.05,
                .type = PLATFORM_FUNCTIONAL,
                .isVisible = true,
                .isChecked = true,
            };
        }
        platformX += platforms[i].width + platformSpacing;
    }

}

void makePointsAndGadgetsOnRandomPlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[])
{
    // Defining the point things
    int pointWidth = 50, pointHeight = 50, value = 0, randValuesPoints[pointCount];
    // Storing random platforms values that are unique
    while(value != pointCount) 
    {
        int rand = GetRandomValue(floorPieceCount, platformTotalCount - pointCount - gadgetsCount - 1); 
        bool copy = false;
        for(int j = 0; j < pointCount ; j++)
        {
            if( randValuesPoints[j] == rand)
            {
                copy = true;
            }
        }
        if( !copy )
        {
            randValuesPoints[value++] = rand;
            //printf("Random values for points:%d\n", rand);
        }
    } 

    // Making the rectangles for the points at random platforms by using previous array
    for(int i = platformTotalCount - pointCount - gadgetsCount ; i < platformTotalCount - gadgetsCount; i++)
    {
        platforms[i] = (Platform){
            .x = platforms[randValuesPoints[i - (platformTotalCount - pointCount - gadgetsCount)]].x + 60,
            .y = platforms[randValuesPoints[i - (platformTotalCount - pointCount - gadgetsCount)]].y - 50,
            .width = pointWidth,
            .height = pointHeight,
            .type = POINT,
            .isVisible = true,
            .isChecked = false,
        };
        //printf("Random values for points:%d\n",randValuesPoints[i - (platformTotalCount - pointCount - gadgetsCount)]);
    }
    makeGadgetsOnRandomPlatforms(floorPieceCount, pointCount, gadgetsCount, platformTotalCount, platforms, randValuesPoints);
}

// Making gadegts 
void makeGadgetsOnRandomPlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[], int randValuesPoints[])
{
    // Defining the gadets things
    int gadgetsWidth = 50, gadgetsHeight = 50, value = 0, randValuesGadgets[gadgetsCount];
    while(value != gadgetsCount) 
    {
        int rand = GetRandomValue(floorPieceCount, platformTotalCount - pointCount - gadgetsCount - 1); 
        bool copy = false;
        // Checks for the same value of the platform of the point and the gadget. So that the point and gadget does not occur on the same platform
        for(int j = 0; j < pointCount ; j++)
        {
            if( randValuesPoints[j] == rand)
            {
                copy = true;
            }
        }
        if( !copy && platforms[rand].type != PLATFORM_DEFECTED )
        {
            // Is it is not same as point's platform and it is also not defected then it should also not repeat so that two gadgets may not ocuur on the same platform
            bool same = false;
            for(int i = 0; i < gadgetsCount; i++)
            {
                if(randValuesGadgets[i] == rand)
                {
                    same = true;
                }
            }
            if(!same)
            {
                randValuesGadgets[value++] = rand;
            }
        }
    } 
    // Making the rectangles for the gadgets at random platforms by using previous array
    for(int i = platformTotalCount - gadgetsCount ; i < platformTotalCount ; i++)
    {
        platforms[i].x = platforms[randValuesGadgets[i - (platformTotalCount - gadgetsCount)]].x + 60;
        if(i % 3 == 2)
        {
            platforms[i].y = platforms[randValuesGadgets[i - (platformTotalCount - gadgetsCount)]].y  - 92;
        }
        else
        {
            platforms[i].y = platforms[randValuesGadgets[i - (platformTotalCount - gadgetsCount)]].y - 60;
        }
        platforms[i].width = gadgetsWidth;
        platforms[i].height = gadgetsHeight + 10;
        platforms[i].type = GADGETS;
        platforms[i].isVisible = true;
        platforms[i].isChecked = false;
        //printf("Random values for the gadgets :%d\n",randValuesGadgets[i - (platformTotalCount - gadgetsCount)]);
    }
}

// Check Collision Function
int character_on_platform(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount)
{
    // Loop through all platforms except last 'pointCount'
    for (int i = 0; i < platformTotalCount - pointCount - gadgetsCount; i++)
    {
        // Define platform and character rectangles for collision check
        Rectangle platform_rec = {
            .x = platforms[i].x, 
            .y = platforms[i].y, 
            .width = platforms[i].width, 
            .height = platforms[i].height
            };
        Rectangle character_rec = {
            .x = character->x, 
            .y = character->y + character->height - character->height * 0.2, 
            .width = character->width, 
            .height = character->height * 0.2 + 1
            };

        // Check if character rectangle collides with platform rectangle
        if (CheckCollisionRecs(character_rec, platform_rec))
        {
            return i; // Return platform index if collision occurs
        }
    }
    return -1; // Return -1 if no collision occurred with any platform
}

// Point collision function
int point_check(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount)
{
    // Loop from last platformTotalCount - pointCount platforms to the end of platforms array
    for(int i = platformTotalCount - pointCount - gadgetsCount; i < platformTotalCount - gadgetsCount; i++)
    {
        // Define character rectangle (position and size)
        Rectangle character_rec = {
            .x = character->x, 
            .y = character->y + character->height - character->height * 0.2, 
            .width = character->width, 
            .height = character->height * 0.2 
        };

        // Define point rectangle (position and size)
        Rectangle point_rec = {
            .x = platforms[i].x, 
            .y = platforms[i].y, 
            .width = platforms[i].width, 
            .height = platforms[i].height 
        };

        // Check for collision between character and point rectangles
        if (CheckCollisionRecs(character_rec, point_rec))
        {
            return i; // Return index if collision detected
        }       
    }
    return -3; // Return -3 if no collision detected
}

// Gadget collision function
int gadgetCheck(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount)
{
    for (int i = platformTotalCount - gadgetsCount; i < platformTotalCount; i++)
    {
        // Define character rectangle (position and size)
        Rectangle character_rec = {
            .x = character->x, 
            .y = character->y + character->height - character->height * 0.2, 
            .width = character->width, 
            .height = character->height * 0.2 
        };

        // Define point rectangle (position and size)
        Rectangle gadget_rec = {
            .x = platforms[i].x, 
            .y = platforms[i].y, 
            .width = platforms[i].width, 
            .height = platforms[i].height 
        };

        // Check for collision between character and gadget rectangles
        if (CheckCollisionRecs(character_rec, gadget_rec))
        {
            return i; // Return index if collision detected
        }       
    }
    return -5; // Return -5 if no collision detected       
}

void DrawGameUI(Camera2D *camera, int scoresPoint, Texture2D *progressBar, int scoreBarHeight, Texture2D gadgets[], int gadgetsCount, Platform platforms[], int platformTotalCount, Texture2D *health, int healthPoints, Texture2D *doraCake )
{
    // Drawing the health points
    int healthPointX = screenWidth - camera->offset.x - 300;
    for(int i = 0; i < healthPoints; i++)
    {
        DrawTexture(*health, healthPointX, 10, WHITE);
        healthPointX += 100;
    }

    // Drawing the doraCake texture
    DrawTexture(*doraCake, worldWidth - doraCake->width - 100, screenHeight - 350, WHITE);

    // Drawing the scores
    DrawTexture(*progressBar, 0 - camera->offset.x, 10, WHITE);
    DrawRectangle(22 - camera->offset.x, 35, scoresPoint, scoreBarHeight + 5, DARKBROWN);
    
    // Format and draw the score text
    char scoreText[20];
    sprintf(scoreText, "Score: %d", scoresPoint);
    DrawText(scoreText, 10 - camera->offset.x, 150, 50, GREEN);

    // Draw the circle with the power-up image
    int x_coordinate = 70, y_coordinate = 650;
    float circleRadius = 70.0f;
    Vector2 circlePositions[3] = {
        {100 - camera->offset.x, 650},
        {200 - camera->offset.x, 760},
        {100 - camera->offset.x, 870},
    };
    Color circleColor = LIGHTGRAY;
    Color circleBorderColor = DARKGRAY;

    // Draw circles with centered power-up images
    for (int i = 0; i < 3; i++)
    {
        // Draw circle
        DrawCircleV(circlePositions[i], circleRadius, circleBorderColor);
        DrawCircleV(circlePositions[i], circleRadius - 5, circleColor);

        // Calculate power-up image position to center it within the circle
        Vector2 gadgetsPosition = {
            circlePositions[i].x - gadgets[i].width / 2,
            circlePositions[i].y - gadgets[i].height / 2
        };

        // Draw power-up image centered within the circle
        if (!platforms[platformTotalCount - gadgetsCount + i].isVisible && !platforms[platformTotalCount - gadgetsCount + i].isChecked)
        {
            int j = 0, gadgetIndex = platformTotalCount - gadgetsCount + i, GadgetX = 0, GadgetY = 0;
            if (gadgetIndex == 89)
            {
                // This means that the gadget with platform index 89 is actually our gadget 2 in the defining array
                j = 2;
                GadgetX = 10;
                GadgetY = -20;
            }
            else if (gadgetIndex == 91)
            {
                // This means that the gadget with platform index 91 is actually our gadget 1 in the defining array
                j = 1;
                GadgetX = -10;
                GadgetY = +20;
            }

            DrawTexturePro(
                gadgets[j],
                (Rectangle){0, 0, (float)gadgets[j].width, (float)gadgets[j].height},
                (Rectangle){gadgetsPosition.x + GadgetX, gadgetsPosition.y + GadgetY, (float)gadgets[j].width, (float)gadgets[j].height},
                (Vector2){0, 0},
                0,
                WHITE
            );
        }
    }
}

// Unloading textures function
void UnloadAllThings(Texture2D characterTextures[], Texture2D *floorTexture, Texture2D platformTextures[], Texture2D *background, Sound stepSounds[], Texture2D *point, Texture2D *progressBar, Sound points[], Texture2D gadgets[], Sound gadgetsSound[], Texture2D *health, Texture2D *doraCake)
{
    // Unload floor texture
    UnloadTexture(*floorTexture);
    
    // Unload health texture
    UnloadTexture(*health);

    // Unload background texture
    UnloadTexture(*background);
    
    // Unload point texture
    UnloadTexture(*point);

    // Unload progress bar texture
    UnloadTexture(*progressBar);

    // Unload doraCake texture 
    UnloadTexture(*doraCake);
    
    // Unload character textures
    for (int i = 0; i < 5; i++)
    {
        UnloadTexture(characterTextures[i]);
    }
    
    // Unload platform textures
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(platformTextures[i]);
    }
    
    // Unload step sounds
    for(int i = 0; i < 3; i++)
    {
        UnloadSound(stepSounds[i]);
    }

    // Unload point gain and lose sounds
    for(int i = 0; i < 2; i++)
    {
        UnloadSound(points[i]);
    }

    // Unload the gadgets texture
    for(int i = 0; i < 4; i++)
    {
        UnloadTexture(gadgets[i]);
    }

    // Unload the sound for gadgets
    for(int i = 0; i < 3; i++)
    {
        UnloadSound(gadgetsSound[i]);
    }
}

