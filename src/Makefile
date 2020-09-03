#Makefile for OliThink

CC     = gcc 
DC     = cc
UC     = cc
WC     = bcc32
CFLAGS = -O2 -Wall -DGCC
WFLAGS = -DWIN32
UFLAGS = -DUNIX -O2
DFLAGS = -migrate -O5 -ifo -arch ev56 -tune ev56 -inline speed \
	 -ansi_alias -ansi_args -assume trusted_short_alignment \
	 -float_const -fp_reorder -no_misalign -readonly_strings \
	 -non_shared -speculate all

DLOBJS = main.o\
         hash.o\
	 take_back.o\
         quiesce.o\
	 think.o\
         next_move.o\
	 sort_moves.o\
	 sort_quiesce.o\
	 init.o\
	 io.o attacked.o\
	 generate_moves.o make_move.o search.o eval.o parser.o \
	 piece_init.o
OBJS   = main.c\
         hash.c\
	 take_back.c\
         quiesce.c\
         think.c\
	 sort_moves.c\
         next_move.c\
	 sort_quiesce.c\
	 init.c\
	 io.c attacked.c\
	 generate_moves.c make_move.c search.c eval.c parser.c \
	 piece_init.c

olithink: $(DLOBJS) $(OBJS)
	$(CC) $(CFLAGS) -o olithink $(DLOBJS)

main.o: main.h
eval.o: eval.h
eval_s.o: eval_s.h

$(DLOBJS): common.h Makefile

unix:	$(DLOBJS) $(OBJS)
	$(UC) $(UFLAGS) -o olithink $(DLOBJS)

win: 	$(DLOBJS) $(OBJS)
	$(WC) $(WFLAGS) -o olithink $(DLOBJS)

dec:
	$(DC) $(DFLAGS) $(OBJS) -o olithink

clean:
	rm *.o olithink

inline:
	$(CC) -finline-functions $(CFLAGS) $(OBJS) -o olithink

backup:
	gtar -zcvf olithink.tgz *.h *.c bl*.bin wh*.bin Make* READ* exa*
	cp olithink.tgz /tmp/
	cp olithink.tgz /btp4/btp434/www/

source:
	gtar -zcvf olithink.src.tgz *.h *.c Makefile README.TXT exa*
	cp olithink.src.tgz /tmp/
	cp olithink.src.tgz /btp4/btp434/www/

zip:
	zip olithink.zip *.h *.c bl*.bin wh*.bin Make* READ* exa*
	cp olithink.zip /tmp/
	cp olithink.zip /btp4/btp434/www/

