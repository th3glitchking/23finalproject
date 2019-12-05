#include <stdio.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define BOARDSIZE 5
#define BOAT 'O'
#define HIT '$'
#define MISS 'X'
#define CURSOR '+'
#define SEA '@'

typedef struct ship_s {
  char **spaces;
  int size;
  int status;
} ship;

//Prototype
void computerInit(char *board, char *shots, ship *ships);
void playerInit(char *board, char *shots, ship *ships);
void makeShip(ship *nShip, int size);
void setShip(ship *nShip);
int turn(int x, int y, char *eBoard, char *shots);
void hit(int x, int y, ship *ships);
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
  
  while(!win(pShips,cShips)){ //While no one has won, keep playing the game
    
    if(!currPlayer) { //player's turn
      mvprintw(30, 0, " ");
    } else { //computer's turn
      
    }
    
    currPlayer = !currPlayer; //Switch player
  }
  
  
  
}

//Implementations
//Inintializes the computer's boards, including placement of it's ships
void computerInit(char *board, char *shots, ship *ships){
  for(int i = 2; i >= 0; ++i) {
    makeShip(&ships[i], i + 1);
    setShip(&ships[i]);
  }
  
}
//Initializes the player's boards, including placement of their ships
void playerInit(char *board, char *shots, ship *ships){
  for(int i = 2; i >= 0; ++i) {
    makeShip(&ships[i], i + 1);
    setShip(&ships[i]);
  }
  
}
//Makes new ship
void makeShip(ship *nShip, int size){
  nShip = (ship*)malloc(sizeof(ship));
  *(nShip).spaces = (char**)malloc(size * sizeof(char*));
  *(nShip).size = size;
  *(nShip).status = 1;
}
//Sets the ship into a location and assigns the board locations to the spaces double pointer
void setShip(ship *nShip, int x, int up, int down, int left, int right){
  while(!x) {
    
  }
}
//Used to track the actions each turn, used for both player and computer
int turn(int x, int y, ship *eShips, char *shots){
  if(eBoard[x][y] == BOAT) {
    eBoard[x][y] = HIT;
    shots[x][y] = HIT;
    return 1;
  } else {
    shots[x][y] = MISS;
    return 0;
  }
}
//Draws the screen for the player
void drawScreen(char *pBoard, char *pShots){
  for(int x = 0; x < BOARDSIZE; ++x) {
    for(int y = 0; y < BOARDSIZE; ++y){
      mvaddch(y,x,pShots[x][y]);
    }
  }
  for(int x = 0; x < BOARDSIZE; ++x) {
    for(int y = 0; y < BOARDSIZE; ++y){
      mvaddch(y + BOARDSIZE + 1,x,pBoard[x][y]);
    }
  }
  refresh();
}
//Used to decide if either the player or the computer have won
int win(ship *pShips, ship *cShips){
  int pwin, cwin;
  pwin = pShips[0].status + pShips[1].status + pShips[2].status;
  cwin = cShips[0].status + cShips[1].status + cShips[2].status;
  if(pwin == 0 && cwin == 0) return 0;
  if(cwin == 0) return 1;
  if(pwin == 0) return 2;
}
