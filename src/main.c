/* OliThink Version 3.0.1 (c) Oliver Brausch, 01. Mar. 2001
//
// ob112@web.de
*/
#include "common.h"
#include "main.h"

int main(int argc, char **argv)
{ 
  int w;
  hashold_mask=0;
  printf("Chess - OliThink\n");
  signal(SIGINT,SIG_IGN);
  cmd_buf[0]='\0';

  if (!parse_args(argc,argv)) return exit_chess(0);
  if (!init_chess()) return exit_chess(0);
  setup_board();
 
  for (;;)
  { 
    if (on_move==machine_color)
    {
      start_time=get_time();
      srand(start_time);

      if (in_book)
      {
        if (book_color!=machine_color) load_book();
        move_after_book();
      }

      if (!in_book)
      {
        if ((w=think())!=0) game_ends(w);
      }
    }
   
    if (on_move!=machine_color || cmd_buf[0]!='\0') 
    {
      if (input_move()==9)return exit_chess(1);
    }
  }
}

int generate_legal_moves(Moves *mv,long *mval,int cl)
{
  int k,n,afl,number;
  vlong hashkey=calc_hash();
  number=generate_moves(mv,mval,hashkey,cl);

     for (k=0;k<number;k++)
     {
       make_move(mv[k]);
       afl=attacked(king_pos[cl],cl);
       take_back();

       if (afl)
       {
         --number;
         for (n=k;n<number;n++)
           mv[n]=mv[n+1];
         --k;
       }
     }    
     return number;
}

void TermSearch(int sig)
{
   if (cmd_buf[0]!='\0') fgets(cmd_buf,255,stdin);
   stop_thinking=1;
   return;
}

long get_time(void)
{
#ifndef WIN32
  gettimeofday (&tv, &tz);
  return(tv.tv_sec*100+(tv.tv_usec/10000));
#else
  ftime(&tv);
  return(tv.time*100+tv.millitm/10);
#endif
}

int check_cont(void) {
  if (bioskey()) TermSearch(0); 
  return (get_time()-start_time>=time_for_move);
}

#ifndef WIN32

int bioskey(void)
{
  fd_set readfds;

  FD_ZERO (&readfds);
  FD_SET (fileno(stdin), &readfds);
  tv.tv_sec=0; tv.tv_usec=0;
  select(16, &readfds, 0, 0, &tv);

  return (FD_ISSET(fileno(stdin), &readfds));
}

#else

#  include <windows.h>
#  include <conio.h>
int bioskey(void)
{
  int i;
   static int init = 0, pipe;
   static HANDLE inh;
   DWORD dw;

#if defined(FILE_CNT)
    if (stdin->_cnt > 0) return stdin->_cnt;
#endif
    if (!init) {
      init = 1;
      inh = GetStdHandle(STD_INPUT_HANDLE);
      pipe = !GetConsoleMode(inh, &dw);
      if (!pipe) {
        SetConsoleMode(inh, dw & ~(ENABLE_MOUSE_INPUT|ENABLE_WINDOW_INPUT));
        FlushConsoleInputBuffer(inh);
      }
    }
    if (pipe) {
      if (!PeekNamedPipe(inh, NULL, 0, NULL, &dw, NULL)) {
        return 1;
      }
      return dw;
    } else {
      GetNumberOfConsoleInputEvents(inh, &dw);
      return dw <= 1 ? 0 : dw;
	}
  return(i);
}
#endif
                                
void game_ends(int w)
{
  if (w==STALE_MATE)
  { 
     printf("Drawn by Stalemate!\n");
  }
  if (w==MACHINE_MATED)
  {
     printf(machine_color==C_WHITE ? "Black mates!\n":"White mates!\n");
  }
  if (w==MACHINE_MATES)
  {
     printf(machine_color==C_BLACK ? "Black mates!\n":"White mates!\n");
  }
  if (w==FIFTY_MOVE)
  { 
     if (ics_mode) printf("draw\n");
     if (!ics_mode) printf("Drawn by 50 moves rule!\n");
  }
  if (w==THREE_POS)
  { 
     if (ics_mode) printf("draw\n");
     if (!ics_mode) printf("Drawn by repitition!\n");
  }
  fflush(stdout);
}
