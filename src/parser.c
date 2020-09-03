#include "common.h"

int parse_move(char *s,Moves *mv)
{
  int number;
  int i,f;
  int flag=F_NORMAL;
  int piece_from=0;
  int from_rank=-1;
  int from_file=-1;
  int to_rank=-1;
  int to_file=-1;
  int promote=0;
  int to=(s[2]-'a')+(s[3]-'1')*8;
  int from=(s[0]-'a')+(s[1]-'1')*8;

  number=generate_legal_moves(moves[0],moveval[0],on_move);

  for (i=0;i<number;i++)
  {
    if (moves[0][i].to==to && moves[0][i].from==from) break;
  }

  if (s[0]>='a' && s[0]<='h' 
   && s[1]>='1' && s[1]<='8' 
   && s[2]>='a' && s[2]<='h')		/* This is e2e4 Notation */
  {
    if (i==number)
    {
      fprintf(stderr,"Illegal move (no matching move)%s\n",s);
      return 0;
    }

    if (moves[0][i].flag & F_PROMOTE_ALL)
    {
      switch (s[4]) {
      default:
      case 'q':
      case 'Q':
           f=F_PROMOTE_Q;
           break;
      case 'k':
      case 'K':
           f=F_PROMOTE_K;
           break;
      case 'b':
      case 'B':
           f=F_PROMOTE_B;
           break;
      case 'r':
      case 'R':
           f=F_PROMOTE_R;
           break;
      case 'n':
      case 'N':
           f=F_PROMOTE_N;
           break;
      }

      for (i=0;i<number;i++)
      {
        if (moves[0][i].to!=to || moves[0][i].from!=from) continue;
        if (moves[0][i].flag & f) break;
      }
      if (i==number)
      {
        fprintf(stderr,"Illegal move (no matching promotion)%s\n",s);
        return 0;
      }
    }
    mv[0]=moves[0][i];
    return 1;
  }

  
  switch (s[0])			/* And here Nxf3 (pgn) Notation */
  {
    case 'K': piece_from=on_move ? W_KING : B_KING; break;
    case 'Q':
    case 'D': piece_from=on_move ? W_QUEEN : B_QUEEN; break;
    case 'R':
    case 'T': piece_from=on_move ? W_ROOK : B_ROOK; break;
    case 'B':
    case 'L': piece_from=on_move ? W_BISHOP: B_BISHOP; break;
    case 'N':
    case 'S': piece_from=on_move ? W_KNIGHT: B_KNIGHT; break;
    case 'O':
    case 'o': s--; break;
    default: 
      if (s[0]>'h' || s[0]<'a') return 0;
      piece_from=on_move ? W_PAWN : B_PAWN;
      s--;
      break;
  }
  s++;
  
  if (s[0]=='x')
  {
    s++;
    flag=F_HIT;
  }
  else
  {
    if (s[0]<'a' || s[0]>'h' || s[1]<'1' || s[1]>'8')
    {
      if (s[0]>='a' && s[0]<='h')
      {
         from_file=s[0]-'a';
         s++;
      }
      else 
      {
         if (s[0]>='1' && s[0]<='8')
         {
           from_rank=s[0]-'1';
           s++;
         }
      }
      if (s[0]=='x')
      {
        s++;
        flag=F_HIT;
      }
    }
  }
  if (s[0]>='a' && s[0]<='h')
  {
    to_file=s[0]-'a';
    s++;
  }
  if (s[0]>='1' && s[0]<='8')
  {
    to_rank=s[0]-'1';
    s++;
  }

  to=to_file+to_rank*8;
  
  if (s[0]=='=')
  {
    s++;
    switch (s[0])
    {
      case 'K': promote=on_move ? W_KING : B_KING; break;
      case 'Q':
      case 'D': promote=on_move ? W_QUEEN : B_QUEEN; break;
      case 'R':
      case 'T': promote=on_move ? W_ROOK : B_ROOK; break;
      case 'B':
      case 'L': promote=on_move ? W_BISHOP: B_BISHOP; break;
      case 'N':
      case 'S': promote=on_move ? W_KNIGHT: B_KNIGHT; break;
    }
  }
  
  if (!strncasecmp(s,"o-o-o",5))
  {
    if (on_move==C_WHITE)
    {
      /* from=4; */
      to=2;
      piece_from=W_KING;
      flag=F_CASTLE_WLONG;
    }
    else
    {
      /* from=60; */
      to=58;
      piece_from=B_KING;
      flag=F_CASTLE_BLONG;
    }
  }
  else if (!strncasecmp(s,"o-o",3))
  {
    if (on_move==C_WHITE)
    {
      /* from=4; */
      to=6;
      piece_from=W_KING;
      flag=F_CASTLE_WSHORT;
    }
    else
    {
      /* from=60; */
      to=62;
      piece_from=B_KING;
      flag=F_CASTLE_BSHORT;
    }
  }

  for (i=0;i<number;i++)
  {
    if (moves[0][i].from<128 &&
        to==moves[0][i].to && 
        piece_from==moves[0][i].piece_from)
    {
      if (from_rank>=0)
        if ((int)(moves[0][i].from / 8)!=from_rank) continue;

      if (from_file>=0)
        if ((int)(moves[0][i].from & 7)!=from_file) continue;

      if (promote!=0)
        if (moves[0][i].promote!=promote) continue;

      if (flag==F_HIT)
        if (!(moves[0][i].flag & F_HIT)) continue;

      break;
    }
  }
  if (i>=number) return 0;
  mv[0]=moves[0][i];

  return 1;
}

int pgn_string(char *pgn,Moves mv)
{
  if (abs(mv.piece_from)==EMPTY || abs(mv.piece_from)>W_KING)
  {
     sprintf(pgn,"*");
     return 0;
  }

  if (abs(mv.piece_from)==W_PAWN)
  {
  if (mv.flag & F_HIT)
  sprintf(pgn,"%cx%c%d",
          mv.from%8+'a',
          mv.to%8+'a',
          mv.to/8+1
         );
  else
  sprintf(pgn,"%c%d",
          mv.to%8+'a',
          mv.to/8+1 
         );

  if (mv.flag & F_PROMOTE_ALL)
  {
    char buf[8];
    sprintf(buf,"=%c",Piece_Char[promote_piece[mv.flag & F_PROMOTE_ALL]]);
    strcat(pgn,buf);
  }

  }
  else
  {
  if (mv.flag & F_HIT)
  sprintf(pgn,"%cx%c%d",
          Piece_Char[abs(mv.piece_from)],
          mv.to%8+'a',
          mv.to/8+1 
         );
  else
  sprintf(pgn,"%c%c%d",
          Piece_Char[abs(mv.piece_from)],
          mv.to%8+'a',
          mv.to/8+1 
         );
  }
  return 1;
}

int e2e4_string(char *pgn,Moves mv)
{
  sprintf(pgn,"%c%d%c%d%c",
    mv.from%8+'a',
    mv.from/8+1,
    mv.to%8+'a',
    mv.to/8+1,
    piece_char[promote_piece[mv.flag & F_PROMOTE_ALL]]
   );
  return 1;
}
