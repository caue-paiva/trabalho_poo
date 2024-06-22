# Makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# Diretório para os arquivos .class
OUT_DIR = out

# Lista de arquivos Java
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java

# Diretório com os arquivos em C do trabalho de arquivos
C_FILE_DIR = trabalho_arquivos

# Detectando o sistema operacional
ifeq ($(OS), Windows_NT)
    RM = rmdir /S /Q
    MKDIR = mkdir
    PYTHON = python
    KILL = taskkill /F /IM python.exe
    RUN_JAVA = cmd /c "java -cp $(OUT_DIR) Main"
else
    RM = rm -rf
    MKDIR = mkdir -p
    PYTHON = python3
    KILL = pkill -f Socketshandler.py
    RUN_JAVA = java -cp $(OUT_DIR) Main
endif

# Compilação do frontend em Java e execução do backend em Python
.PHONY: all
all: compile

compile: $(OUT_DIR)
	@echo "Compiling Java files..."
	javac -d $(OUT_DIR) $(JAVA_FILES)

# Criando diretório de saída para os arquivos .class
$(OUT_DIR):
	@echo "Creating output directory..."
	$(MKDIR) $(OUT_DIR)

# Executando apenas o servidor Python
.PHONY: run_server
run_server:
	@echo "Running Python server..."
	$(PYTHON) Socketshandler.py &

# Executando apenas a interface Java
.PHONY: run_interface
run_interface:
	@echo "Running Java interface..."
	$(RUN_JAVA)

# Limpando os arquivos .class gerados na compilação
.PHONY: clean
clean:
	@echo "Cleaning up generated files..."
	$(RM) $(OUT_DIR)
	-@$(KILL)

clean_bin:
	@echo "Cleaning up binary files..."
	$(RM) $(C_FILE_DIR)/*.bin
