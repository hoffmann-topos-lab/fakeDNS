# Compilador para MIPS (ajuste conforme seu toolchain)
CC = mipsel-linux-gnu-gcc
CFLAGS = -Wall -Wextra -O2 -static
LDFLAGS = 
OBJ = main.o cli.o socket.o dns.o

# Nome do executável final
TARGET = dns_spoof

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

main.o: main.c cli.h socket.h dns.h
	$(CC) $(CFLAGS) -c main.c

cli.o: cli.c cli.h
	$(CC) $(CFLAGS) -c cli.c

socket.o: socket.c socket.h
	$(CC) $(CFLAGS) -c socket.c

dns.o: dns.c dns.h
	$(CC) $(CFLAGS) -c dns.c

clean:
	rm -f $(OBJ) $(TARGET)
