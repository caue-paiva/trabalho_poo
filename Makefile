# Makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# Diretório para os arquivos .class
OUT_DIR = out

# Lista de arquivos Java
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java

# Compilação do frontend em Java
.PHONY: all
all: compile

compile: $(OUT_DIR)
	javac -d $(OUT_DIR) $(JAVA_FILES)

# Criação do diretório de saída para os arquivos .class
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Execução do backend em Python e do frontend em Java
.PHONY: run
run: kill_python
	python3 Socketshandler.py &
	java -cp $(OUT_DIR) Main

# Finaliza qualquer processo Python em execução
.PHONY: kill_python
kill_python:
	pkill -f Socketshandler.py || true

# Limpeza dos arquivos .class gerados pela compilação do Java
.PHONY: clean
clean:
	rm -rf $(OUT_DIR)
