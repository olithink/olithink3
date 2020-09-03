#include "common.h"

int think(void)
{
  int i,j,k,n,fifty,w,number;
  int alpha=-30000;
  int beta=30000;
  int mate_in=0;
  int m=stack_size-last_stack_size;
  last_stack_size=stack_size;

  ply=0;
  nodes=0;
  rest_nodes=CHECK_NODES;

  init_eval(random_move);
  end_game=check_for_endgame();

  for (i=0;i<hash_mask;i++)
  {
     if (hash_table_w[i].depth>0) hash_table_w[i].depth--;
     if (hash_table_b[i].depth>0) hash_table_b[i].depth--;
  }

  for (i=m;i<MAX_PLY;i++)
  {
     pv[0][i-m]=pv[0][i];
     pv_length[i-m]=m;
  }

  memset(history,0,sizeof(history));
/*
  memset(killer,0,sizeof(killer));  
  memset(pv,0,sizeof(pv));
  memset(pv_length,0,sizeof(pv_length));
  memset(hash_table_w,-127,sizeof(hash_table_w));
  memset(hash_table_b,-127,sizeof(hash_table_b));
*/


  for (j=1;j<MAX_DEPTH;j++)
  {

    stop_thinking=0;
    high_new=0;
    high_fail=0;
    low_fail=0;
    w=search(alpha,beta,j);

    if (w==alpha)
    { 
       low_fail=1;
       time_for_move*=5;
       w=search(-30000,beta-ab_window+1,j);
       time_for_move/=5;
    }
    if (w==beta)
    {
       high_fail=1;
       time_for_move*=5;
       w=search(alpha+ab_window-1,30000,j);
       time_for_move/=5;
    }
    
    if ((w==MACHINE_MATED || w==STALE_MATE)) return (w);

    if (actual_eval>29000)
    { 
      mate_in=(30001-actual_eval)/2;
      if ((mate_in*2-1)<=j+2) break;
    } 

    if (j>=search_depth) break;

    if (stop_thinking) break;
    time_for_move=time_for_move*2/5;
    if (check_cont()) stop_thinking=2;
    time_for_move=time_for_move*5/2;

    if (stop_thinking) break;
    if (ab_window>0)
    {
      alpha=w-ab_window;
      beta=w+ab_window;
    }

  }

  if (j==search_depth+1) j--;

  display_pv(j,actual_eval);
  number=generate_moves(moves[ply],moveval[ply],calc_hash(),on_move);

  for (i=0;i<number;i++)
  {
   if (moves[ply][i].to==pv[0][0].to 
    && moves[ply][i].from==pv[0][0].from
    && moves[ply][i].flag==pv[0][0].flag ) break;
  }

  if (i>=number) 
  { 
    fprintf(stderr,"Illegal Move from engine !\n"); 
    i=0;
  }

  alpha=execute_move(i);

  fifty=0;

  if (stack_size>1)
  for (n=stack_size-2;n>=0;n--)
  {
    if (stack[n].flag!=F_NORMAL) break;
    if (stack[n-1].flag!=F_NORMAL) break;
	fifty++;
	if (fifty>=100) return FIFTY_MOVE;
    if (hash_stack[stack_size]==hash_stack[n] || 
		hash_stack[stack_size-1]==hash_stack[n]) {
		for (k=j-1;k>=0;k--) {
			if (hash_stack[n]==hash_stack[k]) return THREE_POS;
		}
	}
  }     

  if (!alpha) return 0;

  if (mate_in && do_kibitz && !have_kibitzed)
  {
    printf("kibitz Forced mate in %d move(s) !\n",mate_in);
    have_kibitzed=1;
  }

  if (do_whisper && j>1)
   whisper_info(j);

  if ((w==MACHINE_MATES)) return(w);

  return 0;
}
