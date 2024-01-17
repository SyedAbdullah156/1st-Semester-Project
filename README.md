# 1st-Semester-Project

This project, completed by "Syed Abdullah," marks the culmination of the first semester, concluding on "1-12-2024."

## Acknowledgment

I hope this project not only reflects my journey but also provides an opportunity for others to learn from it. Constructive feedback is welcome, and collaboration to enhance this work is encouraged.

## Key Points

- This project explores various aspects of [Raylib](https://www.raylib.com/), a C library for game development.
- Tutorials for Raylib in C, especially without Object-Oriented Programming (OOP), are relatively scarce. 

## Learning Resources

- Utilize the Raylib examples included in this repository for practical demonstrations.
- Leverage AI assistance from ChatGPT and BARD for enhanced understanding and problem-solving. These AI tools can provide valuable insights and guidance.

## Project Overview

- This project draws inspiration from the classic Mario game, but with a unique twist – it dives into the world of the beloved Doraemon character. In this adventure, Doraemon     embarks on a quest to collect points cleverly shaped as coins while navigating through a series of platforms, akin to steps or rectangular blocks.

- As our character progresses, it discovers handy gadgets or powers nestled within its journey. These gadgets play a crucial role in overcoming obstacles, adding an extra layer   of excitement and strategy to the gameplay.

## Basic Structure

- The main.c file is the main(central) file of the program which runs on three important functions
  
- These functions are defined in the header file game.h as:
- 1. titleScreen function(contained within title.c file)
  2. gameplayScreen function(contained within gameplay.c file)
  3. endingScreen function(contained within ending.c file)
    
- Moreover the the gameplayScreen fucntions is the core gameplay function that further contains bunch of functions that are declared in the header file gadgets.h and defined in    gadgets.c
  
- Together you can think of it as main.c that runs on title.c, gameplay.c(that runs on gadgets.c and gadgets.h) and ending.c and these three are connected by game.h

- The scores of the game are repeatedly updated in the scores.txt file
