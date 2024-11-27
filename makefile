# Nome do executável
TARGET = semaforo

# Compilador
CC = gcc

# Flags do compilador
CFLAGS = -Wall -pthread -O2

# Nome do arquivo fonte
SRC = semaforo.c

# Regra padrão
all: $(TARGET)

# Regra para construir o executável
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regra para limpar arquivos gerados
clean:
	rm -f $(TARGET)

# Regra para rodar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para reconstruir e rodar
rebuild: clean all run
