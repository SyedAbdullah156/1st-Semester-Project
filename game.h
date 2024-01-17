#ifndef GAME_H
#define GAME_H

#define SCREEN_FACTOR 1
extern int screenWidth;
extern int screenHeight;
extern int scoresPoint;
extern int score[5];

typedef enum {TITLE = 0, GAMEPLAY, ENDING } GameScreen;

// Function prototypes
void titleScreen(void);
int gameplayScreen(void);
char endingScreen(void);

#endif // GAME_H
