#include "common.h"

void take_back(void)
{
  int from,to,piece_from,piece_to,flag;

  stack_size--;
  from		=stack[stack_size].from;
  to		=stack[stack_size].to;
  piece_from	=stack[stack_size].piece_from;
  piece_to	=stack[stack_size].piece_to;
  flag		=stack[stack_size].flag;

  board[from]=piece_from;
  board[to]=piece_to;

  if (piece_from==W_KING)
  {
      king_pos[C_WHITE]=from;
  }

  if (piece_from==B_KING)
      king_pos[C_BLACK]=from;

  if (flag & F_FILTER_EP)
  {
      board[stack[stack_size].enpassant]=piece_to;
      board[to]=EMPTY;
  }

  if (flag & F_CASTLE_ALL)
  {
    switch(flag) {
     case F_CASTLE_WSHORT:
       board[7]=W_ROOK;
       board[5]=EMPTY;
       break;
     case F_CASTLE_WLONG:
       board[0]=W_ROOK;
       board[3]=EMPTY;
       break;
     case F_CASTLE_BSHORT:
       board[63]=B_ROOK;
       board[61]=EMPTY;
       break;
     case F_CASTLE_BLONG:
       board[56]=B_ROOK;
       board[59]=EMPTY;
       break;
     }
  }
}
