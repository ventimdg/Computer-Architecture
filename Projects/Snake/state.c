#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static unsigned int get_next_x(unsigned int cur_x, char c);
static unsigned int get_next_y(unsigned int cur_y, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);


/* Task 1 */
game_state_t* create_default_state() {
  struct game_state_t *answer = (game_state_t *) malloc(sizeof(struct game_state_t));
  answer->snakes = (snake_t *) malloc(sizeof(struct snake_t));
  answer->board = (char **) malloc(sizeof(char *) * 18);
  answer->num_rows = 18;
  answer->num_snakes = 1;
  answer->snakes->tail_x = 2;
  answer->snakes->tail_y = 2;
  answer->snakes->head_x = 4;
  answer->snakes->head_y = 2;
  answer->snakes->live = true;
  unsigned int i = 0;
  unsigned int j = 0;
  for (i; i < 18; i++){
    answer->board[i] = (char *) malloc(sizeof(char) * 21);
  }
  i = 0; 
  for (i; i < 20; i++){
    for (j; j < 18; j++) {
      if (i==0 || i==19 || j==0 || j==17) {
        set_board_at(answer, i, j, '#');
        if (i == 19) {
          set_board_at(answer, i + 1, j , '\n');
        }
      } else {
        set_board_at(answer, i, j, ' ');
      }
    }
    j = 0;
  }
  set_board_at(answer, 9, 2, '*');
  set_board_at(answer, 2, 2, 'd');
  set_board_at(answer, 3, 2, '>');
  set_board_at(answer, 4, 2, 'D');
  return answer;
}


/* Task 2 */
void free_state(game_state_t* state) {
  unsigned int i = 0;
  for (i; i < state->num_rows; i++){
    free(state->board[i]);
  }
  free(state->board);
  free(state->snakes);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  unsigned int i = 0;
  unsigned int j = 0;
  for (i; i < state->num_rows; i++){
    for (j; j< strlen(state->board[i]); j++){
      char cur = get_board_at(state, j, i);
      if (cur != 'q') {
        fprintf(fp, "%c", get_board_at(state, j, i));
      }
    }
    j = 0;
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}


/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int x, unsigned int y) {
  return state->board[y][x];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int x, unsigned int y, char ch) {
  state->board[y][x] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASD"
  Returns false otherwise.
*/
static bool is_head(char c) {
  return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<>v"
*/
static bool is_snake(char c) {
  return  (c == '^' || c == '<' || c == '>' || c == 'v' || is_head(c) || is_tail(c));
}

/*
  Converts a character in the snake's body ("^<>v")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  if (c == '^'){
    return 'w';
  }
  else if (c == '<'){
    return 'a';
  }
  else if (c == 'v'){
    return 's';
  }
  else if (c == '>'){
    return 'd';
  } else {
    return NULL;
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<>v").
*/
static char head_to_body(char c) {
  if (c == 'W'){
    return '^';
  }
  else if (c == 'A'){
    return '<';
  }
  else if (c == 'S'){
    return 'v';
  }
  else if (c == 'D'){
    return '>';
  } else {
    return NULL;
  }
}

/*
  Returns cur_x + 1 if c is '>' or 'd' or 'D'.
  Returns cur_x - 1 if c is '<' or 'a' or 'A'.
  Returns cur_x otherwise.
*/
static unsigned int get_next_x(unsigned int cur_x, char c) {
  if (c == '>' || c == 'd' || c == 'D') {
    return cur_x + 1;
  } else if (c == '<' || c == 'a' || c == 'A') {
    return cur_x - 1;
  } 
  return cur_x;
}

/*
  Returns cur_y + 1 if c is '^' or 'w' or 'W'.
  Returns cur_y - 1 if c is 'v' or 's' or 'S'.
  Returns cur_y otherwise.
*/
static unsigned int get_next_y(unsigned int cur_y, char c) {
  if (c == '^' || c == 'w' || c == 'W') {
    return cur_y - 1;
  } else if (c == 'v' || c == 's' || c == 'S') {
    return cur_y + 1;
  } 
  return cur_y;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  char head = get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y);
  unsigned int x_coord = get_next_x(state->snakes[snum].head_x, head);
  unsigned int y_coord = get_next_y(state->snakes[snum].head_y, head);
  return get_board_at(state, x_coord, y_coord);
}


/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the x and y coordinates of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  char head = get_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y);
  unsigned int x_coord = get_next_x(state->snakes[snum].head_x, head);
  unsigned int y_coord = get_next_y(state->snakes[snum].head_y, head);
  set_board_at(state, x_coord, y_coord, head);
  set_board_at(state, state->snakes[snum].head_x, state->snakes[snum].head_y, head_to_body(head));
  state->snakes[snum].head_x = x_coord;
  state->snakes[snum].head_y = y_coord;
  return;
}


/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^v<>) into a tail character (wasd)

  ...in the snake struct: update the x and y coordinates of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  char tail = get_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y);
  unsigned int x_coord = get_next_x(state->snakes[snum].tail_x, tail);
  unsigned int y_coord = get_next_y(state->snakes[snum].tail_y, tail);
  set_board_at(state, state->snakes[snum].tail_x, state->snakes[snum].tail_y, ' ');
  set_board_at(state, x_coord, y_coord, body_to_tail(get_board_at(state, x_coord, y_coord)));
  state->snakes[snum].tail_x = x_coord;
  state->snakes[snum].tail_y = y_coord;
  return;
}


/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  int numsnakes = state->num_snakes;
  int i = 0;
  for (i; i < numsnakes; i++){
    int x = state->snakes[i].head_x;
    int y = state->snakes[i].head_y;
    char head = get_board_at(state, x, y);
    if (head != 'x') {
      char next = next_square(state, i);
      if (next == '*'){
        update_head(state, i);
        add_food(state);
      } else if (next != ' '){
        set_board_at(state, x, y, 'x');
        state->snakes[i].live = false;
      } else {
        update_head(state, i);
        update_tail(state, i);
      }
    }
  }
  return;
}


/* Task 5 */
game_state_t* load_board(char* filename) {
  char holder;
  FILE *linecounter;
  linecounter = fopen(filename, "r");
  int numlines = 0;
  int longest = 0;
  int cur = 0;
  for (holder = getc(linecounter); holder != EOF; holder = getc(linecounter)) {
    cur++;
    if (holder == '\n') {
      numlines = numlines + 1;
      if (cur > longest) {
        longest = cur;
      }
      cur = 0;
    }
  }
  longest = longest + 5;
  fclose(linecounter);
  char fileline[longest];
  struct game_state_t *answer = (game_state_t *) malloc(sizeof(struct game_state_t));
  answer->board = (char **) malloc(sizeof(char *) * numlines);
  FILE *board = fopen(filename, "r");
  int i = 0;
  int j = 0;
  int numsnakes = 0;
  fgets(fileline, longest, board);
  while(!feof(board)) {
    int len = strlen(fileline);
    answer->board[i] = (char *) malloc(sizeof(char) * len);
    for (j; j < len; j++){
      answer->board[i][j] = fileline[j];
    }
    j = 0;
    i++;
    fgets(fileline, longest, board);
  }
  answer->num_rows = i;
  fclose(board);
  return answer;
}


/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail coordinates filled in,
  trace through the board to find the head coordinates, and
  fill in the head coordinates in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  int colx = state->snakes[snum].tail_x;
  int rowy = state->snakes[snum].tail_y;
  char cur = state->board[rowy][colx];
  while(!is_head(cur)){
    colx = get_next_x(colx, cur);
    rowy = get_next_y(rowy, cur);
    cur = state->board[rowy][colx];
  } 
  state->snakes[snum].head_x = colx;
  state->snakes[snum].head_y = rowy;
  return;
  }


/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  int i = 0;
  int j = 0;
  int numsnakes = 0;
  state->snakes = (snake_t *) malloc(sizeof(struct snake_t));
  for (i; i < state->num_rows; i++){
    for(j; j < strlen(state->board[i]); j++){
      char cur = state->board[i][j];
      if(is_tail(cur)) {
        int snake = numsnakes;
        numsnakes++;
        if(numsnakes > 1){
          state->snakes = (snake_t *) realloc(state->snakes, sizeof(struct snake_t) * numsnakes);
        }
        state->snakes[snake].tail_x = j;
        state->snakes[snake].tail_y = i;
        find_head(state, snake);
      }
    }
    j = 0;
  }
  state->num_snakes = numsnakes;
  return state;
}
//Done
