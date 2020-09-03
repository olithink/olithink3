#include "common.h"

int quiesce(int alpha,int beta)
{
  int c,f,i,k,w,number;
  nodes++;
  pv_length[ply]=ply;

  c=attacked(king_pos[on_move],on_move);       
  if (c)
  {
    number=generate_moves(moves[ply],moveval[ply],0,on_move);
  }
  else
  {
    w=eval();
    if (w>=beta) return beta;
    if (w>alpha) alpha=w;
    number=generate_captures(moves[ply],moveval[ply],on_move);
  }

  f=0;
  for (k=0;k<number;k++)
  {
    i=next_move(moveval[ply],number);
    make_move(moves[ply][i]);

    if (!attacked(king_pos[on_move],on_move))
    {
      f=1;
      on_move=on_move^1;
      ply++;
      w=-quiesce(-beta,-alpha);
      ply--;
      on_move=on_move^1;
    }
    else w=-30000;
    take_back();

    if (w>alpha)
    {
      if (w>=beta)
      {
        return beta;
      }
      alpha=w;

      update_pv(moves[ply][i],ply);
    }
  }
  if (!f)
  {
    if (c) return (ply-30000);
  }
  return alpha;
}
