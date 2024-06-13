CC=gcc 
CFLAGS=-Wall -g -std=c99 
EXEC=main
SRC=trabalho_arquivos/
DATASET=jogador.csv
TEST_IN=test/in
TEST_IN_DATA=test/Entrada
CWD=$(CURDIR)


reset_data: clean clean_bin all run_data run


all:
	gcc -o ${EXEC} ${SRC}*.c
run:
	./${EXEC}
pipe:
	./${EXEC} > output.txt

run_valgndr:
	valgrind --leak-check=full --show-leak-kinds=all  ./${EXEC}

run_gdb:
	gdb ./${EXEC}

clean:
	rm -rf *.o *.exe *.out

clean_bin:
	rm -rf *.bin

test_run_clean: all
	@read -p "numero do teste: " numero; \
	cp ${TEST_IN_DATA}/binario$$numero.bin ${CWD}; \
	./${EXEC} < ${TEST_IN}/$$numero.in
	make clean_bin

test_run: all
	@read -p "numero do teste: " numero; \
	cp ${TEST_IN_DATA}/binario$$numero.bin ${CWD}; \
	./${EXEC} < ${TEST_IN}/$$numero.in

test_run_valgrind: all
	@read -p "numero do teste: " numero; \
	cp ${TEST_IN_DATA}/binario$$numero.bin ${CWD}; \
	valgrind --leak-check=full --show-leak-kinds=all ./${EXEC} < ${TEST_IN}/$$numero.in
	make clean_bin
