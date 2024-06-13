#include "funcoes_miscelaneas.h"



//MISC, funções de uso geral


//remove \n, carriage return e outras chars de uma string
void misc_remove_chars_lixo(char* linha) {
    int tam = strlen(linha); //pega o tamanho da linha
    while (tam > 0 && ( linha[tam - 1] < 33 || linha[tam - 1] == 127)) { //33 até 126 são chars do alfabeto ou simbolos legíveis como !?~^, o resto é espaço, \n,\0,\r...
        tam--; //vai pra um index menor da str
        linha[tam] = '\0'; //coloca \0 no lugar 
    }
}

void misc_scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		misc_scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char r;

	while((r = getchar()) != EOF && isspace(r)); // ignorar espaços, \r, \n...

	if(r == 'N' || r == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(r == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(r != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = r;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

//true se a str for null ou apenas espaço vazio
bool misc_str_somente_whitespace(const char *linha) {
    if (!linha) //se a str for null
        return true;
    while (*linha != '\0') {
        if (!isspace((unsigned char)*linha))  //se um char não for whitespace, retorna falso
            return false; 
        linha++; 
    }
    return true; 
}

//remove chars newline da string, necessário pq o fgets coloca um newline na string lida
//INPLACE
void misc_str_remove_newline(char* linha) {
    int tam = strlen(linha); //pega o tamanho da linha
    while (tam > 0 && (linha[tam - 1] == '\n')) { //remove todos os newlines do fim pro começo
        tam--; //vai pra um index menor da str
        linha[tam] = '\0'; //coloca \0 no lugar 
    }
}

void misc_binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */
	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função misc_binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs); 
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	} 
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}






