#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define max_X 77
#define max_Y 23
#define min_X 3
#define min_Y 4

#define UP 119
#define DOWN 115
#define LEFT 97
#define RIGHT 100

/*
This is of of my first C Programs, I wasn't a good programmer at that time.
The Game is generally good but the code implementation can be done better.
I will update it when I have time. Check my github again for an update version. \
DUY LE-DINH
*/

struct SnakeLocation {
  int x;
  int y;
struct SnakeLocation *next;

};

struct FoodLocation {
  int x;
  int y;
};

//Global pointers, keep in mind that you shouldn't use it. I was just lazy a bit.
struct SnakeLocation *current = NULL;
struct SnakeLocation *tail = NULL;
struct SnakeLocation *head = NULL;
struct FoodLocation *ptr_to_food = NULL;

void initGame();
void createGameBorder();
void createFood();
void createSnake(int x_coordinate, int y_coordinate);
void snakeMove(int x_coordinate, int y_coordinate);
void moveCursorTo(int x_coordinate, int y_coordinate);
void taskDelay(int input_time);

int inputSignal();
int snakeEatFood();
int gameOver();


int main() {
  initGame();
  int next_direction = DOWN, prev_direction = 0;
  int score_display = 0, time_left = 220;
  int temp;

  while (1) {
    while ((!inputSignal()) && (!gameOver(time_left))) {
      time_left--;
      //For reducing counting time, when time = 0: GAME OVER!
      snakeMove(prev_direction, next_direction);
      /*
      Create new Food, update Score and reset time
      (When Snake eat food, time will be reseted)
      */
      if (snakeEatFood()) {
        createSnake(tail->x - 1, tail->y - 1);
        createFood();
        score_display++;
        time_left = 220;
        moveCursorTo((min_X+5), (min_Y-2));
        printf(" YOUR SCORE:%d ", score_display);
      }
      /* Print time if game not Over */
      if (!gameOver(time_left)) {
        moveCursorTo((max_X - 16), (min_Y-2));
        printf(" TIME LEFT:%d ", time_left/11);
      }
      /* Check if game over */
      if (gameOver(time_left)) {
        system("clear");
        moveCursorTo((min_X+7), min_Y);
        printf("GAME OVER! YOUR SCORE:%d POINTS - PRESS CTR+C TO EXIT", score_display);
        break;
      }
    }
    /*
    FOR CONTROLING WHICH DIRECTION THAT SNAKE MOVES.
    This make sure that if you press LEFT then LEFT,
    snake won't go to the RIGHT (see more in the snakeMoveFunction
    */
    temp = next_direction;
    next_direction = getchar();
    if (next_direction != temp) {
      prev_direction = temp;
    }
  }
}
// END OF MAIN FUNCTION

/*
This function clear the terminal screen for game,
print game interface and snake as well as food for snake.
*/
void initGame() {
  system("clear");
  createSnake(16, 11); //*head
  createFood();
  createGameBorder();
  moveCursorTo((min_X+5), (min_Y-2));
  printf(" YOUR SCORE:%d ", 0);
  moveCursorTo((max_X - 16), (min_Y-2));
  printf(" TIME LEFT:%d ", 0);
}

/*
This function create game Border,
with value min_X, min_Y, max_X, max_Y define at the beginning of the program
*/
void createGameBorder() {
  int i;
  for (i = min_X; i <= max_X; i++) {
    moveCursorTo(i, max_Y);
    printf("|");
  }
  for (i = min_Y; i <= max_Y; i++) {
    moveCursorTo(max_X, i);
    printf("||");
  }
  for (i = min_X; i <= max_X; i++) {
    moveCursorTo(i, min_Y);
    printf("|");
  }
  for (i = min_Y; i <= max_Y; i++) {
    moveCursorTo(min_X, i);
    printf("||");
  }
}
/*
This function return 1 if gameOver, and 0 if game is not over.
*/
int gameOver(int time) {
  for (current = tail; current != head; current = current->next) {
    if ((head->x == current->x) && (head->y == current->y)) {
      return 1;
    }
  }
  if ((head->x == min_X + 1) || (head->y == min_Y)
       || (head->x == max_X) || (head->y == max_Y)) { //Game over if snake hits wall,
    return 1;
  }
  if(time == 0) {
  return 1;
  } else return 0;
}

/*
Check if snake hits food.
*/
int snakeEatFood() {
  if ((head->x == ptr_to_food->x) && (head->y == ptr_to_food->y)) {
    return 1;
  }
  return 0;
}

/*
This control how snake move. LEFT, RIGHT, UP, DOWN are defined in the beginning.
*/
void snakeMove(int prev_direction, int next_direction) {
  moveCursorTo(tail->x, tail->y);
  printf(" ");
  switch (next_direction) {

  case LEFT:
    if (prev_direction == RIGHT) {
      tail->x = (head->x + 1);
      tail->y = head->y;
      break;
    } else {
      tail->x = (head->x - 1);
      tail->y = head->y;
      break;
    }
  case RIGHT:
    if (prev_direction == LEFT) {
      tail->x = (head->x - 1);
      tail->y = head->y;
      break;
    } else {
      tail->x = (head->x + 1);
      tail->y = head->y;
      break;
    }
  case UP:
    if (prev_direction == DOWN) {
      tail->y = (head->y + 1);
      tail->x = head->x;
      break;
    } else {
      tail->x = head->x;
      tail->y = (head->y - 1);
      break;
    }
  case DOWN:
    if (prev_direction == UP) {
      tail->x = head->x;
      tail->y = (head->y - 1);
      break;
    } else {
      tail->y = (head->y + 1);
      tail->x = head->x;
      break;
    }
  }
  //Updating Snake Head after change the position of Tail to Head
  head->next = tail;
  moveCursorTo(tail->x, tail->y);
  printf("o");
  tail = tail->next;
  head = head->next;
  taskDelay(3000);
}

/*
Using random function to create new location for food.
*/
void createFood() {
  struct FoodLocation *food = malloc(sizeof *food);
  ptr_to_food = food;
  food->x = rand() % (max_X - min_X) + min_X;
  food->y = rand() % (max_Y - min_Y) + min_Y;
  moveCursorTo(food->x, food->y);
  printf("X");
}

/*
This is a Linked-List functions, used to create a new part of snake, if snake eat Food
*/
void createSnake(int x, int y) {
  struct SnakeLocation* link = (struct SnakeLocation*) malloc(
  sizeof(struct SnakeLocation));
  link->x = x;
  link->y = y;
  if (tail == NULL) {     // If there is no node, this will create the first node
    tail = link;
    head = link;
    tail->next = NULL;
  } else {
    link->next = tail;
    tail = link;
  }
}

/*
This Function uses ANSI escaped sequence:
ESC[y;xf to move the cursor of the terminal to (x,y) coordinate. 0x1B is ESC
*/
void moveCursorTo(int x, int y) {
  printf("%c[%d;%df", 0x1B, y, x);
}

/*
This is a stupid trick, but it works. But it is fun so I keep it.
The better way is usleep() function of <unistd.h> library.
*/
void taskDelay(int x) {
  int i = 0, k = 0;
  for (i = 0; i < 10000 * x; i++) {
    k++;
  }
}

/*
* This is the most important function. What it does is if there is an input
from Keyboard, it will return 1, otherwise, it return 0. This allow the user to
control the game smoothly with the keyboard. Without it, the game won't work.
* non- Canonical mode:
The program does not has to wait for 'enter' to receive an input.
After enter something, it is available for the program right away.
* O_NONBLOCK:
By default, the STDIN_FILENO is in BLOCKMODE. What we want here is to change
to O_NONBLOCK so that read() on STDIN_FILENO, which is a slow file will return
immediately even if no byte available. After getchar(), we need to change it back
to Block mode, otherwise, at the end of the game, you will see a very funny behavior.
*/

int inputSignal() {
struct termios terminal ;
int ch;
// Get terminal setting
tcgetattr(STDIN_FILENO, &terminal );
// Local mode, set to non-Canonical mode, no ECHO
terminal.c_lflag &= ~(ICANON | ECHO);
// Set terminal setting again to new mode
tcsetattr(STDIN_FILENO, TCSANOW, &terminal );
// Set O_NONBLOCK mode
fcntl(STDIN_FILENO, F_SETFL, (fcntl(STDIN_FILENO, F_GETFL, 0)) | O_NONBLOCK);
ch = getchar();
// turn off O_NONBLOCK
fcntl(STDIN_FILENO, F_SETFL, (fcntl(STDIN_FILENO, F_GETFL, 0)) &~ O_NONBLOCK);
if (ch != EOF) {
  ungetc(ch, stdin);
  return 1;
} else
  return 0;
}
