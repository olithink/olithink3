#include "common.h"

void init_knight_moves(void)
{
  int i,j;
  for (i=0;i<64;i++)
  { j=0;
    if ((i & 7)<7 && (i & 56)<48) 
    { knight_class[i].fields[j]=i+17;
      j++;
    }
    if ((i & 7)<6 && (i & 56)<56) 
    { knight_class[i].fields[j]=i+10;
      j++;
    }
    if ((i & 7)<6 && (i & 56)>0) 
    { knight_class[i].fields[j]=i-6;
      j++;
    }
    if ((i & 7)<7 && (i & 56)>8) 
    { knight_class[i].fields[j]=i-15;
      j++;
    }
    if ((i & 7)>0 && (i & 56)<48) 
    { knight_class[i].fields[j]=i+15;
      j++;
    }
    if ((i & 7)>1 && (i & 56)<56) 
    { knight_class[i].fields[j]=i+6;
      j++;
    }
    if ((i & 7)>1 && (i & 56)>0) 
    { knight_class[i].fields[j]=i-10;
      j++;
    }
    if ((i & 7)>0 && (i & 56)>8) 
    { knight_class[i].fields[j]=i-17;
      j++;
    }
    knight_class[i].number=j;
  }
}

void init_king_moves(void)
{
  int i,j;
  for (i=0;i<64;i++)
  { j=0;
    if ((i & 7)<7) 
    { 
      king_class[i].fields[j]=i+1;
      j++;
    }
    if ((i & 7)<7 && (i & 56)<56) 
    { 
      king_class[i].fields[j]=i+9;
      j++;
    }
    if ((i & 7)<7 && (i & 56)>0) 
    { 
      king_class[i].fields[j]=i-7;
      j++;
    }
    if ((i & 56)>0) 
    { 
      king_class[i].fields[j]=i-8;
      j++;
    }
    if ((i & 7)>0 && (i & 56)>0) 
    { 
      king_class[i].fields[j]=i-9;
      j++;
    }
    if ((i & 7)>0) 
    { 
      king_class[i].fields[j]=i-1;
      j++;
    }
    if ((i & 7)>0 && (i & 56)<56) 
    { 
      king_class[i].fields[j]=i+7;
      j++;
    }
    if ((i & 56)<56) 
    { 
      king_class[i].fields[j]=i+8;
      j++;
    }
    king_class[i].number=j;
  }
}

void init_far_moves(void)
{
  int i,j,k,n;

  n=0;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k+1;i<64;i++)
    {
      if ((i & 7)==0) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }
      
  n=1;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k+8;i<64;i+=8)
    {
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=2;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k-1;i>=0;i--)
    {
      if ((i & 7)==7) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=3;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k-8;i>=0;i-=8)
    {
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=4;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k+9;i<64;i+=9)
    {
      if ((i & 7)==0) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=5;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k+7;i<64;i+=7)
    {
      if ((i & 7)==7) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=6;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k-9;i>=0;i-=9)
    {
      if ((i & 7)==7) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }

  n=7;
  for (k=0;k<64;k++)
  { j=0;
    for (i=k-7;i>=0;i-=7)
    {
      if ((i & 7)==0) break;
      far_class[k][n].fields[j]=i;
      j++;
    }
    far_class[k][n].number=j;
  }
}

