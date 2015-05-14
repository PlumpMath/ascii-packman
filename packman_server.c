// 최종업데이트: 2011-05-22
// 빌드: $ g++ ./packman_server.c -o ./packman_server -l ncurses -l pthread
// 실행: $ ./packman_server

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "ncurses.h"

// 맵 개수, 맵 크기
const int number_of_maps = 9;
const int columns_of_map = 60;
const int rows_of_map = 20;

// 맵 데이터
const char map[9][20][61] = {
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
enum GameState {
  WAIT = 0,
  PLAY = 1,
  WIN  = 2,
  LOSE = 3,
};

enum MessageType {
  M_SYS = 0,
  M_P1  = 1,
  M_P2  = 2,
  M_P3  = 3,
};

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
  int              ready;
  struct Position  pos;
  enum Direction   dir;
  enum State       state;
  int              life;
  int              fruit;
  enum ItemType    item;
  clock_t          tick;
  clock_t          state_end;
  int              item_use;
  int              refresh_status;
  int              refresh_message;
  int              refresh_object;
  int              refresh_time;
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

struct Message {
  enum MessageType type;
  char             context[81];
};

// 게임 옵션 상수
const int number_of_players = 3;
const int number_of_enemies = 13;
const int number_of_fruits = 31;
const int number_of_items = 10;
const int number_of_messages = 4;
const int time_limit = 120 * CLOCKS_PER_SEC;
const int speed_fastest = 0.03 * CLOCKS_PER_SEC;
const int speed_fast = 0.11 * CLOCKS_PER_SEC;
const int speed_normal = 0.19 * CLOCKS_PER_SEC;
const int speed_slow = 0.5 * CLOCKS_PER_SEC;

// 공유 메모리 관련 상수 및 변수
int       shm_id;
long int  shm_addr;
const int shm_key = 1053311968;
const int shm_size = sizeof(enum GameState)*1 +
                     sizeof(int)*3 +
                     sizeof(struct Player)*number_of_players +
                     sizeof(struct Enemy)*number_of_enemies +
                     sizeof(struct Fruit)*number_of_fruits +
                     sizeof(struct Item)*number_of_items +
                     sizeof(struct Effect)*10 +
                     sizeof(struct Effect)*40 +
                     sizeof(struct Message)*number_of_messages +
                     sizeof(struct Message)*1;

// 공유 데이터를 위한 포인터들 (구현의 편의를 위해 static var.)
enum GameState* game_state;
int*            joined_player;
int*            map_id;
int*            time_left;
struct Player*  player;
struct Enemy*   enemy;
struct Fruit*   fruit;
struct Item*    item;
struct Effect*  bomb;
struct Effect*  effect;
struct Message* message;
struct Message* post;

// 쓰레드 관련 변수
pthread_t thread_game;
pthread_t thread_chat;
long int  thread_game_arg = 1;
long int  thread_chat_arg = 2;
int thread_status;
int server_quit = 0;

// 기타 전역 변수
clock_t           start_time;
int               next_second;   // 초 계산
int               fruit_remain;


// 함수들.
void* thread_game_routine(void* arg);
void* thread_chat_routine(void* arg);
void InitCurse();
void CloseCurse();
void UpdateServerQuit();
void PrepareSharedMemory();
void CloseSharedMemory();
void WaitGame();
void PrepareGame();
void PlayGame();
int IsSamePos(struct Position* pos1, struct Position* pos2);
enum ObjectType CheckCollision(struct Position* pos);
struct Position PickARandPos();
enum Direction PickARandDir();
void Move(struct Position* pos, enum Direction dir);
enum ObjectType TryMove(struct Position pos, enum Direction dir);
int FindFruitByPos(struct Position pos);
int FindEnemyByPos(struct Position pos);
int FindItemByPos(struct Position pos);
int FindEffectByPos(struct Position pos);
void DamagePlayer(int id);
void DamageEnemy(int id);
void UpdateGameQuit();
void UpdateChat();
void UpdateTime();
void MovePlayer();
void MoveEnemy();
void MoveEffect();
void MoveBomb();
void UseItem();


// main
int main(int argc, char** argv) {
  srand(time(0));
  PrepareSharedMemory();
  InitCurse();
  pthread_create(&thread_game, NULL, &thread_game_routine, (void*) thread_game_arg);
  pthread_create(&thread_chat, NULL, &thread_chat_routine, (void*) thread_chat_arg);
  pthread_join(thread_game, (void**) &thread_status);
  pthread_join(thread_chat, (void**) &thread_status);
  CloseCurse();
  CloseSharedMemory();
  return 0;
}

// ncurse 초기화 함수
void InitCurse() {
  initscr();
  nodelay(stdscr, TRUE);
  start_color();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
}

void CloseCurse() {
  endwin();
}


void *thread_game_routine(void *arg) {
  while(!server_quit) {
    PrepareGame();
    clear();
    attron(A_BOLD);
    attron(A_STANDOUT);
    mvprintw(3, 20, "    PACK-MAN SERVER IS RUNNING    ");
    attroff(A_STANDOUT);
    mvprintw(6, 0, "Waiting for connection...");
    mvprintw(23, 49, "Press 'q' to close the server.");
    attroff(A_BOLD);
    WaitGame();
    clear();
    attron(A_BOLD);
    attron(A_STANDOUT);
    mvprintw(3, 20, "    PACK-MAN SERVER IS RUNNING    ");
    attroff(A_STANDOUT);
    mvprintw(6, 0, "Playing...");
    mvprintw(23, 49, "Press 'q' to close the server.");
    attroff(A_BOLD);
    PlayGame();
    sleep(1);
  }
  pthread_exit((void*) 0);
} 

void *thread_chat_routine(void *arg) {
  while(!server_quit) {
    UpdateChat();
    UpdateServerQuit();
  }
  pthread_exit((void*) 0);
}

void UpdateServerQuit() {
  char ch = getch();
  if(ch == 'q' || ch == 'Q') {
    server_quit = 1;
  }
}

void PrepareSharedMemory() {
  // 공유 메모리 할당 받기
  shm_id = shmget((key_t)shm_key, (size_t)shm_size, 0777 | IPC_CREAT);
  if(shm_id == -1)
  {
    perror("shmat failed : ");
    exit(0);
  }

// 할당 받은 메모리를 나눠, 각 변수에 주소 할당
  long int buffer;
  shm_addr = (long int)shmat(shm_id, NULL, 0);
  buffer = shm_addr;

  game_state = (enum GameState*)buffer;
  buffer += sizeof(enum GameState)*1;
  joined_player = (int*)buffer;
  buffer += sizeof(int)*1;
  map_id = (int*)buffer;
  buffer += sizeof(int)*1;
  time_left = (int*)buffer;
  buffer += sizeof(int)*1;
  player = (struct Player*)buffer;
  buffer += sizeof(struct Player)*number_of_players;
  enemy = (struct Enemy*)buffer;
  buffer += sizeof(struct Enemy)*number_of_enemies;
  fruit = (struct Fruit*)buffer;
  buffer += sizeof(struct Fruit)*number_of_fruits;
  item = (struct Item*)buffer;
  buffer += sizeof(struct Item)*number_of_items;
  bomb = (struct Effect*)buffer;
  buffer += sizeof(struct Effect)*10;
  effect = (struct Effect*)buffer;
  buffer += sizeof(struct Effect)*40;
  message = (struct Message*)buffer;
  buffer += sizeof(struct Message)*number_of_messages;
  post = (struct Message*)buffer;
  buffer += sizeof(struct Message)*1;

  printf("Shared memory prepared.\n");
}

void CloseSharedMemory() {
  shmdt((void*)shm_addr);
  printf("Shared memory closed.\n");
}

void WaitGame() {
  char ch = 0;
  *joined_player = 0;
  while(*joined_player < 3 && !server_quit) {
    attron(A_BOLD);
    mvprintw(7, 0, "joined player : %d", *joined_player);
    attroff(A_BOLD);
    sleep(1);
  }
  *game_state = PLAY;
}

// 게임 로직 처리
void PlayGame() {
  while(*game_state == PLAY && !server_quit) {
    UpdateGameQuit();
    UpdateTime();
    MovePlayer();
    UseItem();
    MoveEnemy();
    MoveEffect();
    MoveBomb();
  }
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
  if(map[*map_id][pos->y][pos->x] == '#') {
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

enum Direction PickARandDir() {
  int i = rand()%4+1;
  if(i == 1)
    return EAST;
  else if(i == 2)
    return WEST;
  else if(i == 3)
    return SOUTH;
  else
    return NORTH;
}

enum ItemType PickARandItem() {
  int i = rand()%4+1;
  if(i == 1)
    return KNIFE;
  else if(i == 2)
    return BOMB;
  else if(i == 3)
    return BOOSTER;
  else
    return FREEZER;
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

// 게임 데이터 초기화 함수
void PrepareGame() {
  *map_id = rand()%number_of_maps;
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
    enemy[i].dir = PickARandDir();
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
  start_time = clock();
  next_second = clock()*2;
  fruit_remain = number_of_fruits;
  int k;
  for(k = 0; k<number_of_players; ++k) {
    player[k].refresh_object = 1;
    player[k].refresh_time = 1;
    player[k].refresh_message = 1;
    player[k].refresh_status = 1;
  }
  *game_state = WAIT;
  for(i = 0; i<number_of_messages; ++i) {
    message[i].type = M_SYS;
    message[i].context[0] = '\0';
  }
  post->type = M_SYS;
  post->context[0] = '\0';
}

// 플레이어 체력 감소 함수
void DamagePlayer(int id) {
  if(player[id].state == DEAD)
    return;
  player[id].life--;
  if(player[id].life < 1)
    player[id].state = DEAD;
  int k;
  for(k = 0; k<number_of_players; ++k) {
    player[k].refresh_object = 1;
    player[k].refresh_status = 1;
  }
}

// 적 사살 함수
void DamageEnemy(int id) {
  if(enemy[id].state == DEAD)
    return;
  enemy[id].state = DEAD;
  int k;
  for(k = 0; k<number_of_players; ++k) {
    player[k].refresh_object = 1;
  }
}

// 게임 종료조건 검사 함수
void UpdateGameQuit() {
  // 시간 초과시 게임 종료
  if(clock() - start_time > time_limit)
    *game_state = LOSE;
  // 플레이어 전원 사망시 게임 종료
  else if(player[0].state == DEAD &&
          player[1].state == DEAD &&
          player[2].state == DEAD)
    *game_state = LOSE;
  // 과일 모두 먹었을 경우 게임 종료
  else if(fruit_remain <= 0)
    *game_state = WIN;
  else
    ;
}

void UpdateChat() {
  if(post->context[0]) {
    attron(A_BOLD);
    mvprintw(7, 0, "%-80s", "");
    mvprintw(8, 0, "%-80s", "");
    mvprintw(7, 0, "Last Message : %s", post->context);
    attroff(A_BOLD);
    int i;
    for(i = number_of_messages-1; i>0; --i) {
      message[i] = message[i-1];
    }
    message[0] = *post;
    post->context[0] = '\0';
    for(i = 0; i<number_of_players; ++i) {
      player[i].refresh_message = 1;
    }
  }
}

// 게임 시간 갱신 함수
void UpdateTime() {
  if(next_second < clock()) {
    int k;
    for(k = 0; k<number_of_players; ++k) {
      player[k].refresh_time = 1;
    }
    *time_left = (time_limit-(clock()-start_time))/CLOCKS_PER_SEC;
    next_second = clock()+CLOCKS_PER_SEC;
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
          int k;
          for(k = 0; k<number_of_players; ++k) {
            player[k].refresh_object = 1;
            player[k].refresh_status = 1;
          }
        }
      }
      else if(target == ITEM) {
        Move(&player[i].pos, player[i].dir);
        int item_id = FindItemByPos(player[i].pos);
        if(!(item_id == -1)) {
          item[item_id].state = DEAD;
          player[i].item = PickARandItem();
          int k;
          for(k = 0; k<number_of_players; ++k) {
            player[k].refresh_object = 1;
            player[k].refresh_status = 1;
          }
        }
      }
      else {
        Move(&player[i].pos, player[i].dir);
        int k;
        for(k = 0; k<number_of_players; ++k) {
          player[k].refresh_object = 1;
        }
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
        enemy[i].state_end = clock()+5*CLOCKS_PER_SEC;
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
        enemy[i].dir = PickARandDir();
        enemy[i].tick -= CLOCKS_PER_SEC;
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
        int k;
        for(k = 0; k<number_of_players; ++k) {
          player[k].refresh_object = 1;
        }
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
        int k;
        for(k = 0; k<number_of_players; ++k) {
          player[k].refresh_object = 1;
        }
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
    if(clock() - bomb[i].tick > 3*CLOCKS_PER_SEC) {
      bomb[i].state = DEAD;
      int k;
      for(k = 0; k<number_of_players; ++k) {
        player[k].refresh_object = 1;
      }
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
      int k;
      for(k = 0; k<number_of_players; ++k) {
        player[k].refresh_status = 1;
      }
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
          player[i].state_end = clock()+20*CLOCKS_PER_SEC;
          player[i].item = NOTHING;
          break;

        case FREEZER:
          for(j = 0; j<number_of_enemies; ++j) {
            if(enemy[j].state != DEAD)
              enemy[j].state = FROZEN;
            enemy[j].state_end = clock()+10*CLOCKS_PER_SEC+rand()%(5*CLOCKS_PER_SEC);
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
