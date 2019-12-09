#include <stdio.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>

// Compile with gcc program.c -o program -lncurses
// Run with ./ds4rd.exe -d 054c:05c4 -D DS4_BT -t -b -bt -bd | ./program

#define BOARDSIZE 4
#define BOAT '@'
#define HIT 'X'
#define MISS 'O'
#define CURSOR '+'
#define SEA ':'

typedef struct ship_s {
  char **spaces; //coordinates
  int size;
  int status;
} ship;

//Prototype
void computerInit(char *board, char *shots, ship *ships);
void playerInit(char *board, char *shots, ship *ships);
void makeShip(ship *nShip, int size);
void setShip(ship *nShip);
void updateShips(ship *pShips, ship *cShips);
int fire(int x, int y, char *eBoard, char *shots);
void hit(int x, int y, ship *ships);
void drawScreen(char *pBoard, char *pShots);
int win(ship *pShips, ship *cShips);

int main(void) {
  char pBoard[BOARDSIZE][BOARDSIZE], pShots[BOARDSIZE][BOARDSIZE], //player's Ship and Shot boards
        cBoard[BOARDSIZE][BOARDSIZE], cShots[BOARDSIZE][BOARDSIZE]; //computer's Ship and Shot boards
  ship pShips[3], cShips[3]; //player's and computer's ships
  int currPlayer = 0; //boolean statement for tracking who's turn it is
  int time, t, c, x, s, l, r, op, sh, r2, l2, r1, l1, up, left, down, right;
  
  //Inintializes the board to be all sea to prepare for ship placement
  for(int i = 0; i < BOARDSIZE; ++i) {
    for(int k = 0; k < BOARDSIZE; ++k) {
      pBoard[i][k] = SEA;
      pShots[i][k] = SEA;
      cBoard[i][k] = SEA;
      cShots[i][k] = SEA;
    }
  }
  
  srand(time(NULL));
  //Initialize game
  initscr();
  refresh();
  computerInit(cBoard,cShots,cShips);
  playerInit(pBoard,pShots,pShips);
  mvprintw(0, BOARDSIZE + 2, "D-Pad -> Move the cursor");
  mvprintw(1, BOARDSIZE + 2, "X Button -> Select space");
  mvprintw(2, BOARDSIZE + 2, "Square Button -> Rotate boat");
  
  while(!win(pShips,cShips)){ //While no one has won, keep playing the game
    
    if(!currPlayer) { //player's turn
      drawScreen(pBoard, pShots);
      int row = 0, col = 0;
      while(!x) {
        scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &time, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
        mvaddch(row,col,pShots[row][col]);
        if(up && row > 0) row--;
        if(down && row < BOARDSIZE - 1) row++;
        if(left && col > 0) col--;
        if(right && col < BOARDSIZE - 1) col++;
        mvaddch(row,col,CURSOR);
        refresh();
      }
      mvprintw(15, BOARDSIZE + 2, fire(row,col,cBoard,pShots) ? "HIT!" : "MISS!");
    } else { //computer's turn
      int row, col;
      do {
        row = rand() % BOARDSIZE;
        col = rand() % BOARDSIZE;
      } while(cShots[row][col] == HIT || cShots[row][col] == MISS);
      mvprintw(15, BOARDSIZE + 2, fire(row,col,pBoard,cShots) ? "HIT!" : "MISS!");
    }
    updateShips(pShips,cShips);
    currPlayer = !currPlayer; //Switch player
  }
  endwin();
  switch(win(pShips,cShips)) {
    case 1: //Player wins
      printf("You win!");
      break;
    case 2: //Computer wins
      printf("You lose. Try again next time.");
      break;
    default: //error?
      printf("An error occured, please try again.");
  }
  
  return 0;
}

//Implementations
//Inintializes the computer's boards, including placement of it's ships
void computerInit(char *board, char *shots, ship *ships){
  for(int i = 2; i >= 0; ++i) {
    makeShip(&ships[i], i + 1);
    int dir = rand() % 2;
    int row = rand() % (dir ? BOARDSIZE - ships[i].size : BOARDSIZE), col = rand() % (dir ? BOARDSIZE : BOARDSIZE - ships[i].size);
    for(int k = 0; k < ships[i].size; ++k) {
      if(dir) {
        ships[i].spaces[k] = &board[row + k][col];
      } else {
        ships[i].spaces[k] = &board[row][col + k];
      }
      
    }
  }
  
}
//Initializes the player's boards, including placement of their ships
void playerInit(char *board, char *shots, ship *ships){
  for(int i = 2; i >= 0; ++i) {
    makeShip(&ships[i], i + 1);
    setShip(&ships[i], board);
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
void setShip(ship *nShip, char *board){
  int time, t, c, x, s, l, r, op, sh, r2, l2, r1, l1, up, left, down, right, row = 0, col = 0, dir = 0;
  while(!x) {
    for(int i = 0; i < (*nShip).size; ++i) {
      if(dir) {
        mvaddch(row + BOARDSIZE + i, col, SEA);
      } else {
        mvaddch(row + BOARDSIZE, col + i, SEA);
      }
    }
    scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &time, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
    if(up && row > 0) row--;
    if(down && row < BOARDSIZE - 1) row++;
    if(left && col > 0) col--;
    if(right && col < BOARDSIZE - 1) col++;
    if(s) dir = !dir;
    for(int i = 0; i < (*nShip).size; ++i) {
      if(dir) {
        mvaddch(row + BOARDSIZE + i, col, BOAT);
      } else {
        mvaddch(row + BOARDSIZE, col + i, BOAT);
      }
    }
    refresh();
  }
  for(int i = 0; i < (*nShip).size; ++i) {
    if(dir) {
      (*nShip).spaces[i] = &board[row + i][col];
    } else {
      (*nShip).spaces[i] = &board[row][col + i];
    }
      
  }
}
//updates the status of each ship, 1 if still afloat, 0 if ship sunken
void updateShips(ship *pShips, ship *cShips){
  for(int i = 0; i < 3; ++i) {
    int pSpaces = 0, cSpaces = 0;
    for(int k = 0; k < pShips[i].size; ++k) {
      if(pShips[i].spaces[k] == BOAT) pSpaces++;
    }
    if(!pSpaces) pShips[i].status = 0;
    for(int k = 0; k < cShips[i].size; ++k) {
      if(cShips[i].spaces[k] == BOAT) cSpaces++;
    }
    if(!cSpaces) cShips[i].status = 0;
  }
}
//Used to track the actions each turn, used for both player and computer
int fire(int row, int col, char *eBoard, char *shots){
  if(eBoard[row][col] == BOAT) {
    eBoard[row][col] = HIT;
    shots[row][col] = HIT;
    return 1;
  } else {
    shots[row][col] = MISS;
    return 0;
  }
}
//Draws the screen for the player
void drawScreen(char *pBoard, char *pShots){
  for(int x = 0; x < BOARDSIZE; ++x) {
    for(int y = 0; y < BOARDSIZE; ++y){
      mvaddch(y,x,pShots[x][y]);
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
  if(cwin == 0) return 1; //player wins
  if(pwin == 0) return 2; //computer wins
  return 0; //no one has won yet
}
