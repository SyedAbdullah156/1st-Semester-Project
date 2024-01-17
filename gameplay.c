// gameplay.c
#include "gadgets.h"
#include "game.h"
#include <raylib.h>
#include <stdio.h>
#include <math.h>

int gameplayScreen(void)
{
    int healthPoints = 3; 
    scoresPoint = 0;
    int frameCount = 0, framesForGadget0 = 360, framesForGadget1 = 120, framesForGadget2 = 120, frameForGadget0Running = 90, framesForTextGadgets = 400, frameCounterForTextGadgets = 0;
    int frameCounterForGadget0 = 0, frameCounterForGadget1 = 0, frameCounterForGadget2 = 0, frameCounterForGadget0Running = 0;
    bool ThisKeyIsPressedForGadget0 = false, ThisKeyIsPressedForGadget1 = false, ThisKeyIsPressedForGadget2 = false, win = false;
    int cannonBallX, cannonBallY, doraCakePick = 0;

    // Initializing the rec for the character by which the collision is to be checked
    Character character = {
        .x = screenWidth / 2, 
        .y = screenHeight / 2, 
        .width = 110, .height = 200, 
        .speed = 15, 
        .direction = 1, 
        .jumping = false, 
        .walking = false,
        .isVisible = true};

    // Initialize textures and sounds
    Texture2D characterTextures[5];
    Texture2D floorTexture;
    Texture2D platformTextures[3];
    Texture2D background;
    Texture2D point;
    Sound stepSounds[3];
    Sound points[2];
    Texture2D progressBar;
    Texture2D gadgets[5];
    Sound gadgetsSound[3];
    Texture2D health;
    Texture2D doraCake;

    InitializeGameTextures(characterTextures, &floorTexture, platformTextures, &background, stepSounds, &point, &progressBar, points, gadgets, gadgetsSound, &health, &doraCake);

    // Defining the background things
    int backgroundWidth = 1792;
    float backgroundRatio = 1 / ((float)(worldWidth - screenWidth) / (float)(backgroundWidth - screenWidth));

    // Defining the platform things
    int platformWidth = 180;
    int platformHeight = screenHeight * 0.05;
    float platformSpacing = 0.15 * screenWidth;
    int platformCount = worldWidth / (platformWidth + platformSpacing);
    int platform1WhiteSpace = 46;
    int platform2WhiteSpace = 17;

    // Defining the floor things
    int floorPieceWidth = 490;
    int floorPieceHeight = 196;
    int floorWhiteSpace = 33;
    int floorPieceCount = ceil((float)worldWidth / (float)floorPieceWidth);

    // Defining the point things
    int pointCount = platformCount / 3;
    int pointWidth = 50;
    int pointHeight = 50;

    // Defining the gadets things
    int gadgetsCount = 3;
    int gadgetsWidth = 50;
    int gadgetsHeight = 50;

    // Defining the doraCake things 
    int doraCakeX = worldWidth - doraCake.width - 100, doraCakeY = screenHeight - 350;
    
    // Making array of total
    int platformTotalCount = floorPieceCount + platformCount + pointCount + gadgetsCount;
    Platform platforms[platformTotalCount];

    // Making underlying rectangles for floor 
    makeFloor(floorPieceCount, platforms);
    // Making underlying rectangles for the platforms
    makePlatforms(floorPieceCount, pointCount, gadgetsCount, platformTotalCount, platforms);
    // Making underlying rectangles for the points and gadgets
    makePointsAndGadgetsOnRandomPlatforms(floorPieceCount, pointCount, gadgetsCount, platformTotalCount, platforms);



    // for(int i = 0; i < platformTotalCount; i++)
    // {
    //     if(platforms[i].type == FLOOR)
    //         printf("FLoor:%d\n",i);
    //     else if(platforms[i].type == PLATFORM_DEFECTED)
    //         printf("Platfrom defected:%d\n",i);
    //     else if(platforms[i].type == PLATFORM_FUNCTIONAL)
    //         printf("Platform Function:%d\n",i);
    //     else if(platforms[i].type == POINT)
    //         printf("Points:%d\n",i);
    //     else
    //         printf("Gadgets:%d\n",i);
    // }

    // Initializing the camera for the screen movement
    Camera2D camera = {.target = {0, 0}, .offset = {0, 0}, .rotation = 0, .zoom = 1};
    PlaySound(stepSounds[2]);

    while (!WindowShouldClose() && healthPoints != 0 && doraCakePick <= 60)
    {
        int doorX;
        bool makeBall = false;
        frameCount++;

        // Checking the condition for the dora cake
        if(character.x + character.width >= doraCakeX )
        {
            doraCakePick++;
        }

        // If after (138s the sound should repeat then multiplied by frames 60)
        if (frameCount % 8280 == 0)
        {
            PlaySound(stepSounds[2]);
        }

        // Making the character with the bounds
        if (character.x < 0)
        {
            character.x = 0;
        }
        else if (character.x > worldWidth - character.width)
        {
            character.x = worldWidth - character.width;
        }
        character.walking = false;

        // Checking the platforms or floor piece form which the character is colliding and making sound(if was in the air)
        int platformIndex = character_on_platform(&character, platforms, pointCount, gadgetsCount, platformTotalCount);
        if (platformIndex == -1)
        {
            PlaySound(stepSounds[0]);
        }
        else if (platformIndex != -1)
        {
            // Ba;lanving the height of the character according to the platforms
            gravity = 0.0f;
            character.y = platforms[platformIndex].y - character.height;
            character.jumping = false;
            // Logic for the Defected platforms and the scores 
            if (platforms[platformIndex].type == PLATFORM_DEFECTED && !platforms[platformIndex].isChecked)
            {
                // for the score points
                scoresPoint -= scoreIncrement;
                if(scoresPoint < 0)
                {
                    scoresPoint = 0;
                }
                platforms[platformIndex].isChecked = true;
                PlaySound(points[1]);
                // for the health points
                healthPoints--;
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                gravity = -40.0f ;
                character.jumping = true;
                PlaySound(stepSounds[1]);
            }
            // Logic for the points when it is on the platform
            int pointIndex = point_check(&character, platforms, pointCount, gadgetsCount, platformTotalCount);
            if(pointIndex != -3 && platforms[pointIndex].isVisible )
            {
                platforms[pointIndex].isVisible = false;
                PlaySound(points[0]);
                scoresPoint += scoreIncrement;
            }

            // Logic for the gadgets when it is on the paltform
            int gadgetIndex = gadgetCheck(&character, platforms, pointCount, gadgetsCount, platformTotalCount);
            if(gadgetIndex != -5 && platforms[gadgetIndex].isVisible)
            {
                platforms[gadgetIndex].isVisible = false;  
            }
        }

        // Making changes for the gadget[0](bambucoptor)
        // the key 1 if pressed after the gadget is picked then the key is said true while the other two gadgets are also not running 
        if(IsKeyPressed(KEY_W) && !platforms[90].isVisible)
        {
            ThisKeyIsPressedForGadget0 = true;
        }
        if(!platforms[90].isVisible && frameCounterForGadget0 < framesForGadget0 && ThisKeyIsPressedForGadget0)
        {
            frameCounterForGadget0++;
            platforms[90].isChecked = true;
            if(IsKeyDown(KEY_DOWN))
            {
                character.y += character.speed;                    
            }
            else if (IsKeyDown(KEY_UP))
            {
                if(character.y > 0)
                {
                    character.y -= character.speed;
                }
                else
                {
                    character.y = 0;
                }                
            }
        }
        if(frameCounterForGadget0 == 1)//We are making frameCounterForGadget == 1 so that the sound for bambucoptor only plays for one time 
        {
            PlaySound(gadgetsSound[0]);
        }
        else if(frameCounterForGadget0 == framesForGadget0)// So that the sound stops when the frames for the gadget(bambucoptor) becomes equal to the time 
        {
            StopSound(gadgetsSound[0]);
        }

        // Making changes for the gadget[2] gate
        // If key A is pressed after the gadget is not visible then the key is done true while the other two gadgets are also not running 
        if(IsKeyPressed(KEY_A) && !platforms[89].isVisible)
        {
            ThisKeyIsPressedForGadget2 = true;
        }
        if(!platforms[89].isVisible && frameCounterForGadget2 < framesForGadget2 && ThisKeyIsPressedForGadget2)
        {
            frameCounterForGadget2++;
            platforms[89].isChecked = true;
            // The character is invisible for the time when it is applicable
            if(frameCounterForGadget2 < framesForGadget2)
            {
                character.isVisible = false;
                character.x += character.speed;
                doorX = character.x - 100;
                if(frameCounterForGadget2 == framesForGadget2 - 60)
                {
                    PlaySound(gadgetsSound[2]);
                }
            }
            else 
            {
                character.isVisible = true;
            }
        }     

        // Making changes for the gadget[1] the cannon and the cannon ball
        // Presing the key for s after picking up the gadget makes the key true while the other two gadgets are also not running 
        if(IsKeyPressed(KEY_S) && !platforms[91].isVisible )
        {
            ThisKeyIsPressedForGadget1 = true;
        }
        if(!platforms[91].isVisible && frameCounterForGadget1 < framesForGadget1 && ThisKeyIsPressedForGadget1)
        {
            frameCounterForGadget1++;
            platforms[91].isChecked = true;
            makeBall = true;
            if(frameCounterForGadget1 == 5)
            {
                // defining the things for ball of cannon when shot is done 
                cannonBallX = character.x + character.width, cannonBallY = character.y + 0.3 * character.height;
                PlaySound(gadgetsSound[1]);
            }
        }


        // Moving the character across the screen
        if (IsKeyDown(KEY_LEFT))
        {
            character.x -= character.speed;
            character.direction = -1;
            character.walking = true;
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            character.x += character.speed;
            character.direction = 1;
            character.walking = true;
        }

        // Changing the camera for the screen movement
        if (character.x > screenWidth * 0.6)
        {
            camera.offset.x = -(character.x - screenWidth * 0.6);
        }
        else if (character.x < screenWidth * 0.4)
        {
            camera.offset.x = -(character.x - screenWidth * 0.4);
        }

        // Limiting camera to stay within the limits
        if (camera.offset.x > 0)
        {
            camera.offset.x = 0;
        }
        else if (camera.offset.x < -(worldWidth - screenWidth))
        {
            camera.offset.x = -(worldWidth - screenWidth);
        }

        // Making the character to move down after jump and he will not be able to jump while using the gadget0(bambucoptor)
        if(!(frameCounterForGadget0 > 0 && frameCounterForGadget0 < framesForGadget0))
        {
            character.y += gravity;
            gravity += 2.0f;
        }


        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(WHITE);

        // Drawing the background
        DrawTexture(background, 0 - camera.offset.x, 0, WHITE);

        // Drawing all platforms (floors and platforms)
        for (int i = 0; i < platformTotalCount; i++)
        {
            if (platforms[i].type == FLOOR)
            {
                DrawTexture(floorTexture, platforms[i].x, platforms[i].y - floorWhiteSpace, WHITE);
            }
            else if(platforms[i].type == PLATFORM_FUNCTIONAL)
            {
                int textureIndex = i % 2;
                DrawTexture(platformTextures[textureIndex], platforms[i].x - 10, platforms[i].y - (textureIndex == 0 ? platform1WhiteSpace : platform2WhiteSpace), WHITE);
            }
            else if(platforms[i].type == PLATFORM_DEFECTED)
            {
                DrawTexture(platformTextures[2], platforms[i].x - 10, platforms[i].y - platform1WhiteSpace , WHITE);
            }
            else if(platforms[i].type == GADGETS)
            {
                int gadgetsIndex = i % 3;
                if(platforms[i].isVisible)
                {
                    DrawTexture(gadgets[gadgetsIndex], platforms[i].x, platforms[i].y, WHITE);                    
                }
            }
            else
            {
                if(platforms[i].isVisible)
                {
                    DrawTexture(point, platforms[i].x, platforms[i].y - 15 , WHITE);
                }
            }
            //DrawRectangleLinesEx((Rectangle){platforms[i].x, platforms[i].y, platforms[i].width, platforms[i].height}, 3, BLACK);
        }

        // Draw the bambucoptor that is running condition
        if(frameCounterForGadget0 > 0 && frameCounterForGadget0 < framesForGadget0)
        {
            DrawTexture(gadgets[3], character.x + 35 , character.y - 35, WHITE);           
        }

        //Drawing the anywhere door(gadgets[4]) when the the frameCounter is bigger than the required and the frmaes for running condiiton are left 
        if(frameCounterForGadget2 >= framesForGadget2 && frameCounterForGadget0Running < frameForGadget0Running)
        {
            frameCounterForGadget0Running++;
            DrawTexture(gadgets[4], doorX, character.y - 80, WHITE);
        }

        // makeBall variable is being repeating true until the frames are left behins once finished the ball will not be drawn 
        if( makeBall )
        {
            DrawCircle(cannonBallX += 15, cannonBallY, 20, BLACK);
        }

        // Drwing the moves for the gadgets
        if(frameCounterForTextGadgets < framesForTextGadgets) 
        {
            frameCounterForTextGadgets++;
            // Calculate the text width and height
            float textWidth = MeasureText("Press the Keys W for bambucoptor A for Anywhere Door", 60);
            float textHeight = 40; // Adjust the height as needed
            // Calculate the position
            float textX = screenWidth / 2 - textWidth / 2 - camera.offset.x + 400;
            float textY = 100;
            // Draw the text in two lines
            DrawText("Press the Keys W for bambucoptor", textX, textY, 40, WHITE);
            DrawText("A for Anywhere Door and S for Cannon", textX, textY + textHeight, 40, WHITE);


        }

        // Changing the frames for the character movement and the jumping movement according to the time
        int x = fmodf(roundf(GetTime() * 8), 4.0);
        int currentFrameIndex = character.jumping ? 4 : (character.walking ? x : 2);
        Texture2D charFrame = characterTextures[currentFrameIndex];
        if(character.isVisible)
        {
            DrawTexturePro(
                charFrame,
                (Rectangle){0, 0, charFrame.width * character.direction, charFrame.height},
                (Rectangle){character.x - 15, character.y + 10, charFrame.width, charFrame.height},
                (Vector2){0, 0},
                0,
                WHITE); 
        //DrawRectangleLinesEx((Rectangle){character.x, character.y, character.width, character.height}, 3, BLACK);
        }

        DrawGameUI(&camera, scoresPoint, &progressBar, scoreBarHeight, gadgets, gadgetsCount, platforms, platformTotalCount, &health, healthPoints, &doraCake);


        EndMode2D();
        EndDrawing();
    }
    UnloadAllThings(characterTextures, &floorTexture, platformTextures, &background, stepSounds, &point, &progressBar, points, gadgets, gadgetsSound, &health, &doraCake);
    return scoresPoint;
}
