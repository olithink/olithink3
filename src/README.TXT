OliThink, Version 3.0.7
   (c) 2003 Oliver Brausch, ob112@web.de

OliThink is a medium-high strength chess programm, with an ELO
about 2100. 

If you are looking for an aggressiv, fast, but beatable 
chess programm, you will be happy with OliThink.
For suggestions, improvements, critics, please email to 
ob112@web.de or http://home.arcor.de/dreamlike

Extract	: tar -xvzf olithink.tgz    on Linux
		: gtar -xvzf olithink.tgz   on commercial Unix platforms
		: pkunzip -d olithink.zip   on dos/windows
		: pkunzip oliexe.zip        on dos/windows

Compile	: make		#Unix with gcc
		: make unix	#Unix with native compiler
		: make dec	#Dec Alpha optimized
		: make win	#Windows with Borland Compiler
                  Note: if your compiler does not support 'long long', try
                  __int64, 64 bit integer ist required!

Interface	: xboard/winboard/robofics or compatible 
                : (all functions implemented)
		: local:  xboard -fcp ./olithink
		: ICS:	  xboard -zp -ics -icshost 131.246.89.4 -fcp ./olithink
                see examples in example.*

All (!) relevant xboard/winboard menus and options should work,
including: 
		'Show Thinking' , '-thinking true'
		'Move Now'
		'Retract Move'
		'Hint'
		'Book'
                '-sd #' (search depth)

Additional command line options without the interface:
      -h #    : hashsize (if #<=128 it will be in MB) (default 4 MB)
	-p	: show principle variant
	-s #	: search depth # plies
	-d	: display ASCII board
	-n	: don't use opening book
	-k	: do not kibitz forced win
	-w	: do not whisper info
        -ab #   : use AB_WINDOW of size 2*#, 0 for none

Files:
	common.h	  : all relevant constants and external functions and variables.
	eval.h		  : square tables for eval.c
	main.h		  : initialisation of board and variables
	eval.c		  : gives back the evaluations of the actual position
	search.c	  : the heart of the OliThink: includes search, quiescience and think
	generate_moves.c  : generate moves, captures and sorts them
	make_move.c	  : make move and undo move
	init.c		  : initialisation, memory allocation, parse arguments and setup board
	io.c		  : conversation with interface
	main.c		  : main loop, switches for different chess types
	hash.c		  : calculates hash, stores hash and looks for hashed postition
        parser.c          : reads pgn and e2e4-notation and translates for the engine
        piece_init.c      : calculates the possible moves of all pieces on all squares
        attacked.c	  : checks whether a field is attacked
	README.TXT	  : this file
        Makefile	  : compiler directives

	white.bin	  : Optional opening books
	black.bin	  :
