#include <stdio.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Compile with gcc program.c -o program -lncurses
// Run with ./ds4rd.exe -d 054c:09cc -D DS4_BT -t -b -bt -bd | ./program

#define BOARDSIZE 4
#define BOAT 'O'
#define HIT 'X'
#define MISS '@'
#define CURSOR '+'
#define SEA ':'

typedef struct ship_s {
  char **spaces; //coordinates
  int size;
  int status;
} ship;

//Prototype
void computerInit(char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE], ship *ships);
void playerInit(char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE], ship *ships);
ship makeShip(int size);
void setShip(ship nShip, char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE]);
void updateShips(ship *pShips, ship *cShips);
int fire(int x, int y, char (*eBoard)[BOARDSIZE], char (*shots)[BOARDSIZE]);
void hit(int x, int y, ship *ships);
void drawScreen(char (*pBoard)[BOARDSIZE], char (*pShots)[BOARDSIZE]);
int win(ship *pShips, ship *cShips);

int main(void) {
  char pBoard[BOARDSIZE][BOARDSIZE], pShots[BOARDSIZE][BOARDSIZE], //player's Ship and Shot boards
        cBoard[BOARDSIZE][BOARDSIZE], cShots[BOARDSIZE][BOARDSIZE]; //computer's Ship and Shot boards
  ship pShips[3], cShips[3]; //player's and computer's ships
  int currPlayer = 0; //boolean statement for tracking who's turn it is
  int ti, t, c, x, s, l, r, op, sh, r2, l2, r1, l1, up, left, down, right;
  
  //Inintializes the board to be all sea to prepare for ship placement
  for(int i = 0; i < BOARDSIZE; ++i) {
    for(int k = 0; k < BOARDSIZE; ++k) {
      pBoard[i][k] = SEA;
      pShots[i][k] = SEA;
      cBoard[i][k] = SEA;
      cShots[i][k] = SEA;
    }
  }
  drawScreen(pBoard, pShots);
  
  srand(time(NULL));
  //Initialize game
  initscr();
  refresh();
  
  computerInit(cBoard,cShots,cShips);
  playerInit(pBoard,pShots,pShips);
  
  mvprintw(0, BOARDSIZE + 2, "D-Pad -> Move the cursor");
  mvprintw(1, BOARDSIZE + 2, "X Button -> Select space");
  mvprintw(2, BOARDSIZE + 2, "Square Button -> Rotate boat");
  
  //Ensure no input before start
  while(up || down || left || right || x) {
	scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
  }
  
  while(win(pShips,cShips) == 0){ //While no one has won, keep playing the game
    
    if(!currPlayer) { //player's turn
	  mvprintw(4, BOARDSIZE + 2, "Player's turn!");
	  refresh();
      drawScreen(pBoard, pShots);
      int row = 0, col = 0;
	  while(up || down || left || right || x) {
		scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
	  }
      while(!x) {
		while(up || down || left || right || x) {
		  scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
		}
        scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
        mvaddch(row,col,pShots[row][col]);
        if(up && row > 0) row--;
        if(down && row < BOARDSIZE - 1) row++;
        if(left && col > 0) col--;
        if(right && col < BOARDSIZE - 1) col++;
        mvaddch(row,col,CURSOR);
        refresh();
      }
      mvprintw(5, BOARDSIZE + 2, fire(row,col,cBoard,pShots) ? "HIT!" : "MISS!");
	  refresh();
    } else { //computer's turn
	  mvprintw(4, BOARDSIZE + 2, "Computer's turn!");
	  refresh();
      int row, col;
      do {
        row = rand() % BOARDSIZE;
        col = rand() % BOARDSIZE;
      } while(cShots[row][col] == HIT || cShots[row][col] == MISS);
      mvprintw(5, BOARDSIZE + 2, fire(row,col,pBoard,cShots) ? "HIT!" : "MISS!");
	  refresh();
    }
    updateShips(pShips,cShips);
    currPlayer = !currPlayer; //Switch player
	scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
	int end = ti + 400;
	while(ti < end) {
	  scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
	}
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
void computerInit(char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE], ship *ships){
  int good, dir = 0, row, col;
  for(int i = 0; i < 3; i++) { //this *should* work
    ships[i] = makeShip(i + 1); //this works
	good = 0;
	while(good == 0){
	  good = 1;
      dir = rand() % 2;
	  if(dir) {
	    row = rand() % ((BOARDSIZE - ships[i].size) + 1);
		col = rand() % BOARDSIZE;
	  } else {
		row = rand() % BOARDSIZE;
		col = rand() % ((BOARDSIZE - ships[i].size) + 1);
	  }
      for(int k = 0; k < ships[i].size; ++k) {
        if(dir) {
		  if(board[row + k][col] == BOAT){ 
		    good = 0;
		  }
        } else {
		if(board[row][col + k] == BOAT){ 
		  good = 0;
		}
        }
      }
	}
	
	//from here down works
	for(int k = 0; k < ships[i].size; ++k) {
      if(dir) {
		board[row + k][col] = BOAT;
        ships[i].spaces[k] = &board[row + k][col];
      } else {
	    board[row][col + k] = BOAT;
        ships[i].spaces[k] = &board[row][col + k];
      }
    }
  }
}
//Initializes the player's boards, including placement of their ships  (works)
void playerInit(char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE], ship *ships){
  for(int i = 0; i < 3; ++i) {
    ships[i] = makeShip(i + 1);
    setShip(ships[i], board, shots);
  }
  
}
//Makes new ship  (works)
ship makeShip(int length){
  ship *newShip = (ship *)malloc(sizeof(ship));
  (*newShip).spaces = (char **)malloc(length * sizeof(char *));
  (*newShip).size = length;
  (*newShip).status = 1;
  return *newShip;
  
  
}
//Sets the ship into a location and assigns the board locations to the spaces double pointer  (works)
void setShip(ship nShip, char (*board)[BOARDSIZE], char (*shots)[BOARDSIZE]){
  int ti, t, c, x = 1, s, l, r, op, sh, r2, l2, r1, l1, up, left, down, right, row = 0, col = 0, dir = 0;
  drawScreen(board, shots);
  while(up || down || left || right || x) {
	scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
  }
  while(!x) {
	
	while(up || down || left || right || x || s) {
	  scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
	}
    for(int i = 0; i < nShip.size; ++i) {
      if(dir) {
        mvaddch(row + BOARDSIZE + 1 + i, col, board[row + i][col]);
      } else {
        mvaddch(row + BOARDSIZE + 1, col + i, board[row][col + i]);
      }
    }
    scanf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d", &ti, &t, &c, &x, &s, &l, &r, &op, &sh, &r2, &l2, &r1, &l1, &up, &left, &down, &right);
    if(up && row > 0) row--;
    if(down && row < BOARDSIZE - (dir ? nShip.size : 1)) row++;
    if(left && col > 0) col--;
    if(right && col < BOARDSIZE - (dir ? 1 : nShip.size)) col++;
    if(s) dir = !dir;
    for(int i = 0; i < nShip.size; ++i) {
      if(dir) {
        mvaddch(row + BOARDSIZE + 1 + i, col, BOAT);
      } else {
        mvaddch(row + BOARDSIZE + 1, col + i, BOAT);
      }
    }
	mvaddch(row + BOARDSIZE + 1, col, CURSOR);
    refresh();
  }
  for(int i = 0; i < nShip.size; ++i) {
    if(dir) {
	  board[row + i][col] = BOAT;
      nShip.spaces[i] = &board[row + i][col];
    } else {
	  board[row][col + i] = BOAT;
      nShip.spaces[i] = &board[row][col + i];
    }
      
  }
}
//updates the status of each ship, 1 if still afloat, 0 if ship sunken (works)
void updateShips(ship *pShips, ship *cShips){
  for(int i = 0; i < 3; ++i) {
    int pSpaces = 0, cSpaces = 0;
    for(int k = 0; k < pShips[i].size; ++k) {
      if(*(pShips[i].spaces[k]) == BOAT){
		++pSpaces;
	  }
    }
    if(pSpaces == 0){ 
	  pShips[i].status = 0;
	}
    for(int k = 0; k < cShips[i].size; ++k) {
      if(*(cShips[i].spaces[k]) == BOAT){
		++cSpaces;
	  }
    }
    if(cSpaces == 0){ 
	  cShips[i].status = 0;
	}
  }
}
//Used to track the actions each turn, used for both player and computer  (works)
int fire(int row, int col, char (*eBoard)[BOARDSIZE], char (*shots)[BOARDSIZE]){
  if(eBoard[row][col] == BOAT) {
    eBoard[row][col] = HIT;
    shots[row][col] = HIT;
    return 1;
  } else {
    shots[row][col] = MISS;
    return 0;
  }
}
//Draws the screen for the player  (works)
void drawScreen(char (*pBoard)[BOARDSIZE], char (*pShots)[BOARDSIZE]){
  for(int row = 0; row < BOARDSIZE; ++row) {
    for(int col = 0; col < BOARDSIZE; ++col){
      mvaddch(row,col,pShots[row][col]);
      mvaddch(row + BOARDSIZE + 1,col,pBoard[row][col]);
    }
  }
  refresh();
}
//Used to decide if either the player or the computer have won  (works)
int win(ship *pShips, ship *cShips){
  int pwin, cwin;
  pwin = pShips[0].status + pShips[1].status + pShips[2].status;
  cwin = cShips[0].status + cShips[1].status + cShips[2].status;
  if(cwin == 0) return 1; //player wins
  if(pwin == 0) return 2; //computer wins
  return 0; //no one has won yet
}
