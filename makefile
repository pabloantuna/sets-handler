INTERPRETE = interprete
SRC = main.c
GLIST = glist
INTERVALO = intervalo
CONJUNTO = conjunto
TABLA = tablahash
GTREE = gtree
COMP = gcc
ARGSCOMP = -Wall -Wextra -Werror -std=c99 -g

all: $(INTERPRETE)

$(INTERPRETE): $(CONJUNTO).o $(INTERVALO).o $(GLIST).o $(TABLA).o $(GTREE).o $(SRC)
	$(COMP) $(ARGSCOMP) -o $(INTERPRETE) $(SRC) $(CONJUNTO).o $(INTERVALO).o $(GLIST).o $(TABLA).o $(GTREE).o

$(CONJUNTO).o: ./Conjunto/$(CONJUNTO).c $(INTERVALO).o $(GLIST).o
	$(COMP) $(ARGSCOMP) -c ./Conjunto/$(CONJUNTO).c

$(GLIST).o: ./GList/$(GLIST).c
	$(COMP) $(ARGSCOMP) -c ./GList/$(GLIST).c

$(INTERVALO).o: ./Intervalo/$(INTERVALO).c
	$(COMP) $(ARGSCOMP) -c ./Intervalo/$(INTERVALO).c

$(TABLA).o: ./TablaHash/$(TABLA).c $(GTREE).o
	$(COMP) $(ARGSCOMP) -c ./TablaHash/$(TABLA).c

$(GTREE).o: ./GTree/$(GTREE).c
	$(COMP) $(ARGSCOMP) -c ./GTree/$(GTREE).c

clean:
	rm *.o