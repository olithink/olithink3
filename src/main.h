int init_board[B_DIM]={
W_ROOK ,W_KNIGHT ,W_BISHOP ,W_QUEEN ,W_KING ,W_BISHOP ,W_KNIGHT ,W_ROOK,
W_PAWN ,W_PAWN 	 ,W_PAWN   ,W_PAWN  ,W_PAWN ,W_PAWN   ,W_PAWN   ,W_PAWN,
EMPTY  ,EMPTY    ,EMPTY    ,EMPTY   ,EMPTY  ,EMPTY    ,EMPTY    ,EMPTY,
EMPTY  ,EMPTY    ,EMPTY    ,EMPTY   ,EMPTY  ,EMPTY    ,EMPTY    ,EMPTY,
EMPTY  ,EMPTY    ,EMPTY    ,EMPTY   ,EMPTY  ,EMPTY    ,EMPTY    ,EMPTY,
EMPTY  ,EMPTY    ,EMPTY    ,EMPTY   ,EMPTY  ,EMPTY    ,EMPTY    ,EMPTY,
B_PAWN ,B_PAWN   ,B_PAWN   ,B_PAWN  ,B_PAWN ,B_PAWN   ,B_PAWN   ,B_PAWN,
B_ROOK ,B_KNIGHT ,B_BISHOP ,B_QUEEN ,B_KING ,B_BISHOP ,B_KNIGHT ,B_ROOK
};

#ifdef WIN32
#include <sys\timeb.h>
struct timeb tv;
#else
#include <sys/time.h>
struct timeval tv;
struct timezone tz;
#endif

char cmd_buf[512];
int board[B_DIM];
Moves moves[MAX_PLY][MAX_MOVES];
long moveval[MAX_PLY][MAX_MOVES];
Piece_Class knight_class[B_DIM];
Piece_Class king_class[B_DIM];
Piece_Class far_class[B_DIM][8];
Moves pv[MAX_PLY][MAX_PLY];
Moves killer[MAX_PLY][MAX_KILL];
int pv_length[MAX_PLY];
int stack_size;
int last_stack_size;

int piece_value_init[13]=
{
-V_KING,-V_QUEEN,-V_ROOK,-V_BISHOP,-V_KNIGHT,-V_PAWN,
 V_EMPTY,
 V_PAWN,V_KNIGHT,V_BISHOP,V_ROOK,V_QUEEN,V_KING
};

Opening_Class opening[61][MAX_BOOK];
Hash_Table *hash_table_w;
Hash_Table *hash_table_b;
vlong *hash_stack;
vlong hash_stack_init[MAX_STACK+1];
Moves *stack;
Moves stack_init[MAX_STACK+2];
Castle_Class castle[MAX_STACK];
long random_hash_index_init[B_DIM][14];
long *random_hash_index[B_DIM];
long random_hash_ep[10];
long random_hash_castle[16];
long nodes;
long start_time;
long rest_nodes;
long end_time;
long time_for_move;
long history[64][64];

char piece_char[7]={' ','p','n','b','r','q','k'};
char Piece_Char[7]={'.','P','N','B','R','Q','K'};
int opening_flag[MAX_BOOK];
int *piece_value;
int promote_piece[F_PROMOTE_K+1];
int on_move;
int machine_color;
int ply;
int random_move;
int num_book;
int book_color;
int in_book;
int in_book_init;
int stop_thinking;
int actual_eval;
int show_pv;
int show_board;
int do_whisper;
int do_kibitz;
int search_depth;
int inc_time;
int init_time;
int low_fail;
int ab_window;
int high_fail;
int high_new;
int hash_mask;
int hashold_mask;
int have_kibitzed;
int do_ponder;
int pondering;
int mb_hash;
int king_pos[2];
int interface=INTERFACE;
int no_captures;
int chess_type=1;
int force_mode;
char oponnent_name[60];
int my_rating;
int oponnent_rating;
int ics_mode;
int op_time;
int my_time;
