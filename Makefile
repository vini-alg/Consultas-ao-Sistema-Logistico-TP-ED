# Compilador
CXX = g++
CC = gcc
CXXFLAGS = -std=c++11 -Wall -Iinclude
CFLAGS = -Wall

# Diretorios
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Arquivos fonte e objeto
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Executavel
EXECUTABLE = $(BINDIR)/tp3.out

# Arquivo fonte e executavel do gerador de entrada
GEN_SOURCE = gerador_de_entradas/genwkl3.c
GEN_EXEC = $(BINDIR)/genwkl3

# Regra principal
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Parâmetros configuráveis para o gerador de entrada
NODES    ?= 10
PACKETS  ?= 100
CLIENTS  ?= 10
NUM_CL   ?= 10
NUM_PC   ?= 10
SEED     ?= 1

# Alvo para gerar entrada de teste aleatória
gen-input: 
	@echo "Configurando o gerador com: NODES=$(NODES), PACKETS=$(PACKETS), SEED=$(SEED)..."
	python3 gerador_de_entradas/configure_gen.py --nodes $(NODES) --packets $(PACKETS) --clients $(CLIENTS) --numcl $(NUM_CL) --numpc $(NUM_PC) --seed $(SEED)
	$(MAKE) $(GEN_EXEC) # Força a recompilação do gerador
	-./$(GEN_EXEC)
	mv tp3.wkl inputs/random_input.txt
	@echo "Entrada aleatória gerada em inputs/random_input.txt"

$(GEN_EXEC): $(GEN_SOURCE)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $< -lm

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(EXECUTABLE) $(GEN_EXEC)

.PHONY: all clean