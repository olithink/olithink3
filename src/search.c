#include "common.h"

int search(int alpha,int beta,int depth)
{
  int c,f,i,k,w,number;
  vlong hashkey;
  int initalpha=alpha;

  if (nodes-rest_nodes>0)
  {
    rest_nodes+=CHECK_NODES;
    if (check_cont()) stop_thinking=1;
  }

  hashkey=calc_hash();

  if (ply)
  if (stack[stack_size-1].flag==F_NORMAL)
  for (i=stack_size-1;i>=0;i--)
  {
    if (stack[i].flag!=F_NORMAL) break;
    if (hash_stack[stack_size]==hash_stack[i])
         return (on_move==machine_color ? -V_DRAW : V_DRAW);
  }                                                                               
  pv_length[ply]=ply;

  if (depth<=0)
  {
     alpha=quiesce(alpha,beta);
     return alpha;
  }

  nodes++;
  f=0;

  c=attacked(king_pos[on_move],on_move);

  if (c) depth++;   /* This one is very important, 
                       checks MUST be searched deeper*/

  if (ply)
  switch(look_up(hashkey,depth,on_move,&alpha,&beta,&number)) 
    {
    case LOW_BOUND:
      return beta;
    case EXACT_EVAL:
    case UP_BOUND:
      if (alpha>beta) 
      { 
        store_hash(hashkey,depth,on_move,alpha,LOW_BOUND,number,pv[ply][ply]);
        return beta;
      }
      return alpha;
      default:
        break;
    }

   number=generate_moves(moves[ply],moveval[ply],hashkey,on_move);


  for (k=0;k<number;k++)
  {
    if (stop_thinking) return alpha;
    i=next_move(moveval[ply],number);
    make_move(moves[ply][i]);

    if (!attacked(king_pos[on_move],on_move))
    {
        f=1;
        on_move=on_move^1;
        ply++;
        w=-search(-beta,-alpha,depth-1);
        ply--;
        on_move=on_move^1;
    }
    else w=-30000;

    take_back();
    if (stop_thinking) return alpha;

    if (w>alpha)
    { 
      history[moves[ply][i].to][moves[ply][i].from]+=depth;

      if (w>=beta) 
      { 
        killer[ply][0]=moves[ply][i];
        store_hash(hashkey,depth,on_move,w,LOW_BOUND,number,moves[ply][i]);
        return beta;
      }
      alpha=w;

      update_pv(moves[ply][i],ply);

      if (ply==0) 
      {  
         if (k>0) high_new=1; 
         display_pv(depth,w);
      }

    }
  }

  if (!f)
  {
   if (!c) {
            alpha=0;
            if (ply==0) return STALE_MATE;
           }
   else { 
            alpha=(ply-30000);
            if (ply==0) return MACHINE_MATED;
        }
  }

  if (alpha>initalpha)
    store_hash(hashkey,depth,on_move,alpha,EXACT_EVAL,number,pv[ply][ply]);
  else
    store_hash(hashkey,depth,on_move,alpha,UP_BOUND,number,pv[ply][ply]);

  return alpha;
}
