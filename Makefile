# makefile para compilar o frontend em Java e executar o backend em Python e o frontend

# diretório para os arquivos .class
OUT_DIR = out

# javas a serem compilados
JAVA_FILES = Main.java FIFAFetch.java FIFAFetchGUI.java

# diretório com os arquivos em C do trabalho de arquivos
C_FILE_DIR = trabalho_arquivos

# detectando o sistema operacional
ifeq ($(OS), Windows_NT)
    RM = Remove-Item -Force -Recurse 
	 RM_DIR = powershell -Command "Remove-Item -Force -Recurse '$(OUT_DIR)' -ErrorAction SilentlyContinue" 
    RM_BIN = powershell -Command "Remove-Item -Force -Recurse '$(C_FILE_DIR)/*.bin' -ErrorAction SilentlyContinue" 
	 MKDIR = mkdir
    PYTHON = python
    KILL = cmd /c "taskkill /F /IM python.exe /T"
    RUN_JAVA = cmd /c "java -cp $(OUT_DIR) Main"
else
    RM = rm -rf
	 RM_DIR = rm -rf $(OUT_DIR)
	 RM_BIN = rm -rf $(C_FILE_DIR)/*.bin
    MKDIR = mkdir -p
    PYTHON = python3
    KILL = pkill -f Socketshandler.py
    RUN_JAVA = java -cp $(OUT_DIR) Main
endif

# compilando o frontend em Java e execução do backend em Python
.PHONY: all
all: compile

compile: $(OUT_DIR)
	@echo "Compiling Java files..."
	javac -d $(OUT_DIR) $(JAVA_FILES)

# criando diretório de saída para os arquivos .class
$(OUT_DIR):
	@echo "Creating output directory..."
	$(MKDIR) $(OUT_DIR)

# executando apenas o servidor Python
.PHONY: run_server
run_server:
	@echo "Running Python server..."
	$(PYTHON) Socketshandler.py &

# executando apenas a interface Java
.PHONY: run_interface
run_interface:
	@echo "Running Java interface..."
	$(RUN_JAVA)

# executando a interface em segundo plano e rodando o servidor no terminal
.PHONY: run
run: clean compile
	@echo "Running Python server in background..."
	$(PYTHON) Socketshandler.py &
	@echo "Running Java interface in terminal..."
	$(RUN_JAVA) 
	

# limpando os arquivos .class gerados na compilação
.PHONY: clean
clean:
	@echo "Cleaning up generated files..."
	$(RM_DIR)
	-@$(KILL)

clean_bin:
	@echo "Cleaning up binary files..."
	$(RM_BIN)
