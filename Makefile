# makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# compilação do frontend em Java
.PHONY: all
all: compile

compile:
	javac Main.java

# execução do backend em Python e do frontend em Java
.PHONY: run
run:
	python3 Socketshandler.py &
	java Main

# limpeza dos arquivos .class gerados pela compilação do Java
.PHONY: clean
clean:
	rm -f *.class
