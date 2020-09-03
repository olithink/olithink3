#include "common.h"

void make_move(Moves mv)
{
  int castle_flag=castle[stack_size].index;
  stack[stack_size]=mv;

  board[mv.from]=EMPTY;
  board[mv.to]=mv.piece_from;

  if (mv.piece_from==W_KING) 
  { 
     king_pos[C_WHITE]=mv.to;
     castle_flag|=NO_CASTLE_W;
  }
  else if (mv.piece_from==B_KING)
  {
     king_pos[C_BLACK]=mv.to;
     castle_flag|=NO_CASTLE_B;
  }
  else if (mv.piece_from==W_ROOK)
  {
    if (mv.from==0)
    {
      castle_flag|=NO_BIG_CASTLE_W;
    }
    else if (mv.from==7)
    {
      castle_flag|=NO_SMALL_CASTLE_W;
    }
  }
  else if (mv.piece_from==B_ROOK)
  {
    if (mv.from==56)
    {
      castle_flag|=NO_BIG_CASTLE_B;
    }
    else if (mv.from==63)
    {
      castle_flag|=NO_SMALL_CASTLE_B;
    }
  }
     
  if (mv.flag & F_HIT)
  {
    if (mv.piece_to==W_ROOK)
	{
	  if (mv.to==0) castle_flag|=NO_BIG_CASTLE_W;
	  else if (mv.to==7) castle_flag|=NO_SMALL_CASTLE_W;
	}
    if (mv.piece_to==B_ROOK)
	{
	  if (mv.to==56) castle_flag|=NO_BIG_CASTLE_B; 
	  else if (mv.to==63) castle_flag|=NO_SMALL_CASTLE_B;
	}

    if (mv.flag & F_FILTER_EP)
    {
       board[mv.enpassant]=EMPTY;
    }
  }

  if (mv.flag & F_PROMOTE_ALL)
  {
     board[mv.to]=mv.promote;
  }

  else if (mv.flag & F_CASTLE_ALL)
    {
      switch(mv.flag) {
       case F_CASTLE_WSHORT:
         board[5]=W_ROOK;
         board[7]=EMPTY;
         castle_flag|=CASTLE_W;
         break;
       case F_CASTLE_WLONG:
         board[3]=W_ROOK;
         board[0]=EMPTY;
         castle_flag|=CASTLE_W;
         break;
       case F_CASTLE_BSHORT:
         board[61]=B_ROOK;
         board[63]=EMPTY;
         castle_flag|=CASTLE_B;
         break;
       case F_CASTLE_BLONG:
         board[59]=B_ROOK;
         board[56]=EMPTY;
         castle_flag|=CASTLE_B;
         break;
       }
    }
  stack_size++;
  castle[stack_size].index=castle_flag;
}
