#ifndef GADGETS_H
#define GADGETS_H

#include "game.h"
#include <raylib.h>

extern float gravity;
extern int worldWidth;
extern int scoreIncrement , scoreBarWidth, scoreBarHeight ;
extern int scoresPoint;
extern float gravity;
extern int worldWidth;
extern int scoreIncrement;
extern int scoreBarWidth;
extern int scoreBarHeight;


typedef struct
{
    float x, y, width, height;
    int speed, direction;
    bool jumping, walking, isVisible;
} Character;

typedef enum
{
    PLATFORM_DEFECTED,
    PLATFORM_FUNCTIONAL,
    FLOOR,
    POINT,
    GADGETS,
} PlatformType;

typedef struct
{
    float x, y, width, height;
    bool isVisible, isChecked;
    PlatformType type;
} Platform;

// Making game Textures
void InitializeGameTextures(Texture2D characterTextures[], Texture2D *floorTexture, Texture2D platformTextures[], Texture2D *background, Sound stepSounds[], Texture2D *point, Texture2D *progressBar, Sound points[], Texture2D gadgets[], Sound gadgetsSound[], Texture2D *health, Texture2D *doraCake);

// Making the floor
void makeFloor(int floorPieceCount, Platform platforms[]);

// Making the platforms 
void makePlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[]);

// Making points on random platforms
void makePointsAndGadgetsOnRandomPlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[]);

// Making gadgets on random platforms
void makeGadgetsOnRandomPlatforms(int floorPieceCount, int pointCount, int gadgetsCount, int platformTotalCount, Platform platforms[], int randValuesPlatforms[]);

// Check Collision Function between character and the platforms
int character_on_platform(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount);

// Check Collision Function between character and the points
int point_check(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount);

// Check Collision Function between character and the gadgets
int gadgetCheck(Character *character, Platform platforms[], int pointCount, int gadgetsCount, int platformTotalCount);

// Drawing the score bar and the health points with the gadgets circles
void DrawGameUI(Camera2D *camera, int scoresPoint, Texture2D *progressBar, int scoreBarHeight, Texture2D gadgets[], int gadgetsCount, Platform platforms[], int platformTotalCount, Texture2D *health, int healthPoints, Texture2D *doraCake);

// Unloading game textures
void UnloadAllThings(Texture2D characterTextures[], Texture2D *floorTexture, Texture2D platformTextures[], Texture2D *background, Sound stepSounds[], Texture2D *point, Texture2D *progressBar, Sound points[], Texture2D gadgets[], Sound gadgetsSound[], Texture2D *health, Texture2D *doraCake);
#endif