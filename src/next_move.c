#include "common.h"

int next_move(long *mval,int number)
{
  int i,k=0,max=-1;
  for (i=0;i<number;i++)
    if (mval[i]>max)
     { max=mval[i];k=i; }

  mval[k]=-1;
  return k;
}

void update_pv(Moves mv,int p)
{
      int j;

      pv[p][p]=mv;
      for(j=p+1;j<pv_length[p+1];j++)
             pv[p][j]=pv[p+1][j];
      pv_length[p]=pv_length[p+1];
}

