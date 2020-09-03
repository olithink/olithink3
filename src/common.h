#include<stdio.h> 
#include<stdlib.h> 
#include<string.h> 
#include<signal.h>

#ifdef WIN32
#define strncasecmp strnicmp
#endif
#ifdef WIN32
typedef __int64 vlong;
#else
typedef long long vlong;
#endif

#ifndef GCC
#define inline
#endif

#define DEFAULTMB_HASH 	4
#define P_BRAIN		1
#define B_DIM		64
#define MAX_MOVES	200
#define MAX_PIECES	64
#define MAX_STACK	900
#define MAX_DEPTH	64
#define MAX_PLY 	128
#define MAX_KILL	1
#define MAX_BOOK	3000
#define CHECK_NODES	1000
#define AB_WINDOW       60
#define V_DRAW          100
#define T_NUMBER	20
#define INTERFACE	1

#define EMPTY		0
#define W_PAWN		1
#define W_KNIGHT	2
#define W_BISHOP	3
#define W_ROOK		4
#define W_QUEEN		5
#define W_KING		6
#define FIGURE_OFFSET	6

#define B_PAWN		-W_PAWN
#define B_KNIGHT	-W_KNIGHT
#define B_BISHOP	-W_BISHOP
#define B_ROOK		-W_ROOK
#define B_QUEEN		-W_QUEEN
#define B_KING		-W_KING

#define V_EMPTY		0
#define V_PAWN 		100
#define V_KNIGHT 	324
#define V_BISHOP 	325
#define V_ROOK 		480
#define V_QUEEN 	950
#define V_KING 		10000

#define C_WHITE 	1
#define C_BLACK		0
#define C_NONE		-1

#define F_NORMAL 	0
#define F_HIT		1
#define F_CHECK		2
#define F_PAWN		4
#define F_PAWN_HIT	5
#define F_FILTER_EP	8
#define F_ENPASSANT	9
#define F_PAWNDOUBLE	16
#define F_PROMOTE_N	32
#define F_PROMOTE_R	64
#define F_PROMOTE_B	128
#define F_PROMOTE_Q	256
#define F_PROMOTE_K	512
#define F_CASTLE_WSHORT	1024
#define F_CASTLE_WLONG	2048
#define F_CASTLE_BSHORT	4096
#define F_CASTLE_BLONG	8192
#define F_CASTLE_ALL	15360
#define F_PROMOTE_ALL   992
#define F_CHANGE_VALUE  993

#define STALE_MATE 	31111
#define MACHINE_MATED 	-29999
#define MACHINE_MATES 	29999
#define THREE_POS 	31113
#define FIFTY_MOVE 	31114

#define LOW_BOUND 0x001
#define UP_BOUND 0x002
#define EXACT_EVAL 0x004
#define AVOID_NULL 0x008
#define HASH_PV 0x010
#define PV_EXACT 0x014

#define NO_SMALL_CASTLE_W 1
#define NO_BIG_CASTLE_W 2
#define NO_CASTLE_W 3
#define NO_SMALL_CASTLE_B 4
#define NO_BIG_CASTLE_B 8
#define NO_CASTLE_B 12
#define CASTLE_W 16
#define CASTLE_B 32

#define second_line_white 8
#define fifth_line_white 32
#define seventh_line_white 48

#define second_line_black 48
#define fifth_line_black 24
#define seventh_line_black 8

typedef unsigned char nchar;
typedef signed char schar;
typedef unsigned short nshort;
typedef unsigned long nlong;

typedef struct {
  nchar from;
  nchar to;
  schar piece_from;
  schar piece_to;
  schar enpassant;
  schar promote;
  nshort flag;
} Moves;

typedef struct {
  char from;
  char to;
} Opening_Class;

typedef struct {
  int number;
  int fields[8];
} Piece_Class;

typedef struct {
  int index;
} Castle_Class;

typedef struct {
  vlong rec;
  signed short value;
  schar depth;
  nchar flag;
  Moves mv;
  nchar number; 
} Hash_Table;

extern Hash_Table *hash_table_w;
extern Hash_Table *hash_table_b;
extern vlong *hash_stack;
extern vlong hash_stack_init[MAX_STACK+1];
extern Moves *stack;
extern Moves stack_init[MAX_STACK+2];
extern Moves pv[MAX_PLY][MAX_PLY];
extern Moves killer[MAX_PLY][MAX_KILL];   
extern Moves moves[MAX_PLY][MAX_MOVES];
extern long moveval[MAX_PLY][MAX_MOVES];
extern Piece_Class knight_class[B_DIM];
extern Piece_Class king_class[B_DIM];
extern Piece_Class far_class[B_DIM][8];
extern Opening_Class opening[61][MAX_BOOK];
extern Castle_Class castle[MAX_STACK];
extern char piece_char[7];
extern char Piece_Char[7];
extern int pv_length[MAX_PLY];
extern int board[B_DIM];
extern int init_board[B_DIM];
extern int stack_size;
extern int last_stack_size;
extern int material;
extern int machine_color;
extern int on_move;
extern int ply;
extern int *piece_value;
extern int promote_piece[F_PROMOTE_K+1];
extern int draw;
extern int random_move;
extern int num_book;
extern int in_book;
extern int in_book_init;
extern int book_color;
extern int stop_thinking;
extern int actual_eval;
extern int show_pv;
extern int show_board;
extern int do_whisper;
extern int do_kibitz;
extern int search_depth;
extern int inc_time;
extern int low_fail;
extern int high_fail;
extern int high_new;
extern int init_time;
extern int opening_flag[MAX_BOOK];
extern vlong random_hash_index_init[B_DIM][14];
extern vlong random_hash_ep[10];
extern vlong random_hash_castle[16];
extern vlong *random_hash_index[B_DIM];
extern long history[B_DIM][B_DIM];
extern long nodes;
extern long start_time;
extern long end_time;
extern long rest_nodes;
extern long time_for_move;
extern int read_book_file;
extern char oponnent_name[60];
extern int my_rating;
extern int oponnent_rating;
extern int number_move_init[MAX_PLY+10];
extern int *number_move;
extern int hash_mask;
extern int have_kibitzed;
extern int do_ponder;
extern int pondering;
extern nlong mb_hash;
extern int king_pos[2];
extern int interface;
extern int piece_value_init[13];
extern int pawn_file[2][10];
extern int no_captures;
extern int chess_type;
extern int type;
extern int force_mode;
extern int ics_mode;
extern int op_time;
extern int my_time;
extern int ab_window;
extern int end_game;

extern int init_chess(void);
extern int parse_args(int,char **);
extern int generate_legal_moves(Moves*,long *,int);
extern int input_move(void);
extern int next_move(long*,int);
extern void TermSearch(int sig);
extern void display_board(void);
extern void display_pv(int,int);
extern void whisper_info(int);
extern void editboard(void);
extern void clearboard(void);
extern void move_after_book(void);
extern int execute_move(int);
extern void load_book(void);
extern void game_ends(int);
extern void setup_board(void);
extern void init_pos(void);
extern void enable_book_moves(void);
extern int parse_move(char*,Moves*);
extern int pgn_string(char*,Moves);
extern int e2e4_string(char*,Moves);
extern int think(void);
extern int check_for_endgame(void);
extern void init_eval(int);
extern int exit_chess(int);
extern int attacked(int,int);
extern void update_pv(Moves,int);
extern long get_time(void);
extern int look_up(vlong,int,int,int *,int *,int *);
extern void store_hash(vlong,int,int,int,int,int,Moves);
extern vlong calc_hash(void);
extern vlong update_hash(void);
extern void take_back(void);
extern void make_move(Moves);
extern int generate_moves(Moves*,long *,vlong,int);
extern int generate_captures(Moves*,long *,int);
extern int eval(void);
extern int quiesce(int,int);
extern int search(int,int,int);
extern int check_cont(void);
extern void sort_moves(int,Moves *,long *,vlong,int);
extern void sort_quiesce(int,Moves *,long *,int);
extern void init_knight_moves(void);
extern void init_king_moves(void);
extern void init_far_moves(void);
extern int bioskey(void);
extern char cmd_buf[512];
