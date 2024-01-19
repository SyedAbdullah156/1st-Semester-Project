// title.c
#include "game.h"
#include <stdio.h>
#include <raylib.h>

typedef struct {
    int x;
    int y;
    int height;
    int width;
}PlayButton;

typedef struct {
    int x;
    int y;
    int height;
    int width; 
}OptionButton;


bool check_mouse_on_playbutton(PlayButton *playButton)
{
    //   559 > x > 721   and  500 > y > 558
    if(GetMouseX() >= playButton->x && GetMouseX() <= playButton->x + playButton->width && GetMouseY() >= playButton->y && GetMouseY() <= playButton->y + playButton->height)// More explicitly use can use 500 instead of playButton_y but it does not matter a lot as it get down at a very high speed
    {
        return true;
    }
    return false;
}

bool check_mouse_on_optionbutton(OptionButton *optionButton)
{
    if(GetMouseX() >= optionButton->x && GetMouseX() <= optionButton->x + optionButton->width && GetMouseY() >= optionButton->y && GetMouseY() <= optionButton->y + optionButton->height)
    {
        return true;
    }
    return false;
}

bool check_mouse_on_backbutton()
{
    if(GetMouseX() >= (int)(screenWidth * 0.2) + 180 && GetMouseX() <= (int)(screenWidth * 0.2) + 230 && GetMouseY() >= (int)(screenHeight * 0.2) + 150 && GetMouseY() <= (int)(screenHeight * 0.2) + 230)
    {
        return true;
    }
    return false;
}

void titleScreen(void) 
{
    // For play button made of three (162x172) buttons:
    PlayButton playButton;
    playButton.height = 58; // That is equal to allPlayButtons.height/3 = 172/3= 58 for single play button out of three play buttons
    playButton.width = 162; // This is equal to allPlayButtons.width = 162 same for each button
    playButton.x = (screenWidth / 2) - (playButton.width / 2);// Where playButton.x = 559 in this case. We are actually subtracting the mid x conponent of play button from the mid x component of the screen so that the playbutton appears on the center of the screen
    playButton.y = 0;// Which will vary in such a way that it will first continuously increase until it approaches a value of 500 and then stop to give a permanent image of play button

    //For option button made of three (162x162) buttons:
    OptionButton optionButton;
    optionButton.height = 54; // That is equal to allOptionButtons.height/3 = 162/3 = 58 for one option button out of three option buttons
    optionButton.width = 162; // That is equal to allOptionButtons.width = 162 same for each button
    optionButton.x = (screenWidth / 2) - (optionButton.width / 2);// Where optionButton.x = 559 in this case.
    optionButton.y = 590; // Because the playButton.y_max as mentioned in the function(check_mouse_on_playbutton) is 558 so making it more bigger for the option button
    bool optionDisplay = false;

    // FOr back button
    int backButtonHeight = 374/3;
    // These frames are used for changing the play and option buttons when mouse is whirld or clicked on then.
    int playFrame = 0, optionFrame = 0, backbuttonFrame = 0;

    // Getting texture and sound
    Texture2D mouse = LoadTexture("picture/mouse.png");
    Texture2D titlePage = LoadTexture("picture/titlePage.png");
    Texture2D allPlayButtons = LoadTexture("picture/playbutton.png");
    Texture2D allOptionButtons = LoadTexture("picture/options.png");
    Texture2D scoreBoard = LoadTexture("picture/scoreBoard.png");
    Texture2D backButton = LoadTexture("picture/backbutton.png");
    Sound jungle = LoadSound("audio/jungle.mp3");
    Sound mouseClick = LoadSound("audio/mouseclick.mp3");
    Font customFont = LoadFont("font/font.ttf");

    HideCursor();
    bool soundPlayed = false, gamePlay = true;  // Flag to track if sound has been played

    SetTargetFPS(60);
    while (!WindowShouldClose() && gamePlay)
    {

        //Making playButton to fall down
        if(playButton.y < 500)
        { 
            playButton.y += 10;// That is the speed by which the playButton will get down
        }
        else
        {
            playButton.y = 500;
        }

        //Checking mouse on the playButton
        if(check_mouse_on_playbutton(&playButton))
        {
            playFrame = 2;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                playFrame = 1;
                PlaySound(mouseClick);
                gamePlay = false;
            }
        }
        else
        {
            playFrame = 0;
        }

        //Checking mouse on the option buttton
        if(check_mouse_on_optionbutton(&optionButton))
        {
            optionFrame = 2;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                optionFrame = 1;
                PlaySound(mouseClick);
                optionDisplay = true;
            }
        }
        else
        {
            optionFrame = 0;
        }

        // Checking mouse on the back button
        if(check_mouse_on_backbutton())
        {
            backbuttonFrame = 2;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                backbuttonFrame = 1;
                PlaySound(mouseClick);
                optionDisplay = false;
            }
        }
        else
        {
            backbuttonFrame = 0;
        }

        //Playing Sound
        if (!soundPlayed)
        {
            PlaySound(jungle);
            soundPlayed = true;  // Set the flag to true after playing the sound
        }
        
        //Making Drawing
        BeginDrawing();// The order of making drawings matter so make it should be in line wise order

        // This will be printed on the whole scrren with 1280x720 pic of titlePage
        // This should be printed first so that all other things will be above it
        DrawTexturePro(
            titlePage,
            (Rectangle){0, 0, 1792, 1024}, 
            (Rectangle){0, 0, 1792, 1024},
            (Vector2){0, 0}, 
            0,
            WHITE
        );
        
        //Making a playButton
        // Rectangle source will change with the movement of the mouse, while the rectangle destination will remain same after completing its journey down the screen
        DrawTexturePro(
            allPlayButtons,
            (Rectangle){0, playButton.height * playFrame  , playButton.width, playButton.height},// Here the x = 0 remains the same and y matters. For first frame it is 0 * playButton.height. For second frame it will start after the 0th ffrmae as 1 * playButton.heigth. For third frame it will start after the second frame as 2 * playButton.height
            (Rectangle){playButton.x, playButton.y, 200, 100},// Here the playButton.x remains the same as given the value of subtraction inthe above but the vlaue of y co-ordinate increses till it approaches 500 and after that remains const
            (Vector2){0, 0},
            0,
            WHITE
        );

        //Making optionButton after playbutton
        DrawTexturePro(
            allOptionButtons,
            (Rectangle){0, optionButton.height * optionFrame  , optionButton.width, optionButton.height},// Same as playButton
            (Rectangle){optionButton.x, optionButton.y, 200, 90},// A little bit contracted vertically 
            (Vector2){0, 0},
            0,
            WHITE
        );

        if(optionDisplay)
        {
            DrawTexture(scoreBoard, screenWidth * 0.2, screenHeight * 0.2, WHITE);
            DrawTextEx(customFont,"Score Highlights!", (Vector2){(screenWidth * 0.2) + 250, (screenHeight * 0.2) + 200}, 50, 10, BLACK );
            // Printing scores
            char scoresTextOption[500];
            int scoresX = (screenWidth * 0.2) + 250, scoresY = (screenHeight * 0.2) + 260; 
            for(int i = 0; i < 5; i++)
            {
                sprintf(scoresTextOption, "\nGame %d scores: %d",  i + 1, score[i]);
                DrawTextEx(customFont,scoresTextOption, (Vector2){scoresX, scoresY}, 20, 5,  BLACK);
                scoresY += 50;
            }
            // Drawing the back button
            DrawTexturePro(
                backButton,
                (Rectangle){0, backButtonHeight * backbuttonFrame, 92, backButtonHeight},
                (Rectangle){(screenWidth * 0.2) + 150, (screenHeight * 0.2) + 150, 92, backButtonHeight},
                (Vector2){0, 0},
                0,
                WHITE
            );
        }
        
        //Hiding the cursur to make a custom cursor in the end
        DrawTexturePro(
            mouse,
            (Rectangle){0.0f, 0.0f, mouse.width, mouse.height},
            (Rectangle){GetMouseX(), GetMouseY(), 40, 40},
            (Vector2){0, 0},
            0,
            WHITE);

        EndDrawing();
    }
    UnloadTexture(titlePage);
    UnloadTexture(allPlayButtons);
    UnloadTexture(allOptionButtons);
    UnloadTexture(mouse);
    UnloadTexture(scoreBoard);
    UnloadSound(jungle);
    UnloadSound(mouseClick);
    UnloadFont(customFont);
    UnloadTexture(backButton);
}
