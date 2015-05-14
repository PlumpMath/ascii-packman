// 최종업데이트: 2011-05-22
// 빌드: $ g++ ./packman_client.c -o ./packman_client -l ncurses
// 실행: $ ./packman_client

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "ncurses.h"

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
const char fruit_ch      = ',';
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

// 공유 메모리 관련 상수 및 변수
int       shm_id;
long int  shm_addr;
const int shm_key = 1053311968;

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

// 기타 전역 변수
int  player_id;
int  program_quit = 0;
int  get_chat = 0;
char chat_ch = '\0';
char chat_str[80] = "";
int  chat_str_len = 0;
int  chat_str_completed = 0;

// 함수들.
void PostChat();
void GetSharedMemoryAddress();
void CloseSharedMemory();
void TitleMenu();
void JoinGame();
void PlayGame();
void ShowWait();
void ShowFull();
void ShowResult();
void ShowHelp();
void WaitUntilPressSpace();
void Input();
void Draw();
void InitCurse();
void CloseCurse();
void PrepareColors();
void DrawMap(const int map_id);
void DrawFloor(const int map_id);
void DrawMessage();
void DrawStatusFrame();
void DrawStatus();
void DrawTime();
void DrawObjects();

// main
int main(int argc, char** argv) {
  GetSharedMemoryAddress();
  InitCurse();
  PrepareColors();
  TitleMenu();
  CloseCurse();
  CloseSharedMemory();
  return 0;
}

void PostChat() {
  if(get_chat) {
    chat_ch = getch();
    if(('a' <= chat_ch && chat_ch <= 'z') ||
       ('A' <= chat_ch && chat_ch <= 'Z') ||
       ('0' <= chat_ch && chat_ch <= '9') ||
       chat_ch == ' ' ||
       chat_ch == '.' ||
       chat_ch == ',' ||
       chat_ch == '?' ||
       chat_ch == '!' ||
       chat_ch == '\'') {
      chat_str[chat_str_len] = chat_ch;
      chat_str[++chat_str_len] = '\0';
      mvprintw(20, 0, "%-79s", chat_str);
    }
    if(chat_ch == '\b' || chat_ch == KEY_BACKSPACE) {
      if(chat_str_len >= 1)
        chat_str[--chat_str_len] = '\0';
    }
    if(chat_ch == '\n' || chat_str_len >= 70) {
      chat_str[chat_str_len] = '\0';
      chat_str_completed = 1;
    }
    if(chat_str_completed) {
      chat_str_completed = 0;
      get_chat = 0;
      chat_ch = '\0';
      chat_str_len = 0;
      mvprintw(20, 0, "%-79s", "");
      strcpy(post->context, chat_str);
      switch(player_id) {
        case 0:
          post->type = M_P1;
          break;
        case 1:
          post->type = M_P2;
          break;
        case 2:
          post->type = M_P3;
          break;
        default:
          ;
          break;
      }
    }
  }
}

void GetSharedMemoryAddress() {
  shm_id = shmget((key_t)shm_key, 0, 0);
  if(shm_id == -1)
  {
    perror("shmat failed : ");
    exit(0);
  }

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

void DrawTitleMenu() {
  clear();
  attron(A_BOLD);
  attron(A_STANDOUT);
  attron(COLOR_PAIR(3));
  mvprintw(5, 25, " A S C I I     P A C K - M A N ");
  attroff(A_STANDOUT);
  mvprintw(15, 25, "[J] JOIN GAME");
  mvprintw(16, 25, "[H] HELP");
  mvprintw(17, 25, "[Q] QUIT");
  mvprintw(13, 25, "Choose an option :");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
}

void TitleMenu() {
  char ch = '\0';
  DrawTitleMenu();
  while(!program_quit) {
    ch = getch();
    switch(ch) {
      case 'j':
      case 'J':
        JoinGame();
        DrawTitleMenu();
        break;
      case 'h':
      case 'H':
        ShowHelp();
        DrawTitleMenu();
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

void JoinGame() {
  if(*joined_player >= 3) {
    ShowFull();
  }
  else {
    player_id = *joined_player;
    (*joined_player)++;
    ShowWait();
    while(*game_state != PLAY) {
      sleep(1);
    }
    clear();
    DrawMap(*map_id);
    DrawStatusFrame();
    PlayGame();
    ShowResult();
    sleep(2);
  }
}

void ShowWait() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  mvprintw(5, 6, "YOU ARE PLAYER %d", player_id+1);
  mvprintw(20, 36, "Plaease wait until other players join...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  refresh();
}

void ShowFull() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  mvprintw(5, 6, "YOU CAN'T JOIN THE GAME -- TOO MANY PLAYERS ARE PLAYING");
  mvprintw(20, 42, "Press spacebar to continue...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  refresh();
  WaitUntilPressSpace();
}


// 게임 로직 처리
void PlayGame() {
  while(*game_state == PLAY) {
    Input();
    PostChat();
    Draw();
  }
}

void ShowResult() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  if(*game_state == WIN) {
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
  mvprintw(20, 42, "Press spacebar to continue...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  WaitUntilPressSpace();
}

void ShowHelp() {
  clear();
  attron(A_BOLD);
  attron(COLOR_PAIR(3));
  mvprintw(5, 6, "INSTRUCTION");
  mvprintw(10, 6, "Move     :  Arrow Keys");
  mvprintw(12, 6, "Use Item :  Spacebar");
  mvprintw(14, 6, "Suicide  :  Q Key");
  mvprintw(16, 6, "Chat     :  Enter Key");
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
  mvprintw(20, 42, "Press spacebar to continue...");
  attroff(COLOR_PAIR(3));
  attroff(A_BOLD);
  refresh();
  WaitUntilPressSpace();
}

void WaitUntilPressSpace() {
  while(getch() != ' ') {
    ;
  }
}

// 키 입력
void Input() {
  if(get_chat)
    return;
  int input;
  input = getch();
  if(input == KEY_UP) {
    player[player_id].dir = NORTH;
  }
  else if(input == KEY_DOWN) {
    player[player_id].dir = SOUTH;
  }
  else if(input == KEY_LEFT) {
    player[player_id].dir = WEST;
  }
  else if(input == KEY_RIGHT) {
    player[player_id].dir = EAST;
  }
  else if(input == ' ') {
    player[player_id].item_use = 1;
  }
  else if(input == 'q' || input == 'Q') {
    player[player_id].life = 0;
    player[player_id].state = DEAD;
    int k;
    for(k = 0; k<number_of_players; ++k) {
      player[k].refresh_object = 1;
      player[k].refresh_status = 1;
    }
  }
  else if(input == '\n' || input == KEY_ENTER) {
    get_chat = 1;
  }
  else if(input == '1') {
    strcpy(post->context, "TEST");
    switch(player_id) {
      case 0:
        post->type = M_P1;
        break;
      case 1:
        post->type = M_P2;
        break;
      case 2:
        post->type = M_P3;
        break;
    }
  }
  else {
    ;
  }
}

// 출력
void Draw() {
  if(!(player[player_id].refresh_object || 
       player[player_id].refresh_message || 
       player[player_id].refresh_status || 
       player[player_id].refresh_time)) {
    return;
  }
  if(player[player_id].refresh_object) {
    player[player_id].refresh_object = 0;
    DrawFloor(*map_id);
    DrawObjects();
  }
  if(player[player_id].refresh_message) {
    player[player_id].refresh_message = 0;
    DrawMessage();
  }
  if(player[player_id].refresh_status) {
    player[player_id].refresh_status = 0;
    DrawStatus();
  }
  if(player[player_id].refresh_time) {
    player[player_id].refresh_time = 0;
    DrawTime();
  }
  move(20, 79);
  refresh();
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

// ncurse color 초기화 함수
void PrepareColors() {
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
  attron(COLOR_PAIR(8));
  int i;
  for(i = 0; i<number_of_messages; ++i) {
    if(message[i].context[0] == '\0')
      continue;
    switch(message[i].type) {
      case M_SYS:
        mvprintw(19+number_of_messages-i, 0, "%-80s", "SERVER:");
        break;
      case M_P1:
        mvprintw(19+number_of_messages-i, 0, "%-80s", "PLAYER 1:");
        break;
      case M_P2:
        mvprintw(19+number_of_messages-i, 0, "%-80s", "PLAYER 2:");
        break;
      case M_P3:
        mvprintw(19+number_of_messages-i, 0, "%-80s", "PLAYER 3:");
        break;
      default:
        ;
        break;
    }
    mvprintw(19+number_of_messages-i, 10, message[i].context);
  }
  attroff(COLOR_PAIR(8));
  attroff(A_BOLD);
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
  mvprintw(19, 62, "TIME : %4d", *time_left);
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
    if(bomb[i].state != DEAD) {
      attron(A_STANDOUT);
      mvaddch(bomb[i].pos.y, bomb[i].pos.x, effect_ch);
      attroff(A_STANDOUT);
    }
  }
  for(i = 0; i<40; ++i) {
    if(effect[i].state != DEAD)
      mvaddch(effect[i].pos.y, effect[i].pos.x, effect_ch);
  }
  attroff(COLOR_PAIR(effect_color));
  attroff(A_BOLD);
}

