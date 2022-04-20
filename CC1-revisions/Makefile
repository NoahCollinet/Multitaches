########################################
#~ définitions
########################################

BIN=bin/client  bin/serveur

# liste des fichiers sources 
SRCS0=client.c utils.c
SRCS1=serveurObfuscated.c 
DIR_GUARD=@mkdir -p $(@D)

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.c
	$(DIR_GUARD)
	gcc -Wall -Iinclude -c $< -o $@

bin/client: $(SRCS0:%.c=obj/%.o)
	$(DIR_GUARD)
	gcc -o $@ $+

bin/serveur: $(SRCS1:%.c=obj/%.o)
	$(DIR_GUARD)
	gcc -o $@ $+ -lpthread

clean:
	rm -f $(BIN) obj/*.o *~