#include <stdio.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define BOARDSIZE 5
#define BOAT 'O'
#define HIT 'X'
#define sea '@'

typedef struct ship_s {
  char *spaces;
  int size;
  int status;
} ship;

//Prototype
void computerInit(char *board, char *shots, ship *ships);
void playerInit(char *board, char *shots, ship *ships);
void makeShip(ship *nShip, int size);
void turn(int x, int y, char *board);
void drawScreen(char *pBoard, char *pShots);
int win(ship *pShips, ship *cShips);

int main(void) {
  char pBoard[BOARDSIZE][BOARDSIZE], pShots[BOARDSIZE][BOARDSIZE], //player's Ship and Shot boards
        cBoard[BOARDSIZE][BOARDSIZE], cShots[BOARDSIZE][BOARDSIZE]; //computer's Ship and Shot boards
  ship pShips[3], cShips[3]; //player's and computer's ships
  int currPlayer = 0; //boolean statement for tracking who's turn it is
  
  //Initialize game
  initscr();
  refresh();
  computerInit(&cBoard,&cShots,&cShips);
  playerInit(&pBoard,&pShots,&pShips);
  
  while(!win(pShips,cShips)){ //while no one has won, keep playing the game
    
    currPlayer = !currPlayer;
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
void computerInit(char *board, char *shots, ship *ships){
  for(int i = 0; i < 3; ++i) {
    makeShip(&ships[i], i
  }
}
//Initializes the player's boards, including placement of their ships
void playerInit(char *board, char *shots, ship *ships){
  
}
//Makes new ship
void makeShip(ship *nShip, int size){
  nShip = (ship*)malloc(sizeof(ship));
  *(nShip).spaces = (char*)malloc(size * sizeof(char));
  *(nShip).size = size;
  *(nShip).status = 1;
}
//Used to track the actions each turn, used for both player and computer
void turn(int x, int y, char *board){
  
}
//Draws the screen for the player
void drawScreen(char *pBoard, char *pShots){
  
}
//Used to decide if either the player or the computer have won
//Also deciding if this function will be a recurrsive run to track the whole game, depends on how much memory it uses
int win(ship *pShips, ship *cShips){
  int pwin, cwin;
  pwin = pShips[0].status + pShips[1].status + pShips[2].status;
  cwin = cShips[0].status + cShips[1].status + cShips[2].status;
  if(pwin == 0 && cwin == 0) return 0;
  if(cwin == 0) return 1;
  if(pwin == 0) return 2;
}
