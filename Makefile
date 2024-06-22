# makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# diretório para os arquivos .class
OUT_DIR = out

# lista de arquivos Java
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java

# diretório com os arquivos em c do trabalho de arquivos
C_FILE_DIR = trabalho_arquivos

# compilação do frontend em Java e execução do backend em Python
.PHONY: all
all: compile

compile: $(OUT_DIR)
	javac -d $(OUT_DIR) $(JAVA_FILES)

# criando diretório de saída para os arquivos .class
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# executando frontend em Java e backend em Python
.PHONY: run
run: all
	python3 Socketshandler.py &
	java -cp $(OUT_DIR) Main

# limpando os arquivos .class gerados na compilação
.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
	-@pkill -f Socketshandler.py

clean_bin:
	rm -rf $(C_FILE_DIR)/*.bin
