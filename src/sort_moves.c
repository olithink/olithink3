#include "common.h"

void sort_moves(int number,Moves *mv,long *mval, vlong hashkey, int c)
{
  int i,hash_to,hash_from,hash_flag;
  Hash_Table *htable;

  if (c)
       htable=hash_table_w+(hashkey & hash_mask);
  else
       htable=hash_table_b+(hashkey & hash_mask);

  if (htable->rec==hashkey && htable->number)
  {
      hash_flag=htable->mv.flag;
      hash_to=htable->mv.to;
      hash_from=htable->mv.from;
  }
  else
  {
      hash_flag=-1;
      hash_to=-1;
      hash_from=-1;
  }

  for (i=0;i<number;i++)
  {
    mval[i]=history[mv[i].to][mv[i].from];

    if (mv[i].to==pv[0][ply].to) 
       if (mv[i].from==pv[0][ply].from) 
          if (mv[i].flag==pv[0][ply].flag) 
              mval[i]+=1000000-ply*100000;

    if (mv[i].to==hash_to) 
       if (mv[i].from==hash_from) 
           if (mv[i].flag==hash_flag) 
              mval[i]+=4000000;

     if (mv[i].to==killer[ply][0].to &&
        mv[i].from==killer[ply][0].from &&
        mv[i].flag==killer[ply][0].flag)
          mval[i]+=1000;

    if (mv[i].flag & F_HIT)
    {
      if (mv[i].to==stack[stack_size-1].to)
        mval[i]+=200000;
      mval[i]+=(6+abs(mv[i].piece_to)-abs(mv[i].piece_from))<<17;
    }
  }
}
