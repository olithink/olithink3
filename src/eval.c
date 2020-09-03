#include "common.h"
#include "eval.h"
#define PAWN_FIELD 5
#define KNIGHT_FIELD 3
#define BISHOP_FIELD 3
#define ROOK_FIELD 3
#define QUEEN_FIELD 1
#define KING_FIELD 1

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

  w=(w_m-b_m);
  w+=(atw-atb);

  if (on_move) return w;
  else return -w;
}
