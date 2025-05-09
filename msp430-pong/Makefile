# CC = msp430-gcc
# CFLAGS = -mmcu=msp430g2553 -Ilib
# SRC = src/main.c src/pong.c src/switch.c lib/lcdutils.c lib/lcddraw.c
# OBJ = $(SRC:.c=.o)
# TARGET = pong

# all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CC) -o $@ $^

# %.o: %.c
# 	$(CC) $(CFLAGS) -c $<

# clean:
# 	rm -f $(OBJ) $(TARGET)


CPU             	= msp430g2553
CFLAGS          	= -mmcu=${CPU} -Os -I../h
LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/ 

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all:pong.elf

#additional rules for files
pong.elf: ${COMMON_OBJECTS} src/main.o src/pong.o
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd 

load: pong.elf
	msp430loader.sh $^

clean:
	rm -f *.o *.elf
