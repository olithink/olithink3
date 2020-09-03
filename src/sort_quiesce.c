#include "common.h"

void sort_quiesce(int number,Moves *mv,long *mval, int c)
{
  int i;

  for (i=0;i<number;i++)
  {
    mval[i]=0;

    if (mv[i].flag & F_HIT)
    {
      if (mv[i].to==stack[stack_size-1].to)
        mval[i]+=500000;
      mval[i]+=(6+abs(mv[i].piece_to)-abs(mv[i].piece_from))<<17;
    }
  }
}
