CC=gcc 
CFLAGS=-Wall -g -std=c99 
EXEC=main
SRC=
DATASET=jogador.csv
TEST_IN=test/in
TEST_IN_DATA=test/Entrada
CWD=$(CURDIR)


reset_data: clean clean_bin all run_data run


all:
	gcc -o ${EXEC} ${SRC}*.c

run:
	./${EXEC}

run_windows:
	$(EXEC)


pipe_out:
	./${EXEC} > output.txt

clean:
	rm -rf *.o *.exe *.out

clean_bin:
	rm -rf *.bin
