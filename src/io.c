#include "common.h"

void pv_out(int length)
{
  char pgn[10];
  int j;
  for(j=0;j<length;j++)
  {
     pgn_string(pgn,pv[0][j]);
     printf(" %s",pgn);
  }
  printf("\n");
}

void display_pv(int i,int w)
{
  if (abs(w)<30000) actual_eval=w;
  if (!show_pv) return;
  printf("%2d%c%6d %5ld %2d        ",
     i,
     stop_thinking==2 ? '#' :(stop_thinking ? '!' :(
     low_fail ? '-': (high_fail ? '+': (high_new ? '&':'.')))),     
     actual_eval,
     nodes,
     pv_length[0]>i ? pv_length[0] : i);

  i=pv_length[0]>i ? pv_length[0] : i;
  pv_out(i);
  fflush(stdout);
  low_fail=high_fail=high_new=0;
}

void whisper_info(int j)
{
  int i=pv_length[0]>j ? pv_length[0] : j;
  end_time=get_time();
  if (end_time==start_time) ++end_time;

  printf("whisper %c%d.%02d (ply %d)"
    ,actual_eval<0 ? '-' : '+'
    ,abs(actual_eval/100)
    ,abs(actual_eval%100)
    ,j);
  printf(" %ld nodes, %ld nps\nwhisper",
	   nodes, (nodes*100)/(end_time-start_time));
  pv_out(i);
  fflush(stdout);
}

void display_board(void)
{
  int i,j,w,c;
  if (!show_board) return;
  for (j=0;j<8;j++)
  {
    for (i=0;i<8;i++)
    {
      w=board[i+(7-j)*8];
      c=((w>0) ? '*' : ' '); 
      w=(int)((w<0) ? piece_char[-w] : Piece_Char[w]); 
      printf(" %c%c",c,w);
    }
    printf("\n");
  }
}

void display_book(void)
{
  int i,j,counter,fl[MAX_BOOK]; 
  printf("\n");

  if (!in_book || stack_size<1) return;

  for (i=0;i<num_book;i++)
    fl[i]=1;
 
  for (i=0;i<num_book;i++)
  { 
      counter=0;
      if (!fl[i]) continue;
      if (!opening_flag[i]) continue;
      if (opening[stack_size][i].from>63) continue;

      printf("   %c%d%c%d",
	   (opening[stack_size][i].from)%8+'a',
	   (opening[stack_size][i].from)/8+1,
	   (opening[stack_size][i].to)%8+'a',
	   (opening[stack_size][i].to)/8+1);

      if (opening[stack_size+1][i].from>63) 
      { 
	printf("     1\n");
	continue;
      }

      printf("   %c%d%c%d     ",
	   (opening[stack_size+1][i].from)%8+'a',
	   (opening[stack_size+1][i].from)/8+1,
	   (opening[stack_size+1][i].to)%8+'a',
	   (opening[stack_size+1][i].to)/8+1);

      for (j=i;j<num_book;j++)
      {
	if (!opening_flag[j]) continue;
	if (opening[stack_size+1][j].from==opening[stack_size+1][i].from
	 && opening[stack_size+1][j].to==opening[stack_size+1][i].to
	 && opening[stack_size][j].from==opening[stack_size][i].from
	 && opening[stack_size][j].to==opening[stack_size][i].to)
	{
	  counter++;
	  fl[j]=0;
	}
      }
  printf("%d\n",counter);
  }
  printf("\n");
  fflush(stdout);
}

void display_hint(void)
{ 
  char pgn[10];
  if ((in_book) || stack_size<1) return;
  pgn_string(pgn,pv[0][1]);
  printf("Hint: %s\n",pgn);
  fflush(stdout);
}

int input_move(void)
{ 
  char buf[128];
  int f;

  if (!interface) printf(":");

  if (cmd_buf[0]=='\0') fgets(cmd_buf,255,stdin); 

  strncpy(buf,cmd_buf,127);
  cmd_buf[0]='\0';

  if (!strncasecmp(buf,"type",4)) return 1;

  if (!strncmp(buf,"quit",4) ||
      !strncmp(buf,"exit",4))
    return 9;

  if (!strncmp(buf,"post",4))
    show_pv=1;

  if (!strncmp(buf,"nopost",4))
    show_pv=0;

  if (!strncmp(buf,"easy",4))
    do_ponder=0;

  if (!strncmp(buf,"hard",4))
    do_ponder=1;

  if (!strncmp(buf,"hint",4))
    display_hint();

  if (!strncmp(buf,"force",5))
  { 
    machine_color=C_NONE;
    return 0;
  }

  if (!strncmp(buf,"go",2))
  { 
    machine_color=on_move;
    return 0;
  }

  if (!strncmp(buf,"white",5))
  {
    if (machine_color==C_NONE)
      machine_color=on_move^1;
    return 0;
  }

  if (!strncmp(buf,"black",5))
  {
    if (machine_color==C_NONE)
      machine_color=on_move^1;
    return 0;
  }

  if (!strncmp(buf,"level",5))
    sscanf(buf+6,"%*d %d %d",&init_time,&inc_time);

  if (!strncmp(buf,"time",4))
  { 
    sscanf(buf+5,"%d",&my_time);
    f=(stack_size<T_NUMBER ? T_NUMBER*2-stack_size : T_NUMBER);
    time_for_move=my_time/f+inc_time*90;
    if (time_for_move>my_time-50) time_for_move=my_time-50;
  }

  if (!strncmp(buf,"otim",4))
  { 
    sscanf(buf+5,"%d",&op_time);
  }

  if (!strncmp(buf,"xboard",5))
  { 
	   printf("feature myname=\"OliThink 3.0.6\" done=1\n"); 
	   fflush(stdout); 
  }

  if (!strncmp(buf,"name",4))
  { 
    sscanf(buf+5,"%s",oponnent_name);
    printf("tellics say Hello, %s. I wish us a good game!\n",
           oponnent_name);
    fflush(stdout);
    ics_mode=1;
  }

  if (!strncmp(buf,"rating",6))
  { 
    sscanf(buf+7,"%d %d",&my_rating,&oponnent_rating);
  }

  if (!strncmp(buf,"sd",2))
  { 
    sscanf(buf+3,"%d",&search_depth);
    if (search_depth>MAX_DEPTH) search_depth=MAX_DEPTH;
  }

  if (!strncmp(buf,"bogus",3))
    interface=1;

  if (!strncmp(buf,"new",3))
    setup_board();

  if (!strncmp(buf,"bk",2))
    display_book();

  if (!strncmp(buf,"undo",4) && stack_size>0)
  { 
    take_back();
    on_move^=1;
    display_board();
    enable_book_moves();
    return 0;
  }

  if (!strncmp(buf,"remove",6) && stack_size>1)
  { 
    take_back();
    take_back();
    display_board();
    enable_book_moves();
    return 0;
  }

  if (!strncmp(buf,"edit",4))
  { 
    editboard();
    display_board();
  }

  if (!parse_move(buf,moves[0])) return input_move();
  make_move(moves[0][0]);
  if (in_book) enable_book_moves();
  on_move=on_move^1;
  display_board();
  return 0;
}

void editboard(void)
{ 
  char s[128];
  int set_color=C_WHITE,piece;
  for (;;) {
    fgets(s,255,stdin); 
    piece=0;
    if (s[0]=='#') clearboard();
    if (s[0]=='c') set_color*=(-1);
    if (s[0]=='.') 
    {  
       init_pos();
       in_book=0;
       return; 
    }
    if (s[0]=='P'||s[0]=='p') piece=W_PAWN*set_color;
    if (s[0]=='B'||s[0]=='b') piece=W_BISHOP*set_color;
    if (s[0]=='N'||s[0]=='n') piece=W_KNIGHT*set_color;
    if (s[0]=='R'||s[0]=='r') piece=W_ROOK*set_color;
    if (s[0]=='Q'||s[0]=='q') piece=W_QUEEN*set_color;
    if (s[0]=='K'||s[0]=='k') piece=W_KING*set_color;
    if (piece)
      board[(s[1]-'a')+(s[2]-'1')*8]=piece;
  }
}

void load_book(void)
{
  FILE *fp;
  char d,e;
  int i,j;
  num_book=0;
  book_color=machine_color;
  
  if (machine_color==C_WHITE) fp=fopen("white.bin","rb");
  else			      fp=fopen("black.bin","rb");

  if (fp==NULL) return;
  num_book=MAX_BOOK;

  for (i=0;i<MAX_BOOK;i++)
  {
    if (feof(fp)) { num_book=i-1; break; }
    for (j=0;j<MAX_STACK;j++)
    { 
      fread((void*)&d,sizeof(d),1,fp); 
      if (d>63) break;
      fread((void*)&e,sizeof(e),1,fp); 

      if (j<60)
      {
        opening[j][i].from=d;
        opening[j][i].to=e;
      } 
    }
  }
  fclose(fp);
  enable_book_moves();
}

void enable_book_moves(void)
{ 
  int i,j;
  in_book=1;
  for (j=0;j<num_book;j++)
    opening_flag[j]=1;

  for (i=0;i<stack_size;i++)
    for (j=0;j<num_book;j++)
      if (opening[i][j].from != stack[i].from
        || opening[i][j].to != stack[i].to )
          opening_flag[j]=0;
}

void move_after_book(void)
{
  int i,to,from,j=0,number;
  int possible_move[MAX_BOOK];
  number=generate_legal_moves(moves[0],moveval[0],on_move);  
  
  for (i=0;i<num_book;i++)
  {
    if (opening_flag[i])
       possible_move[j++]=i;
  }  

  if (j==0)
  { 
    in_book=0;
    return;
  }

  j=rand()%j;
  from=opening[stack_size][possible_move[j]].from;
  to  =opening[stack_size][possible_move[j]].to;

  if (from>63)
  {
    in_book=0;
    return;
  }
    
  for (i=0;i<number;i++)
    if (moves[0][i].to==to && moves[0][i].from==from) break;

  if (i==number)
  { 
    in_book=0;
    return;
  }
  if (!execute_move(i)) return;
  enable_book_moves();
}

int execute_move(int i)
{ 
  char pgn[8];
  check_cont();

  if (cmd_buf[0]!='\0' && cmd_buf[0]!='?' ) return 0;
  if (on_move!=machine_color) return 0;

  e2e4_string(pgn,moves[0][i]);
  printf("%d. ... %s\n",
         (stack_size+2)/2,
         pgn
        );
  fflush(stdout);

  on_move=on_move^1;
  make_move(moves[0][i]);
  return 1;
}
