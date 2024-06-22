# constantes
CC=gcc
CFLAGS=-Wall -g -std=c99
EXEC=main
SRC=
DATASET=jogador.csv
TEST_IN=test/in
TEST_IN_DATA=test/Entrada
CWD=$(CURDIR)

# vendo qual o sistema operacional
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
else
    DETECTED_OS := $(shell uname)
endif

# fazendo os comandos de acordo com o sistema operacional
ifeq ($(DETECTED_OS),Windows)
    RM = del /q
    RMDIR = rd /s /q
    RUN = $(EXEC).exe
else
    RM = rm -f
    RMDIR = rm -rf
    RUN = ./$(EXEC)
endif

.PHONY: reset_data all run run_windows pipe_out clean clean_bin

reset_data: clean clean_bin all run_data run

all:
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)*.c

run:
	$(RUN)

run_windows:
	$(RUN)

pipe_out:
	$(RUN) > output.txt

clean:
	$(RM) *.o *.exe *.out

clean_bin:
	$(RMDIR) *.bin
