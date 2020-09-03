#include "common.h"

void init_pos()
{
  int i;

  stack_size=0;
  memset(castle,0,sizeof(castle));

  for (i=0;i<64;i++)
  {
    if (board[i]==W_KING) { king_pos[C_WHITE]=i; continue; }
    if (board[i]==B_KING) { king_pos[C_BLACK]=i; continue; }
  }

  if (board[4]!=W_KING) castle[0].index|=NO_CASTLE_W;
  if (board[0]!=W_ROOK) castle[0].index|=NO_BIG_CASTLE_W;
  if (board[7]!=W_ROOK) castle[0].index|=NO_SMALL_CASTLE_W;
  if (board[60]!=B_KING) castle[0].index|=NO_CASTLE_B;
  if (board[56]!=B_ROOK) castle[0].index|=NO_BIG_CASTLE_B;
  if (board[63]!=B_ROOK) castle[0].index|=NO_SMALL_CASTLE_B;

  memset(history,0,sizeof(history));
  memset(killer,0,sizeof(killer));
  memset(pv,0,sizeof(pv));
  memset(pv_length,0,sizeof(pv_length));
  memset(stack,0,sizeof(stack));
  end_game=check_for_endgame();
  display_board();
}

void setup_board(void)
{
  int i;
  inc_time=0;
  init_time=3;
  stack_size=0;
  last_stack_size=0;
  machine_color=C_BLACK;
  force_mode=0;
  on_move=C_WHITE;
  book_color=C_NONE;
  in_book=in_book_init;
  have_kibitzed=0;
  pondering=0;
  my_time=op_time=30000;

  for (i=0;i<64;i++)
    board[i]=init_board[i];
  init_pos();
}

vlong my_random(void)
{
     return ((vlong)rand()<<56)
           +((vlong)rand()<<48)
           +((vlong)rand()<<40)
           +((vlong)rand()<<32)
           +((vlong)rand()<<24)
           +((vlong)rand()<<16)
           +((vlong)rand()<<8)
           +(vlong)rand(); 
}

int init_hash(void)
{
  int i,j;
  nlong hash_size=1;
  nlong cell_size=sizeof(Hash_Table);

  if (sizeof(vlong)!=8)
  {
    fprintf(stderr, " ERROR MACHINE/COMPILER DOES NOT SUPPORT 64 BIT HASH! \n");
    return 0;
  }

  srand(0);
  for (i=0;i<64;i++)
    for (j=0;j<14;j++)
      random_hash_index_init[i][j]=my_random();

  for (i=0;i<10;i++)
      random_hash_ep[i]=my_random();

  for (i=0;i<16;i++)
      random_hash_castle[i]=my_random();

  for (i=0;i<64;i++)
    random_hash_index[i]=random_hash_index_init[i]+FIGURE_OFFSET;

  for (i=1;i<1000;i++)
  {
    hash_size=(1<<i);
    if (hash_size*2*cell_size >= mb_hash) break;
  }
  hash_size=hash_size>>1;

  hash_mask=hash_size-1;
  if (!hash_table_w) hash_table_w=malloc(hash_size*cell_size);                      
  if (!hash_table_b) hash_table_b=malloc(hash_size*cell_size);

  if (!hash_table_w || !hash_table_b)
  {
     fprintf(stderr," Not enough memory for hash \n");
     return 0;
  }
  memset(hash_table_w,-127,hash_size*cell_size);
  memset(hash_table_b,-127,hash_size*cell_size);
  fprintf(stderr," allocated %ld hash cells\n"
                 "           %ld kb of memory (%ld bytes each cell)\n",
                 hash_size<<1,(hash_size*cell_size)>>9,cell_size);
  return 1;
}                                                                                 
int init_chess()
{
  promote_piece[0]=EMPTY;
  promote_piece[F_PROMOTE_N]=W_KNIGHT;
  promote_piece[F_PROMOTE_R]=W_ROOK;
  promote_piece[F_PROMOTE_B]=W_BISHOP;
  promote_piece[F_PROMOTE_Q]=W_QUEEN;
  promote_piece[F_PROMOTE_K]=W_KING;

  piece_value=piece_value_init+FIGURE_OFFSET;
  stack=stack_init+1;
  hash_stack=hash_stack_init+1;

  if (!init_hash()) return 0;

  init_knight_moves();
  init_king_moves();
  init_far_moves();
  return 1;
}

void clearboard(void)
{
  int i;
  for (i=0;i<64;i++) board[i]=EMPTY;
  init_pos();
}

int parse_args(int argc, char **argv)
{ 
  int i;
  ics_mode=0;
  time_for_move=200;
  in_book_init=1;
  show_pv=0;
  show_board=0;
  do_whisper=1;
  do_kibitz=1;
  search_depth=MAX_DEPTH;
  random_move=0;
  do_ponder=P_BRAIN;
  mb_hash=DEFAULTMB_HASH*1024*1024;
  ab_window=AB_WINDOW;

  for (i=1;i<argc;i++)
  {
    if (!strncmp(argv[i],"-d",2)) show_board=1;
    if (!strncmp(argv[i],"-n",2)) in_book_init=0;
    if (!strncmp(argv[i],"-p",2)) show_pv=1;
    if (!strncmp(argv[i],"-k",2)) do_kibitz=0;
    if (!strncmp(argv[i],"-w",2)) do_whisper=1;
    if (!strncmp(argv[i],"-x",2)) time_for_move=1<<30;
    if (!strncmp(argv[i],"-t",2)) do_ponder^=1;
    if (!strncmp(argv[i],"-h",2))
    {
       if (++i<argc)
         sscanf(argv[i],"%ld",&mb_hash);
       if (mb_hash<1) mb_hash=1;
    }
    if (!strncmp(argv[i],"-H",2))
    {
       if (++i<argc)
         sscanf(argv[i],"%ld",&mb_hash);
       if (mb_hash<1) mb_hash=1;
       mb_hash*=1024*1024;
    }                                                                         
    if (!strncmp(argv[i],"-s",2))
    {
       if (++i<argc)
         sscanf(argv[i],"%d",&search_depth);
       if (search_depth>MAX_DEPTH) search_depth=MAX_DEPTH;
    }
    if (!strncmp(argv[i],"-r",2))
    {
       if (++i<argc)
	 sscanf(argv[i],"%d",&random_move);
       if (random_move<0) random_move=0;
    }
    if (!strncmp(argv[i],"-ab",3))
    {
       if (++i<argc)
	 sscanf(argv[i],"%d",&ab_window);
       if (ab_window<0) ab_window=0;
    }
  }
  return 1;
}

int exit_chess(int rv)
{
  return rv;
}
