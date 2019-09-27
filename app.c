#define W 600
#define H 600
#define SNAKE_WIDTH 10
#define SNAKE_MAXSIZE 100
#define BORDER_WIDTH 10

extern int jsSetInterval(void (*callback)(), int timeout);
extern void jsClearInterval(int index);

void runCallback(void (*callback)()) {
  return callback();
}

extern void print(int message);
extern void alert(int message);
extern void jsFillRect(int x, int y, int w, int h, int color);
extern void jsClearRect(int x, int y, int w, int h);
extern int random(int min, int max);

typedef struct Rect {
  int x;
  int y;
  int w;
  int h;
} Rect;

typedef enum Type {
  WALL,
  SNAKE,
} Type;


typedef struct Object {
  Type t;
  Rect r;  // Bounds
} Object;


/***
 *
 *   Snake (as stack) start
 *
 */
typedef struct Snake {
    int end_index;
    int start_index;
    // 0 - right, 1 down, 2 left, 3 top
    int direction;
    Rect rects[SNAKE_MAXSIZE];
} Snake;

Snake snake = {-1, -1};

void snake_insert(int x, int y)
{
    if (snake.end_index == SNAKE_MAXSIZE - 1) {
        // rewind queue
        for (int i = 0; i < SNAKE_MAXSIZE; i++) {
            snake.rects[i] = snake.rects[i + snake.start_index];
        }

        snake.end_index = snake.end_index - snake.start_index;
        snake.start_index = 0;
    }

    if (snake.end_index < SNAKE_MAXSIZE - 1) {
        if (snake.start_index == - 1) {
            snake.start_index = 0;
        }
        snake.end_index = snake.end_index + 1;
        Rect rect = {x, y, SNAKE_WIDTH, SNAKE_WIDTH};
        snake.rects[snake.end_index] = rect;
    } else {
        print(1);
    }
} /* End of insert() */

void snake_delete()
{
    if (snake.start_index != - 1 && snake.start_index < snake.end_index)
    {
        snake.start_index = snake.start_index + 1;
    }
} /* End of delete() */

Rect get_snake_head() {
    return snake.rects[snake.end_index];
}

void snake_growth() {
    Rect head = get_snake_head();

    switch (snake.direction) {
        case 0: // right
            snake_insert(head.x + SNAKE_WIDTH, head.y);
            break;

        case 1: // bottom
            snake_insert(head.x, head.y + SNAKE_WIDTH);
            break;

        case 2: // left
            snake_insert(head.x - SNAKE_WIDTH, head.y);
            break;

        case 3: // top
            snake_insert(head.x, head.y - SNAKE_WIDTH);
            break;
    }
}

void snake_change_direction (new_direction) {
    int delta = new_direction - snake.direction;
    if (delta != 2 && delta != -2) {
        snake.direction = new_direction;
    }
}

/**
 * Snake  end
 **/

// border
Rect borders[4] = {
  { 0, 0, W, BORDER_WIDTH },       // Top Wall
  { 0, H - BORDER_WIDTH, W, BORDER_WIDTH },  // Bottom Wall
  { 0, 0, BORDER_WIDTH, H },     // Left Wall
  { W - BORDER_WIDTH, 0, BORDER_WIDTH, H },       // Right Wall
};

Rect foods[100] = {
    { 400, 300, SNAKE_WIDTH, SNAKE_WIDTH}
};
food_count = 1;

void remove_food(int index) {
  for (int i = index; i < food_count; i++)
     foods[i] = foods[i+1];

  food_count = food_count - 1;
}

int board_point_count_w = (W - BORDER_WIDTH * 2) / SNAKE_WIDTH - 1;
int board_point_count_h = (H - BORDER_WIDTH * 2) / SNAKE_WIDTH - 1;

void add_food_randomly() {
  int x = random(0, board_point_count_w) * SNAKE_WIDTH + BORDER_WIDTH;
  int y = random(0, board_point_count_h) * SNAKE_WIDTH + BORDER_WIDTH;

  Rect food = {x, y, SNAKE_WIDTH, SNAKE_WIDTH};
  foods[food_count] = food;
  food_count++;
}


void draw() {
  jsClearRect(0, 0, W, H);
  for (int i = 0; i < 4; i++) {
    Rect r = borders[i];
    jsFillRect(r.x, r.y, r.w, r.h, 0);
  }

  for (int i = 0; i < food_count; i++) {
    Rect r = foods[i];
    jsFillRect(r.x, r.y, r.w, r.h, 0xFFA500);
  }

  for (int i = snake.start_index; i <= snake.end_index; i++) {
    Rect r = snake.rects[i];
    jsFillRect(r.x, r.y, r.w, r.h, 0xFF0000);
  }
}

int valueInRange(int value, int min, int max) {
  return (value >= min) && (value < max);
}

int mainInterval;

void move_snake() {
    snake_growth();
    snake_delete();

    Rect head = get_snake_head();

    int border_touched = 0;
    for (int i = 0; i < 4; i++) {
      Rect border = borders[i];
      if (valueInRange(head.x, border.x, border.x + border.w) && valueInRange(head.y, border.y, border.y + border.h)) {
        border_touched = 1;
        break;
      }
    }

    if (border_touched) {
        alert(1);
        jsClearInterval(mainInterval);
    }

    for (int i = 0; i < food_count; i++) {
        Rect food = foods[i];
        if (head.x == food.x && head.y == food.y) {
           remove_food(i);
           if (food_count < 10) {
               add_food_randomly();
           }
           if (food_count < 7) {
               add_food_randomly();
           }

           snake_growth();
           break;
        }
    }



    draw();
}

void handleKeyPress(int direction_code) {
    snake_change_direction(direction_code);
}

void handleClick(int x, int y) {
//  Object point = {.r = {x, y, 10, 10}}
// map[world_count] = point;
//  world_count++;
//  draw();
}

int main() {
  snake_insert(W / 2, H / 2);
  snake_growth();
  snake_growth();
  snake_growth();
  snake_growth();

  draw();
  mainInterval = jsSetInterval(move_snake, 250);

  return 0;
}
