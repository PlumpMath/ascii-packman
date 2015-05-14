#include <time.h>
#include <pthread.h>
#include "curses.h"

// 오브젝트별 출력할 아스키 코드 및 색상
const char floor_ch      = ' ';
const char floor_color   = 17;
const char wall_ch       = '#';
const char wall_color    = 19;
const char player0_ch    = 'A';
const char player0_color = 3;
const char player1_ch    = 'B';
const char player1_color = 7;
const char player2_ch    = 'C';
const char player2_color = 4;
const char enemy_ch      = 'x';
const char enemy_color   = 8;
const char fruit_ch      = '.';
const char fruit_color   = 6;
const char item_ch       = '?';
const char item_color    = 6;
const char effect_ch     = '*';
const char effect_color  = 8;

// 맵 개수, 맵 크기
const int number_of_maps = 9;
const int columns_of_map = 60;
const int rows_of_map = 20;

// 맵 데이터
const char map[9][20][60] = {
{
"############################################################",
"                             ####                           ",
"# ###### ###### ######## ### ############### ########## ## #",
"# ###### ###### ######## ### ############### ########## ## #",
"# ###### ###### ######## ###          ###### ##########    #",
"#        ###### ######## ############ ###### ############# #",
"# ############# ######## ############ ###### ############# #",
"#                        ############ ###### ############# #",
"# ############## ####### ############   #### ############# #",
"# ############## ####### ############ # #### ############# #",
"               # #######                     ########       ",
"# ############ #         ##### ######### ############ #### #",
"# ############ # ############# ######### ############ #### #",
"# #########                    ######### ############ #### #",
"# ######### ################## #########                   #",
"# ######### ################## ######### ######## ######## #",
"# ######### ################## ######### ######## ######## #",
"# ######### ################## ######### ######## ######## #",
"                          ####                              ",
"############################################################",
},
{
"############################################## #############",
"#                                                          #",
"#       #   ### ################ ## ## #### #  ######### # #",
"# #######     #     #####        ## ## #### ##         # # #",
"#         ### # ### #     ########             ####### # # #",
"# #########   #     # ### #        ######## ##       # # # #",
"#    #        # ##### ### # ############### ## ##### # # # #",
"# ## # ########         # #                 ##             #",
"#    #          ####### # #  ############## ############ # #",
"  #########   #      ## # ##              #            # #  ",
"#         ##### #### ## # ############### # ########## # # #",
"# ####### #   #                         # # #          # # #",
"# ####        # ####################### # # ########## # # #",
"# #### ########                                          # #",
"# ####        # ###### ######## ################### #### # #",
"# ####### #          #          #                        # #",
"#         ##### #### ############ ######### ############ # #",
"# #########   #      ############ ######### ############ # #",
"#                                                          #",
"############################################## #############",
},
{
"######################### ######## #########################",
"#                                                          #",
"# ######## ######## ##### ## ###### ###### #### ##### ## # #",
"# ######## ######## ##### ## ###### #   ## #### ##### ## # #",
"           ######## ##### ## ######   # ##      ##### ## #  ",
"# ######## ######## ##### ## ###### ### ## ########## ## # #",
"# ########                            # ## ########## ## # #",
"#          ########### ############## # ## ########## ## # #",
"# ##########         # ###            # ##            ## # #",
"# #          ####### # ###  ###  #### # ################ # #",
"# # ################ # ####  ###  ### # #######          # #",
"# # ##            ## #            ### #         ########## #",
"# # ## ########## ## # ############## ###########          #",
"# # ## ########## ## #                            ######## #",
"# # ## ########## ## ### ###### ## ##### ######## ######## #",
"  # ## #####      ## #   ###### ## ##### ########           ",
"# # ## ##### #### ##   # ###### ## ##### ######## ######## #",
"# # ## ##### #### ###### ###### ## ##### ######## ######## #",
"#                                                          #",
"######################### ######## #########################",
},
{
"########################## ## ##############################",
"#                          ##                              #",
"# ### ###### # ###### #### ## ###### ##### ##### ######### #",
"# ### ####     ###### #### ## ####   #####                 #",
"# ###      ########## #### ## #### ######### #### ### #### #",
"# ### #### ########## #### ##         ###### #### ### #### #",
"#     #### #####      #### ## #######    ### ####     #### #",
"# ### #### ##### ####      ## ####### ## ### #########     #",
"# ### #### ##### ######### ## ####### ## ###           ### #",
"# ###      ##### ######### ########## ## ############# ### #",
"# ### #### ##### #########    #######              ### ### #",
"# ### ####          ######### ####### ### #### ### #   ### #",
"# ### #### ######## ###### ##         ### #### ### # ##### #",
"# ###      ########        ## ##### ##### #### ### # ##### #",
"# ### #### ######## ## ### ## ##### ##### ####        #### #",
"# ### #### ######## ## ### ##                  ######      #",
"#     ####             ### ## ##### ######### ####### #### #",
"# ######## ###### ######## ## ##### ######### ####### #### #",
"#                          ##                              #",
"########################## ## ##############################",
},
{
"############################################################",
"#   #   #   ######                       #####     #   #   #",
"# # # # # #        ################### #       # # # # # # #",
"# # # # # ## #####       ###   #       # ##### # # # # # # #",
"  #   # # ## #   #  #### ### # # ### ###       # # # #   #  ",
"# # # # # ## # # # #         # #   # ########### #   # # # #",
"# # # # # ## # # # ##  # ##### ###             # # # # # # #",
"# # # # # ## # #    # ## #       ##### ####### # # # # # # #",
"# # # #   #       #   ## # #####     #      ## # # #   # # #",
"#   # # # # ### #   # ## #     ##### ###### ## # # # # #   #",
"#   # # # # ### # # #    #####            # ## # # # # #   #",
"# # # #   # ### #   # ##    ## ########## # ## # # #   # # #",
"# # # # # # ### # # # ##### ## #          # ## # # # # # # #",
"# # # # # # ### #   #            ########## ## # # # # # # #",
"# # # # # # ### # # ###### #####            ## # #   # # # #",
"  #   # # # ##           #  ##   #### ######## # # # #   #  ",
"# # # # # # ## ######### ## ## ###    #        # # # # # # #",
"# # # # # # ## ######### ## ## #   ## # ######## # # # # # #",
"#   #   #   ##                   ####              #   #   #",
"############################################################",
},
{
"#############################   ############################",
"#####     #################       ###############       ####",
"#####  ##    ########                   #######   ####  ####",
"##### ######                                    ###### #####",
"#####  #####    # #### ##         ############   ####  #####",
"######   #   ###   ##   ##             ##    ##   #   ######",
"#######    ##    ###### ###      #    #####   ##     #######",
"#######   ###     ####     # ## #      ###            ######",
"######     ##            # #    ##      #    ##        #####",
"#####       ######## ##### ########## #######           ####",
"#####              #       ######                       ####",
"######      ########         ##         ###########    #####",
"#######      #########               ############     ######",
"########       ###############################       #######",
"##########         #########################       #########",
"############         ######################      ###########",
"################          #########          ###############",
"#####################                   ####################",
"###########################       ##########################",
"#############################   ############################",
},
{
"############################################################",
"#####################               ########################",
"##############                             #################",
"############                                  ##############",
"#########        ##                              ###########",
"#######         #####                              #########",
"#####          #######                   ###################",
"###             #####                #######################",
"###               ##              ##########     ###########",
"##                              ###########       ##########",
"#                                 #####     #    ###########",
"#                                       ####################",
"##                                    ######################",
"###                                      ###################",
"#####                                               ########",
"#######                                          ###########",
"#########                                      #############",
"###########                                  ###############",
"################                         ###################",
"############################################################",
},
{
"################  ###########  ###########  ################",
"##                 #########                 ###############",
"###               ###########               ################",
"####             #############             #################",
"#####           ###############           ##################",
"######         #################         ###################",
"#######       ###################       ####################",
"########     #####################     #####################",
"#########   #######################   ######################",
"########## ######################### #######################",
"####################### ######################### ##########",
"######################   #######################   #########",
"#####################     #####################     ########",
"####################       ###################       #######",
"###################         #################         ######",
"##################           ###############           #####",
"#################             #############             ####",
"################               ###########               ###",
"###############                 #########                 ##",
"################  ###########  ###########  ################",
},
{
"############################  ##############################",
"#      #                                            #      #",
"# #### # ###################  ##################### # #### #",
"# #### # #                                        # # #### #",
"# #### # # ###################################### # # #### #",
"         #                                                  ",
"########## #################  # ############################",
"#                          #  #                            #",
"# ###################### # #  # ######################## # #",
"# #                      # #  # #                        # #",
"# #                      # #  # #                        # #",
"# # ###################### #  # # ######################## #",
"#                          #  #                            #",
"########################## #  ################### ##########",
"                                                  #        ",
"# #### # # ###################################### # # #### #",
"# #### # #                                        # # #### #",
"# #### # ###################  ##################### # #### #",
"#      #                                            #      #",
"############################  ##############################",
},
};

// 열거형 및 구조체 타입 정의
enum Direction {
  STOP   = 0,
  EAST   = 1,
  WEST   = 2,
  SOUTH  = 3,
  NORTH  = 4,
};

enum ObjectType {
  FLOOR  = 0,
  WALL   = 1,
  PLAYER = 2,
  ENEMY  = 3,
  FRUIT  = 4,
  ITEM   = 5,
  EFFECT = 6,
};

enum ItemType {
  NOTHING = 0,
  KNIFE   = 1,
  BOMB    = 2,
  BOOSTER = 3,
  FREEZER = 4,
};

enum State {
  NORMAL = 0,
  FAST   = 1,
  SLOW   = 2,
  FROZEN = 3,
  DEAD   = 4,
};

struct Position {
  int x;
  int y;
};

struct Player {
  struct Position  pos;
  enum Direction   dir;
  enum State       state;
  int              life;
  int              fruit;
  enum ItemType    item;
  clock_t          tick;
  clock_t          state_end;
  int              item_use;
};

struct Enemy {
  struct Position  pos;
  enum Direction   dir;
  int              state;
  clock_t          tick;
  clock_t          state_end;
};

struct Fruit {
  struct Position  pos;
  enum State       state;
};

struct Item {
  struct Position  pos;
  enum State       state;
};

struct Effect {
  struct Position  pos;
  enum Direction   dir;
  enum State       state;
  clock_t          tick;
  clock_t          state_end;
};

// 게임 옵션
const int time_limit = 120 * 1000;
const int number_of_players = 3;
const int number_of_enemies = 13;
const int number_of_fruits = 31;
const int number_of_items = 10;
const int speed_fastest = 30;
const int speed_fast = 110;
const int speed_normal = 190;
const int speed_slow = 500;

// 전역 변수
int           map_id;
struct Player player[3];
struct Enemy  enemy[13];
struct Fruit  fruit[31];
struct Item   item[10];
struct Effect bomb[10];
struct Effect effect[40];
clock_t       start_time;
int           next_second;   // 초 계산
int           game_quit = 0;
int           game_win = 0;
int           refresh_status;
int           refresh_message = 1;
int           refresh_object = 1;
int           refresh_time = 1;
int           fruit_remain;


// 함수들.
void TitleMenu();
void PrepareGame();
void PlayGame();
void ShowResult();
void ShowHelp();
void Input();
void Draw();
void InitCurse();
void InitColor();
int IsSamePos(struct Position* pos1, struct Position* pos2);
enum ObjectType CheckCollision(struct Position* pos);
struct Position PickARandPos();
void Move(struct Position* pos, enum Direction dir);
enum ObjectType TryMove(struct Position pos, enum Direction dir);
int FindFruitByPos(struct Position pos);
int FindEnemyByPos(struct Position pos);
int FindItemByPos(struct Position pos);
int FindEffectByPos(struct Position pos);
void DrawMap(const int map_id);
void DrawFloor(const int map_id);
void DrawMessage();
void DrawStatusFrame();
void DrawStatus();
void DrawTime();
void DrawObjects();
void DamagePlayer(int id);
void DamageEnemy(int id);
void UpdateGameQuit();
void UpdateTime();
void MovePlayer();
void MoveEnemy();
void MoveEffect();
void MoveBomb();
void UseItem();

// main
int main(int argc, char** argv) {
  InitCurse();
  InitColor();
  TitleMenu();
  endwin();
  return 0;
}

void TitleMenu() {
  int program_quit = 0;
  char ch = '\0';
  while(!program_quit) {
    clear();
    attron(A_BOLD);
    attron(COLOR_PAIR(3));
    mvprintw(5, 25, "A S C I I     P A C K - M A N ");
    mvprintw(15, 25, "[P] PLAY GAME");
    mvprintw(16, 25, "[H] HELP");
    mvprintw(17, 25, "[Q] QUIT");
    mvprintw(13, 25, "Choose an option :");
    attroff(COLOR_PAIR(3));
    attroff(A_BOLD);

    ch = getch();
    switch(ch) {
      case 'p':
      case 'P':
        PrepareGame();
        PlayGame();
        ShowResult();
        break;
      case 'h':
      case 'H':
        ShowHelp();
        break;
      case 'q':
      case 'Q':
        program_quit = 1;
        break;
      default:
        break;
    }
  }
}

// 게임 로직 처리
void PlayGame() {
  while(!game_quit) {
    Input();
    UpdateGameQuit();
    UpdateTime();
    MovePlayer();
    UseItem();
    MoveEnemy();
    MoveEffect();
    MoveBomb();
    Draw();
  }
}

void ShowResult() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  if(game_win == 1) {
    if(player[0].fruit > player[1].fruit && player[0].fruit > player[2].fruit) {
      mvprintw(5, 6, "PLAYER 1 IS THE WINNER!!!");
      mvprintw(7, 10, "PLAYER 1 : %d fruits", player[0].fruit);
      mvprintw(8, 10, "PLAYER 2 : %d fruits", player[1].fruit);
      mvprintw(9, 10, "PLAYER 3 : %d fruits", player[2].fruit);
    }
    else if(player[1].fruit > player[2].fruit) {
      mvprintw(5, 6, "PLAYER 2 IS THE WINNER!!!");
      mvprintw(7, 10, "PLAYER 1 : %d fruits", player[0].fruit);
      mvprintw(8, 10, "PLAYER 2 : %d fruits", player[1].fruit);
      mvprintw(9, 10, "PLAYER 3 : %d fruits", player[2].fruit);
    }
    else {
      mvprintw(5, 6, "PLAYER 3 IS THE WINNER!!!");
      mvprintw(7, 10, "PLAYER 1 : %d fruits", player[0].fruit);
      mvprintw(8, 10, "PLAYER 2 : %d fruits", player[1].fruit);
      mvprintw(9, 10, "PLAYER 3 : %d fruits", player[2].fruit);
    }
  }
  else {
    mvprintw(5, 6, "-- GAME OVER --");
  }
  mvprintw(20, 50, "Press Any Key...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  getch();
}

void ShowHelp() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  mvprintw(5, 6, "INSTRUCTION");
  mvprintw(10, 6, "Move :  Arrow Key");
  mvprintw(12, 6, "Item :  Space Bar");
  mvprintw(14, 6, "Quit :  [Q] Key");
  mvprintw(10, 46, "PLAYER 1");
  mvprintw(11, 46, "PLAYER 2");
  mvprintw(12, 46, "PLAYER 3");
  mvprintw(13, 46, "ENEMY");
  mvprintw(14, 46, "FRUIT (collect them!)");
  mvprintw(15, 46, "ITEM");
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(player0_color));
  mvaddch(10, 40, player0_ch);
  attroff(player0_color);
  attron(COLOR_PAIR(player1_color));
  mvaddch(11, 40, player1_ch);
  attroff(player1_color);
  attron(COLOR_PAIR(player2_color));
  mvaddch(12, 40, player2_ch);
  attroff(player2_color);
  attron(COLOR_PAIR(enemy_color));
  mvaddch(13, 40, enemy_ch);
  attroff(enemy_color);
  attron(COLOR_PAIR(fruit_color));
  mvaddch(14, 40, fruit_ch);
  attroff(fruit_color);
  attron(COLOR_PAIR(item_color));
  mvaddch(15, 40, item_ch);
  attroff(item_color);
  attron(COLOR_PAIR(3));
  mvprintw(20, 50, "Press Any Key...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  getch();
}

// 키 입력
void Input() {
  if(kbhit()) {
    int input;
    input = getch();
    if(input == KEY_UP) {
      player[0].dir = NORTH;
    }
    else if(input == KEY_DOWN) {
      player[0].dir = SOUTH;
    }
    else if(input == KEY_LEFT) {
      player[0].dir = WEST;
    }
    else if(input == KEY_RIGHT) {
      player[0].dir = EAST;
    }
    else if(input == ' ') {
      player[0].item_use = 1;
    }
    else if(input == 'w') {
      player[1].dir = NORTH;
    }
    else if(input == 's') {
      player[1].dir = SOUTH;
    }
    else if(input == 'a') {
      player[1].dir = WEST;
    }
    else if(input == 'd') {
      player[1].dir = EAST;
    }
    else if(input == 'e') {
      player[1].item_use = 1;
    }
    else if(input == '8') {
      player[2].dir = NORTH;
    }
    else if(input == '2') {
      player[2].dir = SOUTH;
    }
    else if(input == '4') {
      player[2].dir = WEST;
    }
    else if(input == '6') {
      player[2].dir = EAST;
    }
    else if(input == '5') {
      player[2].item_use = 1;
    }
    else if(input == 'q') {
      game_quit = 1;
    }
    else {
    }
  }
}

// 출력
void Draw() {
  if(!(refresh_object || refresh_message || refresh_status || refresh_time)) {
    return;
  }
  if(refresh_object) {
    refresh_object = 0;
    DrawFloor(map_id);
    DrawObjects();
  }
  if(refresh_message) {
    refresh_message = 0;
    DrawMessage();
  }
  if(refresh_status) {
    refresh_status = 0;
    DrawStatus();
  }
  if(refresh_time) {
    refresh_time = 0;
    DrawTime();
  }
  mvaddch(22, 78, ' ');
  refresh();
}

// ncurse 초기화 함수
void InitCurse() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
}

// ncurse color 초기화 함수
void InitColor() {
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  init_pair(8, COLOR_WHITE, COLOR_BLACK);
  init_pair(9, COLOR_BLACK, COLOR_WHITE);
  init_pair(10, COLOR_RED, COLOR_WHITE);
  init_pair(11, COLOR_GREEN, COLOR_WHITE);
  init_pair(12, COLOR_YELLOW, COLOR_WHITE);
  init_pair(13, COLOR_BLUE, COLOR_WHITE);
  init_pair(14, COLOR_MAGENTA, COLOR_WHITE);
  init_pair(15, COLOR_CYAN, COLOR_WHITE);
  init_pair(16, COLOR_WHITE, COLOR_WHITE);
  init_pair(17, COLOR_BLACK, COLOR_BLACK);
  init_pair(18, COLOR_RED, COLOR_RED);
  init_pair(19, COLOR_GREEN, COLOR_GREEN);
  init_pair(20, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(21, COLOR_BLUE, COLOR_BLUE);
  init_pair(22, COLOR_MAGENTA, COLOR_MAGENTA);
  init_pair(23, COLOR_CYAN, COLOR_CYAN);
  init_pair(24, COLOR_WHITE, COLOR_WHITE);
}

// 좌표 비교 함수
int IsSamePos(struct Position* pos1, struct Position* pos2) {
  if(pos1->x == pos2->x && pos1->y == pos2->y)
    return 1;
  else
    return 0;
}

// 충돌 검사 함수
enum ObjectType CheckCollision(struct Position* pos) {
  int i;
  if(map[map_id][pos->y][pos->x] == '#') {
    return WALL;
  }
  else {
    for(i = 0; i<40; ++i) {
      if(IsSamePos(&effect[i].pos, pos) && effect[i].state != DEAD)
        return EFFECT;
    }
    for(i = 0; i<number_of_enemies; ++i) {
      if(IsSamePos(&enemy[i].pos, pos) && enemy[i].state != DEAD)
        return ENEMY;
    }
    for(i = 0; i<number_of_players; ++i) {
      if(IsSamePos(&player[i].pos, pos) && player[i].state != DEAD)
        return PLAYER;
    }
    for(i = 0; i<number_of_fruits; ++i) {
      if(IsSamePos(&fruit[i].pos, pos) && fruit[i].state != DEAD)
        return FRUIT;
    }
    for(i = 0; i<number_of_items; ++i) {
      if(IsSamePos(&item[i].pos, pos) && item[i].state != DEAD)
        return ITEM;
    }
    return FLOOR;
  }
  return 0;
}

// 임의 위치 선택 함수
struct Position PickARandPos() {
  struct Position pos;
  while(1) {
    pos.x = rand()%columns_of_map;
    pos.y = rand()%rows_of_map;
    if(CheckCollision(&pos) == FLOOR)
      return pos;
  }
}

// 개체 이동 함수
void Move(struct Position* pos, enum Direction dir) {
  switch(dir) {
    case(EAST):
      pos->x++;
      if(pos->x==columns_of_map)
        pos->x = 0;
      break;
    case(WEST):
      pos->x--;
      if(pos->x==-1)
        pos->x = columns_of_map-1;
      break;
    case(SOUTH):
      pos->y++;
      if(pos->y==rows_of_map)
        pos->y = 0;
      break;
    case(NORTH):
      pos->y--;
      if(pos->y==-1)
        pos->y = rows_of_map-1;
      break;
    default:
      break;
  }
}

// 이동대상 지점 개체 검사 함수
enum ObjectType TryMove(struct Position pos, enum Direction dir) {
  Move(&pos, dir);
  return CheckCollision(&pos);
}

// 대상 좌표의 과일 검사
int FindFruitByPos(struct Position pos) {
  int i;
  for(i = 0; i<number_of_fruits; ++i) {
    if(fruit[i].pos.x == pos.x && fruit[i].pos.y == pos.y)
      return i;
  }
  return -1;
}

// 대상 좌표의 적 검사
int FindEnemyByPos(struct Position pos) {
  int i;
  for(i = 0; i<number_of_enemies; ++i) {
    if(enemy[i].pos.x == pos.x && enemy[i].pos.y == pos.y)
      return i;
  }
  return -1;
}

// 대상 좌표의 아이템 검사
int FindItemByPos(struct Position pos) {
  int i;
  for(i = 0; i<number_of_items; ++i) {
    if(item[i].pos.x == pos.x && item[i].pos.y == pos.y)
      return i;
  }
  return -1;
}

// 대상 좌표의 이펙트 검사
int FindEffectByPos(struct Position pos) {
  int i;
  for(i = 0; i<10; ++i) {
    if(effect[i].state == NORMAL &&
       effect[i].pos.x == pos.x && effect[i].pos.y == pos.y)
      return i;
  }
  return -1;
}

// 지도 출력 함수
void DrawMap(const int map_id) {
  int i, j;
  for(i = 0; i<rows_of_map; ++i) {
    for(j = 0; j<columns_of_map; ++j) {
      if(map[map_id][i][j] == '#') {
        attron(COLOR_PAIR(wall_color));
        mvaddch(i, j, wall_ch);
        attroff(COLOR_PAIR(wall_color));
      }
      else {
        attron(COLOR_PAIR(floor_color));
        mvaddch(i, j, floor_ch);
        attroff(COLOR_PAIR(floor_color));
      }
    }
  }
}

// 바닥 출력 함수
void DrawFloor(const int map_id) {
  int i, j;
  for(i = 0; i<rows_of_map; ++i) {
    for(j = 0; j<columns_of_map; ++j) {
      if(map[map_id][i][j] != '#') {
        attron(COLOR_PAIR(floor_color));
        mvaddch(i, j, floor_ch);
        attroff(COLOR_PAIR(floor_color));
      }
    }
  }
}

// 메시지 출력 함수
void DrawMessage() {
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  mvprintw(20, 0, " ======== Messages ======== ");
  attroff(A_BOLD);
  attroff(COLOR_PAIR(3));
  attron(COLOR_PAIR(8));
  mvprintw(21, 0, "PLAYER 1: What's this? ");
  mvprintw(22, 0, "PLAYER 2: The messages from players show here.");
  mvprintw(23, 0, "PLAYER 3: So, we can chat here?");
  mvprintw(24, 0, "PLAYER 2: Yes. But it's under construction.");
  attroff(COLOR_PAIR(8));
}

// 게임 정보 출력 함수
void DrawStatusFrame() {
  attron(A_BOLD);
  attron(COLOR_PAIR(player0_color));
  mvprintw(1, 61, "==== PLAYER 1 ====");
  mvprintw(2, 62, "LIFE");
  mvprintw(3, 62, "FRUIT");
  mvprintw(4, 62, "ITEM");
  attroff(COLOR_PAIR(player0_color));
  attron(COLOR_PAIR(player1_color));
  mvprintw(7, 61, "==== PLAYER 2 ====");
  mvprintw(8, 62, "LIFE");
  mvprintw(9, 62, "FRUIT");
  mvprintw(10, 62, "ITEM");
  attroff(COLOR_PAIR(player1_color));
  attron(COLOR_PAIR(player2_color));
  mvprintw(13, 61, "==== PLAYER 3 ====");
  mvprintw(14, 62, "LIFE");
  mvprintw(15, 62, "FRUIT");
  mvprintw(16, 62, "ITEM");
  attroff(COLOR_PAIR(player2_color));
  attroff(A_BOLD);
}

// 플레이어 정보 출력(갱신) 함수
void DrawStatus() {
  int i;
  attron(A_BOLD);
  attron(COLOR_PAIR(player0_color));
  if(player[0].life < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[0].life)
      mvaddch(2, 69+i, player0_ch);
    else
      mvaddch(2, 69+i, ' ');
  }
  else {
    mvprintw(2, 69, "          ");
    mvprintw(2, 69, "%d", player[0].life);
  }
  if(player[0].fruit < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[0].fruit)
      mvaddch(3, 69+i, fruit_ch);
    else
      mvaddch(3, 69+i, ' ');
  }
  else {
    mvprintw(3, 69, "          ");
    mvprintw(3, 69, "%d", player[0].fruit);
  }
  if(player[0].item == NOTHING) {
    mvprintw(4, 69, "          ");
  }
  else if(player[0].item == KNIFE) {
    mvprintw(4, 69, "knife   ");
  }
  else if(player[0].item == BOMB) {
    mvprintw(4, 69, "bomb    ");
  }
  else if(player[0].item == BOOSTER) {
    mvprintw(4, 69, "booster ");
  }
  else if(player[0].item == FREEZER) {
    mvprintw(4, 69, "freezer ");
  }
  else {
  }
  attroff(player0_color);
  attron(COLOR_PAIR(player1_color));
  if(player[1].life < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[1].life)
      mvaddch(8, 69+i, player1_ch);
    else
      mvaddch(8, 69+i, ' ');
  }
  else {
    mvprintw(8, 69, "          ");
    mvprintw(8, 69, "%d", player[1].life);
  }
  if(player[1].fruit < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[1].fruit)
      mvaddch(9, 69+i, fruit_ch);
    else
      mvaddch(9, 69+i, ' ');
  }
  else {
    mvprintw(9, 69, "          ");
    mvprintw(9, 69, "%d", player[1].fruit);
  }
  if(player[1].item == NOTHING) {
    mvprintw(10, 69, "          ");
  }
  else if(player[1].item == KNIFE) {
    mvprintw(10, 69, "knife   ");
  }
  else if(player[1].item == BOMB) {
    mvprintw(10, 69, "bomb    ");
  }
  else if(player[1].item == BOOSTER) {
    mvprintw(10, 69, "booster ");
  }
  else if(player[1].item == FREEZER) {
    mvprintw(10, 69, "freezer ");
  }
  else {
  }
  attroff(COLOR_PAIR(player1_color));
  attron(COLOR_PAIR(player2_color));
  if(player[2].life < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[2].life)
      mvaddch(14, 69+i, player2_ch);
    else
      mvaddch(14, 69+i, ' ');
  }
  else {
    mvprintw(14, 69, "          ");
    mvprintw(14, 69, "%d", player[2].life);
  }
  if(player[2].fruit < 10) {
    for(i = 0; i<10; ++i)
    if(i<player[2].fruit)
      mvaddch(15, 69+i, fruit_ch);
    else
      mvaddch(15, 69+i, ' ');
  }
  else {
    mvprintw(15, 69, "          ");
    mvprintw(15, 69, "%d", player[2].fruit);
  }
  if(player[2].item == NOTHING) {
    mvprintw(16, 69, "          ");
  }
  else if(player[2].item == KNIFE) {
    mvprintw(16, 69, "knife   ");
  }
  else if(player[2].item == BOMB) {
    mvprintw(16, 69, "bomb    ");
  }
  else if(player[2].item == BOOSTER) {
    mvprintw(16, 69, "booster ");
  }
  else if(player[2].item == FREEZER) {
    mvprintw(16, 69, "freezer ");
  }
  else {
  }
  attroff(COLOR_PAIR(player2_color));
  attroff(A_BOLD);
}

// 남은 시간 출력 함수
void DrawTime() {
  attron(A_BOLD);
  attron(COLOR_PAIR(2));
  mvprintw(19, 62, "TIME : %4d", (time_limit-(clock()-start_time))/1000);
  attroff(A_BOLD);
  attroff(COLOR_PAIR(2));
}

// 개체 출력 함수
void DrawObjects() {
  int i;
  attron(A_BOLD);
  attron(COLOR_PAIR(fruit_color));
  for(i = 0; i<number_of_fruits; ++i) {
    if(fruit[i].state != DEAD)
      mvaddch(fruit[i].pos.y, fruit[i].pos.x, fruit_ch);
  }
  attroff(COLOR_PAIR(fruit_color));
  attron(COLOR_PAIR(item_color));
  for(i = 0; i<number_of_items; ++i) {
    if(item[i].state != DEAD)
      mvaddch(item[i].pos.y, item[i].pos.x, item_ch);
  }
  attroff(COLOR_PAIR(item_color));
  if(player[0].state != DEAD) {
    attron(COLOR_PAIR(player0_color));
    mvaddch(player[0].pos.y, player[0].pos.x, player0_ch);
    attroff(COLOR_PAIR(player0_color));
  }
  if(player[1].state != DEAD) {
    attron(COLOR_PAIR(player1_color));
    mvaddch(player[1].pos.y, player[1].pos.x, player1_ch);
    attroff(COLOR_PAIR(player1_color));
  }
  if(player[2].state != DEAD) {
    attron(COLOR_PAIR(player2_color));
    mvaddch(player[2].pos.y, player[2].pos.x, player2_ch);
    attroff(COLOR_PAIR(player2_color));
  }
  attron(COLOR_PAIR(enemy_color));
  for(i = 0; i<number_of_enemies; ++i) {
    if(enemy[i].state != DEAD)
      mvaddch(enemy[i].pos.y, enemy[i].pos.x, enemy_ch);
  }
  attroff(COLOR_PAIR(enemy_color));
  attron(COLOR_PAIR(effect_color));
  for(i = 0; i<10; ++i) {
    if(bomb[i].state != DEAD)
      mvprintw(bomb[i].pos.y, bomb[i].pos.x, "%d", ((((clock()-bomb[i].tick)/1000)-3)*-1));
  }
  for(i = 0; i<40; ++i) {
    if(effect[i].state != DEAD)
      mvaddch(effect[i].pos.y, effect[i].pos.x, effect_ch);
  }
  attroff(COLOR_PAIR(effect_color));
  attroff(A_BOLD);
}

// 게임 데이터 초기화 함수
void PrepareGame() {
  srand(time(0)*clock());
  map_id = rand()%number_of_maps;
  int i;
  for(i = 0; i<number_of_players; ++i) {
    player[i].life = 5;
    player[i].fruit = 0;
    player[i].state = NORMAL;
    player[i].item = NOTHING;
    player[i].tick = 0;
    player[i].pos = PickARandPos();
    player[i].dir = STOP;
    player[i].item_use = 0;
  }
  for(i = 0; i<number_of_enemies; ++i) {
    enemy[i].state = NORMAL;
    enemy[i].tick = 0;
    enemy[i].pos = PickARandPos();
    enemy[i].dir = rand()%4+1;
  }
  for(i = 0; i<number_of_fruits; ++i) {
    fruit[i].pos = PickARandPos();
    fruit[i].state = NORMAL;
  }
  for(i = 0; i<number_of_items; ++i) {
    item[i].pos = PickARandPos();
    item[i].state = NORMAL;
  }
  for(i = 0; i<10; ++i) {
    bomb[i].state = DEAD;
    bomb[i].dir = STOP;
  }
  for(i = 0; i<40; ++i) {
    effect[i].state = DEAD;
    effect[i].dir = STOP;
  }
  game_quit = 0;
  game_win = 0;
  start_time = clock();
  next_second = clock()*2;
  fruit_remain = number_of_fruits;

  clear();
  refresh_object = 1;
  refresh_time = 1;
  refresh_message = 1;
  refresh_status = 1;
  DrawStatusFrame();
  DrawMap(map_id);
}

// 플레이어 체력 감소 함수
void DamagePlayer(int id) {
  if(player[id].state == DEAD)
    return;
  player[id].life--;
  if(player[id].life < 1)
    player[id].state = DEAD;
  refresh_status = 1;
  refresh_object = 1;
}

// 적 사살 함수
void DamageEnemy(int id) {
  if(enemy[id].state == DEAD)
    return;
  enemy[id].state = DEAD;
  refresh_object = 1;
}

// 게임 종료조건 검사 함수
void UpdateGameQuit() {
  // 시간 초과시 게임 종료
  if(clock() - start_time > time_limit)
    game_quit = 1;
  // 플레이어 전원 사망시 게임 종료
  else if(player[0].state == DEAD &&
     player[1].state == DEAD &&
     player[2].state == DEAD)
    game_quit = 1;
  // 과일 모두 먹었을 경우 게임 종료
  else if(fruit_remain <= 0) {
    game_quit = 1;
    game_win = 1;
  }
  else
    ;
}

// 게임 시간 갱신 함수
void UpdateTime() {
  if(next_second > clock()) {
    refresh_time = 1;
    next_second = clock()+1000;
  }
}

// 플레이어 이동 및 충돌 검사
void MovePlayer() {
  int i;
  for(i = 0; i<number_of_players; ++i) {
    if(player[i].state == DEAD)
      continue;
    if(player[i].state == FROZEN)
      continue;
    int speed = speed_normal;
    if(player[i].state == FAST) {
      speed = speed_fast;
      if(player[i].state_end <= clock())
        player[i].state = NORMAL;
    }
    else if(player[i].state == SLOW) {
      speed = speed_slow;
    }
    else {
    }
    if(clock() - player[i].tick > speed) {
      player[i].tick = clock();
      enum ObjectType target = TryMove(player[i].pos, player[i].dir);
      if(target == WALL) {
      }
      else if(target == ENEMY || target == EFFECT) {
        Move(&player[i].pos, player[i].dir);
        DamagePlayer(i);
      }
      else if(target == FRUIT) {
        Move(&player[i].pos, player[i].dir);
        int fruit_id = FindFruitByPos(player[i].pos);
        if(!(fruit_id == -1)) {
          fruit[fruit_id].state = DEAD;
          player[i].fruit++;
          fruit_remain--;
          refresh_status = 1;
          refresh_object = 1;
        }
      }
      else if(target == ITEM) {
        Move(&player[i].pos, player[i].dir);
        int item_id = FindItemByPos(player[i].pos);
        if(!(item_id == -1)) {
          item[item_id].state = DEAD;
          player[i].item = rand()%4+1;
          refresh_status = 1;
          refresh_object = 1;
        }
      }
      else {
        Move(&player[i].pos, player[i].dir);
        refresh_object = 1;
      }
    }
  }
}

// 적 이동 및 충돌 검사
void MoveEnemy() {
  int i;
  for(i = 0; i<number_of_enemies; ++i) {
    if(enemy[i].state == DEAD)
      continue;
    if(enemy[i].state == FROZEN) {
      if(enemy[i].state_end <= clock()) {
        enemy[i].state = SLOW;
        enemy[i].state_end = clock()+5000;
      }
      else {
        continue;
      }
    }
    int speed = speed_normal;
    if(enemy[i].state == FAST) {
      speed = speed_fast;
    }
    else if(enemy[i].state == SLOW) {
      speed = speed_slow;
      if(enemy[i].state_end <= clock())
        enemy[i].state = NORMAL;
    }
    else {
    }
    if(clock() - enemy[i].tick > speed) {
      enemy[i].tick = clock();
      enum ObjectType target = TryMove(enemy[i].pos, enemy[i].dir);
      if(target == WALL) {
        enemy[i].dir = rand()%4+1;
        enemy[i].tick -= 1000;
      }
      else if(target == PLAYER) {
        Move(&enemy[i].pos, enemy[i].dir);
        int j;
        for(j = 0; j<3; ++j) {
          if(IsSamePos(&player[j].pos, &enemy[i].pos))
            DamagePlayer(j);
        }
      }
      else if(target == EFFECT) {
        DamageEnemy(i);
      }
      else {
        Move(&enemy[i].pos, enemy[i].dir);
        refresh_object = 1;
      }
    }
  }
}

// 활성 아이템 오브젝트 처리 (이텍트 == 칼)
void MoveEffect() {
  int i;
  for(i = 0; i<40; ++i) {
    if(effect[i].state == DEAD)
      continue;
    if(clock() - effect[i].tick > speed_fastest) {
      effect[i].tick = clock();
      enum ObjectType target = TryMove(effect[i].pos, effect[i].dir);
      if(target == WALL) {
        effect[i].state = DEAD;
        effect[i].dir = STOP;
      }
      else if(target == PLAYER) {
        Move(&effect[i].pos, effect[i].dir);
        int j;
        for(j = 0; j<3; ++j) {
          if(IsSamePos(&player[j].pos, &effect[i].pos))
            DamagePlayer(j);
        }
      }
      else if(target == ENEMY) {
        Move(&effect[i].pos, effect[i].dir);
        int j;
        for(j = 0; j<number_of_enemies; ++j) {
          if(IsSamePos(&enemy[j].pos, &effect[i].pos))
            DamageEnemy(j);
        }
      }
      else {
        Move(&effect[i].pos, effect[i].dir);
        refresh_object = 1;
      }
    }
  }
}

// 활성 아이템 오브젝트 처리 (폭탄)
void MoveBomb() {
  int i, j;
  for(i = 0; i<10; ++i) {
    if(bomb[i].state == DEAD)
      continue;
    if(clock() - bomb[i].tick > 3000) {
      bomb[i].state = DEAD;
      refresh_object = 1;
      for(j = 0; j<3; ++j) {
        if(IsSamePos(&player[j].pos, &bomb[i].pos))
          DamagePlayer(j);
      }
      for(j = 0; j<number_of_enemies; ++j) {
        if(IsSamePos(&enemy[j].pos, &bomb[i].pos))
          DamagePlayer(j);
      }
      for(j = 0; j<40; ++j) {
        if(effect[j].state == DEAD) {
          effect[j].state = NORMAL;
          effect[j].pos = bomb[i].pos;
          effect[j].dir = EAST;
          effect[j].tick = clock();
          break;
        }
      }
      for(j = 0; j<40; ++j) {
        if(effect[j].state == DEAD) {
          effect[j].state = NORMAL;
          effect[j].pos = bomb[i].pos;
          effect[j].dir = WEST;
          effect[j].tick = clock();
          break;
        }
      }
      for(j = 0; j<40; ++j) {
        if(effect[j].state == DEAD) {
          effect[j].state = NORMAL;
          effect[j].pos = bomb[i].pos;
          effect[j].dir = SOUTH;
          effect[j].tick = clock();
          break;
        }
      }
      for(j = 0; j<40; ++j) {
        if(effect[j].state == DEAD) {
          effect[j].state = NORMAL;
          effect[j].pos = bomb[i].pos;
          effect[j].dir = NORTH;
          effect[j].tick = clock();
          break;
        }
      }
    }
  }
}

// 아이템 사용 처리 함수
void UseItem() {
  int i, j;
  // 플레이어 아이템 사용 처리
  for(i = 0; i<3; ++i) {
    if(player[i].item_use) {
      refresh_status = 1;
      player[i].item_use = 0;
      switch(player[i].item) {
        case KNIFE:
          for(j = 0; j<40; ++j) {
            if(effect[j].state == DEAD) {
              effect[j].state = NORMAL;
              effect[j].pos = player[i].pos;
              effect[j].dir = player[i].dir;
              Move(&effect[j].pos, effect[j].dir);
              effect[j].tick = clock();
              break;
            }
          }
          player[i].item = NOTHING;
          break;

        case BOMB:
          for(j = 0; j<10; ++j) {
            if(bomb[j].state == DEAD) {
              bomb[j].state = NORMAL;
              bomb[j].pos = player[i].pos;
              bomb[j].dir = player[i].dir;
              bomb[j].tick = clock();
              break;
            }
          }
          player[i].item = NOTHING;
          break;

        case BOOSTER:
          player[i].state = FAST;
          player[i].state_end = clock()+20000;
          player[i].item = NOTHING;
          break;

        case FREEZER:
          for(j = 0; j<number_of_enemies; ++j) {
            if(enemy[j].state != DEAD)
              enemy[j].state = FROZEN;
            enemy[j].state_end = clock()+10000+rand()%5000;
          }
          player[i].item = NOTHING;
          break;

        case NOTHING:
        default:
          break;
      }
    }
  }
}
