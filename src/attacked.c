#include "common.h"

inline int check_for_diagonal(int f,int n,int p1,int p2,int p3)
{
  int j,b;
  if (far_class[f][n].number==0) return 0;

  b=board[far_class[f][n].fields[0]];
  if (b)
  {
    if (b==p1 || b==p2 || b==p3) return 1;
    return 0;
  }

  for (j=1;j<far_class[f][n].number;j++)
  {
    b=board[far_class[f][n].fields[j]];
    if (b)
    {
      if (b==p1 || b==p2) return 1;
      return 0;
    }
  }
  return 0;
}



int w_attacked(int f)
{
  int j;
  if (f<48)
  {
     if ((f & 7)<7) if (board[f+9]==B_PAWN) return 1;
     if ((f & 7)>0) if (board[f+7]==B_PAWN) return 1;
  }

  j=0;
  do
  {
    if (board[knight_class[f].fields[j]]==B_KNIGHT) return 1;
  }
  while (++j<knight_class[f].number);

  if (check_for_diagonal(f,0,B_ROOK,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,1,B_ROOK,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,2,B_ROOK,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,3,B_ROOK,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,4,B_BISHOP,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,5,B_BISHOP,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,6,B_BISHOP,B_QUEEN,B_KING)) return 1;
  if (check_for_diagonal(f,7,B_BISHOP,B_QUEEN,B_KING)) return 1;
  return 0;

}

int b_attacked(int f)
{
  int j;

  if (f>15)
  {
    if ((f & 7)>0) if (board[f-9]==W_PAWN) return 1;
    if ((f & 7)<7) if (board[f-7]==W_PAWN) return 1;
  }

  j=0;
  do
  {
    if (board[knight_class[f].fields[j]]==W_KNIGHT) return 1;
  }
  while (++j<knight_class[f].number);

  if (check_for_diagonal(f,0,W_ROOK,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,1,W_ROOK,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,2,W_ROOK,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,3,W_ROOK,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,4,W_BISHOP,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,5,W_BISHOP,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,6,W_BISHOP,W_QUEEN,W_KING)) return 1;
  if (check_for_diagonal(f,7,W_BISHOP,W_QUEEN,W_KING)) return 1;
  return 0;
}

int attacked(int f, int cl)
{
   if (cl)
     return w_attacked(f);

   return b_attacked(f);
}
