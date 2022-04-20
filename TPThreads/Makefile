########################################
#~ définitions
########################################

BIN=versionNcond version1cond

# liste des fichiers sources
SRCS1=versionNcond.c
SRCS2=version1cond.c

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

%.o: %.c
	g++ -Wall -Iinclude -c $< -o $@

versionNcond: $(SRCS1:%.c=%.o)
	g++ -o $@ $+ -lpthread

version1cond: $(SRCS2:%.c=%.o)
	g++ -o $@ $+ -lpthread

clean:
	rm -f $(BIN) *.o *~
