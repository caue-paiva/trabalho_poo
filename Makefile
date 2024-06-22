# makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# diretório para os arquivos .class
OUT_DIR = out

# lista de arquivos Java
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java
C_FILE_DIR =trabalho_arquivos

# compilação do frontend em Java e execução do backend em Python
.PHONY: all
all: compile
	python3 Socketshandler.py &

compile: $(OUT_DIR)
	javac -d $(OUT_DIR) $(JAVA_FILES)

# criando diretório de saída para os arquivos .class
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# executando frontend em Java e dependência da limpeza e compilação
.PHONY: run
run: clean all
	java -cp $(OUT_DIR) Main

# limpando os arquivos .class gerados na compilação
.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
	-@pkill -f Socketshandler.py

clean_bin:
	rm -rf $(C_FILE_DIR)/*.bin

