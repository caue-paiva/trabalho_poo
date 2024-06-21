# makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# diretório para os arquivos .class
OUT_DIR = out

# lista de arquivos Java
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java

# compilação do frontend em Java
.PHONY: all
all: compile

compile: $(OUT_DIR)
	javac -d $(OUT_DIR) $(JAVA_FILES)

# criação do diretório de saída para os arquivos .class
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# execução do backend em Python e do frontend em Java
.PHONY: run
run:
	python3 Socketshandler.py &
	java -cp $(OUT_DIR) Main

# lipando os  arquivos .class gerados pela compilação do Java
.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
