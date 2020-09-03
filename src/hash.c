#include "common.h"

vlong update_hash(void)
{
  int from;
  int to; 
  vlong hashkey;

  from=stack[stack_size-1].from;
  to=stack[stack_size-1].to;

  hashkey=hash_stack[stack_size-1];

  hashkey=hashkey^random_hash_index[from][stack[stack_size-1].piece_from];
  hashkey=hashkey^random_hash_index[from][board[from]];
  hashkey=hashkey^random_hash_index[to][stack[stack_size-1].piece_to];
  hashkey=hashkey^random_hash_index[to][board[to]];

  if (stack[stack_size-1].flag & F_PAWNDOUBLE)
    hashkey=hashkey^random_hash_ep[stack[stack_size-1].to & 7];

  hashkey=hashkey^random_hash_castle[castle[stack_size].index];

  hash_stack[stack_size]=hashkey;
  return hashkey;
}

vlong calc_hash(void)
{
  int i;
  vlong hashkey=0;
  if (stack_size>0) return update_hash();
  for (i=0;i<64;i++)
  {
    if (board[i])
      hashkey=hashkey^random_hash_index[i][board[i]];
  }

  if (stack[stack_size-1].flag & F_PAWNDOUBLE)
    hashkey=hashkey^random_hash_ep[stack[stack_size-1].to & 7];

  hashkey=hashkey^random_hash_castle[castle[stack_size].index];

  hash_stack[stack_size]=hashkey;
  return hashkey;
}

void store_hash(vlong hashkey, int depth, int color, int value, int flag,
                int number, Moves mv)
{
  Hash_Table *htable;

  if (color)
    htable=hash_table_w+(hashkey & hash_mask);
  else
    htable=hash_table_b+(hashkey & hash_mask);

  if (depth<htable->depth) return;
  if (depth==htable->depth) 
     if (!(flag & LOW_BOUND))
        return;

  if (flag & PV_EXACT) {
    if (abs(value)>29000)
    {
      if (value>0) value=value+ply-1;
      else value=value-ply+1;
    }
  }

  htable->flag=(nchar)flag;
  htable->rec=hashkey;
  htable->value=(short)value;
  htable->depth=(schar)depth;
  htable->mv=mv;
  htable->number=(nchar)number;
}

int look_up(vlong hashkey,int depth,int color,int *alpha,int *beta,int *number)
{
  int w,retv=0;

  Hash_Table *htable;
  if (color)
    htable=hash_table_w+(hashkey & hash_mask);
  else
    htable=hash_table_b+(hashkey & hash_mask);

  if (htable->rec==hashkey)
  {
      w=htable->value;

      if (htable->depth>=depth)
      {
        switch(htable->flag)
        {
          case LOW_BOUND:
           if (w >= *beta)
           {
              *beta=w;
              return(LOW_BOUND);
           }
           break;

          case UP_BOUND:
           if (w <= *alpha)
           {
              *alpha=w;
              return(UP_BOUND);
           }
           break;

          case EXACT_EVAL:
           if (w >= *alpha)
           {
             if (abs(w)>29000)
             {
               if (w>0) w-=(ply-1);
               else w+=(ply-1);
             }

           *alpha=w;
           }
           if (htable->number>0) update_pv(htable->mv,ply);
           *number=htable->number;
           return(EXACT_EVAL);
          default:
           break;
        }
      }
  }
  return retv;
}
