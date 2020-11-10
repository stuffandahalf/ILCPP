.POSIX:

POSIX_FLAG=-D _POSIX_C_SOURCE=200112L

SRC=src
TARGET=ilcpp

OBJS=main.o

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)

main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $(SRC)/main.c $(POSIX_FLAG)

