#include <stdio.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define BOARDSIZE 5
#define BOAT 'O'
#define HIT 'X'
#define sea '@'

typedef struct ship_s {
  int *spaces;
  int size;
  int status;
} ship;

//Prototype
void computerInit(char *board, char *shots);
void playerInit(char *board, char *shots);
void turn(int x, int y, char *board);
void drawScreen(char *pBoard
int win(ship *pShips, ship *cShips);

int main(void) {
  char pBoard[BOARDSIZE][BOARDSIZE], pShots[BOARDSIZE][BOARDSIZE], //player's Ship and Shot boards
        cBoard[BOARDSIZE][BOARDSIZE], cBoard[BOARDSIZE][BOARDSIZE]; //computer's Ship and Shot boards
  ship pShips[3], cShips[3]; //player's and computer's ships
  int currPlayer; //boolean statement for tracking who's turn it is
  initscr();
  refresh();
  
  while(!win(pShips,cShips)){
    
  }
  
  /* Code for if win fuction is made recurrsive
  switch(win(pShips,cShips)) {
    case 1://player wins
      
    case 2://computer wins
      
    default://error
      
  }
  */
  
}

//Implementations
//Inintializes the computer's boards, including placement of it's ships
void computerInit(char *board, char *shots){
  
}
//Initializes the player's boards, including placement of their ships
void playerInit(char *board, char *shots){
  
}
//Used to track the actions each turn, used for both player and computer
void turn(int x, int y, char *board){
  
}
//Used to decide if either the player or the computer have won
//Also deciding if this function will be a recurrsive run to track the whole game, depends on how much memory it uses
int win(ship *pShips, ship *cShips){
  
}
