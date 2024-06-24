
# Projeto Banco de Dados FIFA

## Desenvolvedores do Projeto:

* [Cauê Paiva Lira](https://github.com/caue-paiva)
* [Luiz Felipe Diniz Costa](https://github.com/lfelipediniz)
* [João Pedro Alves Notari Godoy](https://github.com/joaopgodoy)

## Descrição do Projeto

O Projeto Banco de Dados FIFA é um aplicativo desktop, multiplataforma, desenvolvido em Java Swing para uma interface gráfica amigável. Ele permite a manipulação de dados de jogadores FIFA entre os anos 2017-23. Este projeto usa comunicação por socket em Python para interagir com um backend desenvolvido em linguagem C para o curso de Organização de Arquivos, também oferecido pela nossa universidade.

### Funcionalidades

- **Carregar dados**: Carrega arquivos de dados de jogadores FIFA entre os anos 2017-23, esse arquivo foi disponibilizado na disciplina de arquivos.
- **Pesquisar**: Pesquisa jogadores usando vários critérios.
- **Visualizar e Editar**: Visualiza informações detalhadas dos jogadores, edita e atualiza registros.
- **Excluir**: Remove registros de jogadores do banco de dados.
- **Comunicação por socket**: A interface Java se conecta a um servidor Python, que se comunica com um programa em C para realizar operações nos arquivos de dados.

## Como o Projeto Funciona

### Visão Geral
O projeto integra uma interface Java Swing, um servidor de socket em Python e um backend em C. Veja como esses componentes interagem:

### Interface Gráfica do Usuário (GUI)
- **Main.java**: Inicializa a interface gráfica do usuário.
- **FIFAFetch.java**: Gerencia as conexões de socket para transmissão de dados.
- **FIFAFetchGUI.java**: Gerencia todas as interações do usuário para manipulação de dados, incluindo carregar, pesquisar, visualizar, editar e excluir dados de jogadores.
- **FIFAPlayer.java**: Estrutura os dados dos jogadores para operações internas.

### Servidor e Comunicação
- **Socketshandler.py**: Opera o servidor de socket para comunicações com a GUI.
- **ClientMessagesHandler.py**: Processa comandos da GUI e coordena com o backend em C.
- **TrabalhoArquivosHandler.py**: Gerencia diretamente as operações de arquivos com base nas solicitações do servidor.

### Manipulação de Dados
- O backend em C manipula diretamente os arquivos de dados dos jogadores, garantindo operações eficientes.

### Fluxo de Trabalho
1. Ações do usuário na GUI acionam comandos de operação de dados.
2. Os comandos são enviados por socket para o servidor Python.
3. O servidor processa essas solicitações, realiza operações de arquivo e envia os resultados de volta.
4. A GUI é atualizada para refletir as mudanças, proporcionando interação em tempo real.

Esta arquitetura modular melhora a robustez e a manutenção do sistema, isolando as responsabilidades de cada módulo.

## Instalação

### Requisitos

- [Java](https://www.oracle.com/java/technologies/javase-downloads.html) ou [OpenJDK](https://openjdk.java.net/)
- [Python](https://www.python.org/)
- [GCC](https://gcc.gnu.org/)
- [Make](https://www.gnu.org/software/make/)

### Linux

Configurar o ambiente no Linux é direto. Consulte a documentação oficial ou recursos online se encontrar algum problema.

### Windows

#### Passo 1: Instalar Make

1. Instale o Chocolatey a partir [aqui](https://chocolatey.org/install).
2. Abra o PowerShell como administrador e execute:

    ```powershell
    choco install make
    ```

3. Se Python ou OpenJDK/Java não estiverem instalados, você pode instalá-los via Chocolatey também.

#### Passo 2: Instalar GCC

1. Baixe o MinGW do [SourceForge](https://sourceforge.net/projects/mingw/).
2. Execute o instalador como administrador e siga os passos de instalação. Na tela do gerenciador de instalação, selecione a opção com `gcc - g++`.

    <p align="center">
      <img src="readmeImgs/z.png" alt="Instalação do GCC">
    </p>

3. Vá ao menu "Installation" no topo e clique em "Apply Changes".

#### Passo 3: Configurar Variáveis de Ambiente

1. Procure por "Environment Variables" no menu Iniciar e abra.

    <p align="center">
      <img src="readmeImgs/w.png" alt="Variáveis de Ambiente">
    </p>

2. Clique em "Environment Variables".
3. Selecione a variável "Path" e clique em "Edit".

    <p align="center">
      <img src="readmeImgs/a.png" alt="Editar Path">
    </p>

4. Adicione uma nova entrada com o caminho onde o GCC foi instalado, tipicamente: ``C:\MinGWin``

5. Confirme as alterações. Para verificar a instalação, abra o PowerShell e execute:

    ```bash
    gcc --version
    ```

    <p align="center">
      <img src="readmeImgs/c.png" alt="Versão do GCC">
    </p>

6. Reinicie o computador para garantir que todas as alterações tenham efeito.

## Uso

### Clonar o Repositório

1. Use Git Clone ou baixe um arquivo zip
    ```bash
    git clone https://github.com/caue-paiva/trabalho_poo.git
    ```
2. Entre na pasta
    ```bash
    cd trabalho_poo
    ```

### Executar o Programa

#### Linux

1. Construa o projeto
    ```bash
    make compile
    ```

2. Execute o servidor e a interface
    ```bash
    make run
    ```

#### Windows

1. Limpe construções anteriores (importante para soquete Python)
    ```powershell
    make clean
    ```

2. Construa o projeto
    ```powershell
    make compile
    ```

3. Execute o servidor
    ```powershell
    make run_server
    ```

4. Em outra janela do PowerShell, execute a interface:
    ```powershell
    make run_interface
    ```

## Estrutura do Projeto

### Interface Gráfica do Usuário (GUI)

- Mostrar todos os Jogadores no arquivo FIFA 17-23
- Pesquisar jogadores por ID, idade, nome, nacionalidade e clube.
- Exibir resultados de pesquisa e permitir seleção para visualização detalhada e edição.
- Excluir registros de jogadores.

### Servidor e Comunicação

- A GUI se conecta a um servidor Python usando socket.
- O servidor Python traduz comandos e interage com um programa em C para manipular os arquivos de dados.
- Cada conexão da GUI tem sua própria cópia do arquivo indexado.

## Notas Importantes

- O sistema é projetado para rodar tanto no Windows quanto no Linux.

## Informações Adicionais

Projeto desenvolvido para o curso de [Programação Orientada a Objetos](https://uspdigital.usp.br/jupiterweb/obterDisciplina?sgldis=SSC0103&codcur=55041&codhab=0) no Instituto de Matemática e Computação, Universidade de São Paulo.