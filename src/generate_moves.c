#include "common.h"

void add_white_moves(int*,Moves*,int,int,int);
void add_black_moves(int*,Moves*,int,int,int);
void add_white_captures(int*,Moves*,int,int,int);
void add_black_captures(int*,Moves*,int,int,int);
void add_white_promotion(int*,Moves*,int);
void add_black_promotion(int*,Moves*,int);
void add_white_castle(int*,Moves*);
void add_black_castle(int*,Moves*);
void add_enpassant(int*,Moves*,int,int,int,int);

inline void move_entry(Moves *mv,int from,int to,int piece_from,int piece_to,int flag)
{
    mv->from=(nchar)from;
    mv->to=(nchar)to;
    mv->piece_from=(schar)piece_from;
    mv->piece_to=(schar)piece_to;
    mv->flag=(nshort)flag;
}

int generate_white_moves(Moves *mv,long *mval,vlong hashkey)
{
  int number=0;
  int i,j;
  int p,pc,b,c;
 
  for (i=0;i<64;i++)
  {
     pc=board[i];
     if (pc<=EMPTY) continue;

     p=pc;
     if (p==W_PAWN)
     {
        if (!board[i+8])
        {
           move_entry(&mv[number],i,i+8,pc,EMPTY,F_PAWN);
           number++;

           if ((i & 56)==second_line_white)
           {
             if (!board[i+16])
             {
               move_entry(&mv[number],i,i+16,pc,EMPTY,F_PAWNDOUBLE);
               number++;
             }
           }
           else 
           { 
             if ((i & 56)==seventh_line_white)
               add_white_promotion(&number,mv,F_NORMAL);
           }
        }
        if ((i & 7)<7)
        { 
          c=i+9;
          b=board[c];
          if (b<EMPTY) 
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_white)
              add_white_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_white)
            add_enpassant(&number,mv,i+1,i,c,pc);
        }
        if ((i & 7)>0)
        { 
          c=i+7;
          b=board[c];
          if (b<EMPTY)
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_white)
              add_white_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_white)
            add_enpassant(&number,mv,i-1,i,c,pc);
        }
        continue;
     }

     if (p==W_KNIGHT)
     {
       j=0;
       do
       { 
         c=knight_class[i].fields[j];
         b=board[c];
         if (b>EMPTY) continue;
         if (!b)
            move_entry(&mv[number],i,c,pc,b,EMPTY);
         else
            move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==W_BISHOP)
     { 
       for (j=4;j<8;j++)
         add_white_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_ROOK)
     { 
       for (j=0;j<4;j++)
         add_white_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_QUEEN)
     { 
       for (j=0;j<8;j++)
         add_white_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_KING)
     {
       j=0;
       do
       { 
         c=king_class[i].fields[j];
         b=board[c];
         if (b>EMPTY) continue;
         if (!b)
           move_entry(&mv[number],i,c,pc,b,EMPTY);
         else
           move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<king_class[i].number);

       add_white_castle(&number,mv);
       continue;
     }
  }
  sort_moves(number,mv,mval,hashkey,C_WHITE);
  return number;
}

int generate_black_moves(Moves *mv,long *mval,vlong hashkey)
{
  int number=0;
  int i,j;
  int p,pc,b,c;
 
  for (i=0;i<64;i++)
  {
     pc=board[i];
     if (pc>=EMPTY) continue;

     p=-pc;
     if (p==W_PAWN)
     {
        if (!board[i-8])
        {
           move_entry(&mv[number],i,i-8,pc,EMPTY,F_PAWN);
           number++;

           if ((i & 56)==second_line_black)
           {
             if (!board[i-16])
             {
               move_entry(&mv[number],i,i-16,pc,EMPTY,F_PAWNDOUBLE);
               number++;
             }
           }
           else 
           { 
             if ((i & 56)==seventh_line_black)
               add_black_promotion(&number,mv,F_NORMAL);
           }
        }
        if ((i & 7)<7)
        { 
          c=i-7;
          b=board[c];
          if (b>EMPTY) 
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_black)
             add_black_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_black)
           add_enpassant(&number,mv,i+1,i,c,pc);
        }
        if ((i & 7)>0)
        { 
          c=i-9;
          b=board[c];
          if (b>EMPTY)
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_black)
             add_black_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_black)
            add_enpassant(&number,mv,i-1,i,c,pc);
        }
        continue;
     }

     if (p==W_KNIGHT)
     {
       j=0;
       do
       { 
         c=knight_class[i].fields[j];
         b=board[c];
         if (b<EMPTY) continue;
         if (!b)
            move_entry(&mv[number],i,c,pc,b,EMPTY);
         else
            move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==W_BISHOP)
     { 
       for (j=4;j<8;j++)
        add_black_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_ROOK)
     { 
       for (j=0;j<4;j++)
        add_black_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_QUEEN)
     { 
       for (j=0;j<8;j++)
        add_black_moves(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_KING)
     {
       j=0;
       do
       { 
         c=king_class[i].fields[j];
         b=board[c];
         if (b<EMPTY) continue;
         if (!b)
            move_entry(&mv[number],i,c,pc,b,EMPTY);
         else
            move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<king_class[i].number);

       add_black_castle(&number,mv);
       continue;
     }
  }
  sort_moves(number,mv,mval,hashkey,C_BLACK);
  return number;
}

int generate_moves(Moves *mv,long *mval,vlong hashkey,int cl)
{
 if (cl) return generate_white_moves(mv,mval,hashkey);
 return generate_black_moves(mv,mval,hashkey);
}

int generate_white_captures(Moves *mv,long *mval)
{
  int number=0;
  int i,j;
  int p,pc,b,c;
 
  for (i=0;i<64;i++)
  {
     pc=board[i];
     if (pc<=EMPTY) continue;

     p=pc;
     if (p==W_PAWN)
     {
        if (!board[i+8])
        {
           if ((i & 56)==seventh_line_white)
           {
             move_entry(&mv[number],i,i+8,pc,EMPTY,F_PAWN);
             number++;
             add_white_promotion(&number,mv,F_NORMAL);
           }
        }
        if ((i & 7)<7)
        { 
          c=i+9;
          b=board[c];
          if (b<EMPTY) 
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_white)
             add_white_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_white)
           add_enpassant(&number,mv,i+1,i,c,pc);
        }
        if ((i & 7)>0)
        { 
          c=i+7;
          b=board[c];
          if (b<EMPTY)
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_white)
             add_white_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_white)
            add_enpassant(&number,mv,i-1,i,c,pc);
        }
        continue;
     }

     if (p==W_KNIGHT)
     {
       j=0;
       do
       { 
         c=knight_class[i].fields[j];
         b=board[c];
         if (b>=EMPTY) continue;
         move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==W_BISHOP)
     { 
       for (j=4;j<8;j++)
        add_white_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_ROOK)
     { 
       for (j=0;j<4;j++)
        add_white_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_QUEEN)
     { 
       for (j=0;j<8;j++)
        add_white_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_KING)
     {
       j=0;
       do
       { 
         c=king_class[i].fields[j];
         b=board[c];
         if (b>=EMPTY) continue;
         move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<king_class[i].number);
     }
  }
  sort_quiesce(number,mv,mval,C_WHITE);
  return number;
}

int generate_black_captures(Moves *mv,long *mval)
{
  int number=0;
  int i,j;
  int p,pc,b,c;
 
  for (i=0;i<64;i++)
  {
     pc=board[i];
     if (pc>=EMPTY) continue;

     p=-pc;
     if (p==W_PAWN)
     {
        if (!board[i-8])
        {
           if ((i & 56)==seventh_line_black)
           {
             move_entry(&mv[number],i,i-8,pc,EMPTY,F_PAWN);
             number++;
             add_black_promotion(&number,mv,F_NORMAL);
           }
        }
        if ((i & 7)<7)
        { 
          c=i-7;
          b=board[c];
          if (b>EMPTY) 
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_black)
             add_black_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_black)
           add_enpassant(&number,mv,i+1,i,c,pc);
        }
        if ((i & 7)>0)
        { 
          c=i-9;
          b=board[c];
          if (b>EMPTY)
          {
            move_entry(&mv[number],i,c,pc,b,F_PAWN_HIT);
            number++;
            if ((i & 56)==seventh_line_black)
             add_black_promotion(&number,mv,F_HIT);
          }
          if ((i & 56)==fifth_line_black)
            add_enpassant(&number,mv,i-1,i,c,pc);
        }
        continue;
     }

     if (p==W_KNIGHT)
     {
       j=0;
       do
       { 
         c=knight_class[i].fields[j];
         b=board[c];
         if (b<=EMPTY) continue;
         move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<knight_class[i].number);
       continue;
     }

     if (p==W_BISHOP)
     { 
       for (j=4;j<8;j++)
         add_black_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_ROOK)
     { 
       for (j=0;j<4;j++)
         add_black_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_QUEEN)
     { 
       for (j=0;j<8;j++)
         add_black_captures(&number,mv,i,j,pc);
       continue;
     }

     if (p==W_KING)
     {
       j=0;
       do
       { 
         c=king_class[i].fields[j];
         b=board[c];
         if (b<=EMPTY) continue;
         move_entry(&mv[number],i,c,pc,b,F_HIT);
         number++;
       }
       while(++j<king_class[i].number);
     }
  }
  sort_quiesce(number,mv,mval,C_BLACK);
  return number;
}

int generate_captures(Moves *mv,long *mval,int cl)
{
  if (cl) return generate_white_captures(mv,mval);
  return generate_black_captures(mv,mval);
}

void add_white_moves(int *number,Moves *mv,int i,int n,int pc)
{
  int j,b,c;
  for (j=0;j<far_class[i][n].number;j++)
  {
    c=far_class[i][n].fields[j];
    b=board[c];
    if (!b)
    {
      move_entry(&mv[*number],i,c,pc,b,F_NORMAL);
      (*number)++;
      continue;
    }

    if (b<EMPTY)
    {
      move_entry(&mv[*number],i,c,pc,b,F_HIT);
      (*number)++;
    }
    return;
  }
}

void add_black_moves(int *number,Moves *mv,int i,int n,int pc)
{
  int j,b,c;
  for (j=0;j<far_class[i][n].number;j++)
  {
    c=far_class[i][n].fields[j];
    b=board[c];
    if (!b)
    {
      move_entry(&mv[*number],i,c,pc,b,F_NORMAL);
      (*number)++;
      continue;
    }

    if (b>EMPTY)
    {
      move_entry(&mv[*number],i,c,pc,b,F_HIT);
      (*number)++;
    }
    return;
  }
}



void add_white_captures(int *number,Moves *mv,int i,int n,int pc)
{
  int j,b,c;
  for (j=0;j<far_class[i][n].number;j++)
  {
    c=far_class[i][n].fields[j];
    b=board[c];
    if (!b) continue;
    if (b<EMPTY)
    {
      move_entry(&mv[*number],i,c,pc,b,F_HIT);
      (*number)++;
    }
    return;
  }
}

void add_black_captures(int *number,Moves *mv,int i,int n,int pc)
{
  int j,b,c;
  for (j=0;j<far_class[i][n].number;j++)
  {
    c=far_class[i][n].fields[j];
    b=board[c];
    if (!b) continue;
    if (b>EMPTY)
    {
      move_entry(&mv[*number],i,c,pc,b,F_HIT);
      (*number)++;
    }
    return;
  }
}

void add_white_promotion(int *number,Moves *mv,int flag)
{
             mv[(*number)-1].flag=(nshort)(flag | F_PROMOTE_N);
             mv[(*number)-1].promote=W_KNIGHT;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_R);
             mv[*number].promote=W_ROOK;
             (*number)++;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_B);
             mv[*number].promote=W_BISHOP;
             (*number)++;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_Q);
             mv[*number].promote=W_QUEEN;
             (*number)++;
}

void add_black_promotion(int *number,Moves *mv,int flag)
{
             mv[(*number)-1].flag=(nshort)(flag | F_PROMOTE_N);
             mv[(*number)-1].promote=B_KNIGHT;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_R);
             mv[*number].promote=B_ROOK;
             (*number)++;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_B);
             mv[*number].promote=B_BISHOP;
             (*number)++;
             mv[*number]=mv[(*number)-1];
             mv[*number].flag=(nshort)(flag | F_PROMOTE_Q);
             mv[*number].promote=B_QUEEN;
             (*number)++;
}

void add_white_castle(int *number,Moves *mv)
{
  int castle_flag=castle[stack_size].index;
    if (!(castle_flag & NO_BIG_CASTLE_W))
    {
      if (!board[3])
      if (!board[2])
      if (!board[1])
      if (board[0]==W_ROOK)
      {
         if (!attacked(4,C_WHITE))
         if (!attacked(3,C_WHITE))
         if (!attacked(2,C_WHITE))
         {
           move_entry(&mv[*number],4,2,W_KING,EMPTY,F_CASTLE_WLONG);
           (*number)++;
         }
      }
    }
    if (!(castle_flag & NO_SMALL_CASTLE_W))
    {
      if (!board[5])
      if (!board[6])
      if (board[7]==W_ROOK)
      {
         if (!attacked(4,C_WHITE))
         if (!attacked(5,C_WHITE))
         if (!attacked(6,C_WHITE))
         {
           move_entry(&mv[*number],4,6,W_KING,EMPTY,F_CASTLE_WSHORT);
           (*number)++;
         }
      }
    }
}

void add_black_castle(int *number,Moves *mv)
{
  int castle_flag=castle[stack_size].index;
    if (!(castle_flag & NO_BIG_CASTLE_B))
    {
      if (!board[59])
      if (!board[58])
      if (!board[57])
      if (board[56]==B_ROOK)
      {
         if (!attacked(60,C_BLACK))
         if (!attacked(59,C_BLACK))
         if (!attacked(58,C_BLACK))
         {
           move_entry(&mv[*number],60,58,B_KING,EMPTY,F_CASTLE_BLONG);
           (*number)++;
         }
      }
    }
    if (!(castle_flag & NO_SMALL_CASTLE_B))
    {
      if (!board[62])
      if (!board[61])
      if (board[63]==B_ROOK)
      {
         if (!attacked(60,C_BLACK))
         if (!attacked(61,C_BLACK))
         if (!attacked(62,C_BLACK))
         {
           move_entry(&mv[*number],60,62,B_KING,EMPTY,F_CASTLE_BSHORT);
           (*number)++;
         }
      }
    }
  return;
}

void add_enpassant(int *number,Moves *mv,int f,int i,int c,int pc)
{
  if (!(stack[stack_size-1].flag & F_PAWNDOUBLE)) return;
  if (stack[stack_size-1].to != f) return;

  mv[*number].from=(nchar)i;
  mv[*number].to=(schar)c;
  mv[*number].piece_from=(schar)pc;
  mv[*number].piece_to=(schar)board[f];
  mv[*number].enpassant=(schar)f;
  mv[*number].flag=F_ENPASSANT;
  (*number)++;
  no_captures=0;
}

