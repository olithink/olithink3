#include "common.h"
#include "eval.h"
#define PAWN_FIELD 5
#define KNIGHT_FIELD 3
#define BISHOP_FIELD 3
#define ROOK_FIELD 3
#define QUEEN_FIELD 1
#define KING_FIELD 1

void init_eval(int r)
{ 
  int i;
  for (i=0;i<64;i++)
  {
    move_black[i]=move_black_init[i];
    move_white[i]=move_white_init[i];
  }
    
  if (r<0) return;
  while(r--)
  {
    i=rand()%64;
    move_white[i]++;
    move_black[i%8+8*(7-(int)(i/8))]++;
  }
}

int check_for_endgame(void)
{
  int i,p=0;
  int bnw=1,bnb=1,retv=1;

   for (i=0;i<64;i++)
   {
    if (abs(board[i]==W_PAWN)) {bnw=0;bnb=0;}
    if (board[i]>W_PAWN && board[i]<W_KING)
      p+=piece_value[board[i]]; 
   }
   
   if (bnw==1 && bnb==1) retv=2;
    
   if (p>V_QUEEN) return 0;

   if (p!=V_KNIGHT+V_BISHOP) bnw=0;
   if (p) bnb=0;

   p=0;
   for (i=0;i<64;i++)
    if (board[i]<B_PAWN && board[i]>B_KING)
      p-=piece_value[board[i]]; 

   if (p>V_QUEEN) return 0;

   if (p!=V_KNIGHT+V_BISHOP) bnb=0;
   if (p) bnw=0;

   if (bnw || bnb) 
   { 
     for (i=0;i<64;i++)
       if (abs(board[i]==W_BISHOP)) break;

     if ((i%2)==0) bnw=+4;
     else bnw=-4;

     if (end_game!=3)
     for (i=0;i<12;i++)
     {
       move_endgame[bnkk_add[i]]+=bnw;   
       move_endgame[bnkk_sub[i]]-=bnw;   
     }
     return 3;
   }

   return retv;
}

inline void add_fields(int *num,int i,int n,int pc)
{
  int j,c;
  for (j=0;j<far_class[i][n].number;j++)
  {
    c=far_class[i][n].fields[j];
    num[c]+=pc;

    if (board[c]) return;
  }
}

int fields_white(int*num)
{
  int number=0;
  int i,j;
  int p,c;

  for (i=0;i<64;i++)
  {
     p=board[i];
     if (p<=EMPTY) continue;
     if (p==W_PAWN)
     {
        number+=V_PAWN;
        if ((i & 7)<7)
        {
           c=i+9;
           num[c]+=PAWN_FIELD;
        }

        if ((i & 7)>0)
        {
           c=i+7;
           num[c]+=PAWN_FIELD;
        }

        continue;
     }

     if (p==W_KNIGHT)
     {
       number+=V_KNIGHT;
       j=0;
       do
       {
         c=knight_class[i].fields[j];
         num[c]+=KNIGHT_FIELD;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==W_BISHOP)
     {
       number+=V_BISHOP;
       for (j=4;j<8;j++)
         add_fields(num,i,j,BISHOP_FIELD);
       continue;
     }

     if (p==W_ROOK)
     {
       number+=V_ROOK;
       for (j=0;j<4;j++)
         add_fields(num,i,j,ROOK_FIELD);
       continue;
     }

     if (p==W_QUEEN)
     {
       number+=V_QUEEN;
       for (j=0;j<8;j++)
         add_fields(num,i,j,QUEEN_FIELD);
       continue;
     }

     if (p==W_KING)
     {
       j=0;
       do
       {
         c=king_class[i].fields[j];
         num[c]+=KING_FIELD;
       }
       while(++j<king_class[i].number);
     }
  }
  return number;
}

int fields_black(int*num)
{
  int number=0;
  int i,j;
  int p,c;

  for (i=0;i<64;i++)
  {
     p=board[i];
     if (p>=EMPTY) continue;
     if (p==B_PAWN)
     {
        number+=V_PAWN;
        if ((i & 7)<7)
        {
          c=i-7;
          num[c]+=PAWN_FIELD;
        }

        if ((i & 7)>0)
        {
          c=i-9;
          num[c]+=PAWN_FIELD;
        }

        continue;
     }

     if (p==B_KNIGHT)
     {
       number+=V_KNIGHT;
       j=0;
       do
       {
         c=knight_class[i].fields[j];
         num[c]+=KNIGHT_FIELD;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==B_BISHOP)
     {
       number+=V_BISHOP;
       for (j=4;j<8;j++)
         add_fields(num,i,j,BISHOP_FIELD);
       continue;
     }

     if (p==B_ROOK)
     {
       number+=V_ROOK;
       for (j=0;j<4;j++)
         add_fields(num,i,j,ROOK_FIELD);
       continue;
     }

     if (p==B_QUEEN)
     {
       number+=V_QUEEN;
       for (j=0;j<8;j++)
         add_fields(num,i,j,QUEEN_FIELD);
       continue;
     }

     if (p==B_KING)
     {
       j=0;
       do
       {
         c=king_class[i].fields[j];
         num[c]+=KING_FIELD;
       }
       while(++j<king_class[i].number);
     }
  }
  return number;
}


int eval(void)
{ 
  int i,w,b_m,w_m,atw,atb;
  int numw[64],numb[64];

  atw=0; atb=0; 

  for (i=0;i<64;i++) { numw[i]=0; numb[i]=0;}

  w_m=fields_white(numw);
  b_m=fields_black(numb);

  for (i=0;i<64;i++)
  {
    atw+=move_white[i]*numw[i]; 
    atb+=move_black[i]*numb[i];
  }

  if (end_game)
  {
    atw+=5*end_game*end_game*move_endgame[king_pos[C_WHITE]];
    atb+=5*end_game*end_game*move_endgame[king_pos[C_BLACK]];
  }

  w=(w_m-b_m);
  w+=(atw-atb);

  if (on_move) return w;
  else return -w;
}
