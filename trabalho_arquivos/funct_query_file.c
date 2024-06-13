#include "funct_query_file.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/

/*
Arquivo para a lógica de busca em um arquivo de registros, implementa 3 funções internas para auxiliar 1 função externa a realizas as buscas

*/

//dado um registro de busca (com os campos buscados como válidos e o resto NULO) e um arquivo aberto, imprime todos os registros
//que correspondem a busca e retorna o FILE* fp para o começo
bool __query_busca_arquivo(const Registro* reg_busca,FILE* fp, bool* flag_inconsistente){
   assert_3ptrs(fp,reg_busca,flag_inconsistente);

   Header* buffer_header = rgt_header_ler_arquivo(fp); //le o header do arquivo, avançando o FILE* fp para os registros
   if (buffer_header->status == INCONSISTENT_HEADER){
      printf(DEFAULT_ERROR_MSG);
      fclose(fp);
      *flag_inconsistente = true;
      return false;
   }
   
   bool achou = false; //flag para ditar se algum registro condizente a busca foi encontrado
   int bytes_lidos;

   while(1){ //loop ate houver falha na leitura de um registro ou chegar no fim do arquivo
      bool removido_flag = false;    //flag de caso o registro esteja removido, para saber se o NULL veio de um removido ou de um erro/fim do arquivo
      
      Registro* reg_lido = rgt_registro_ler_arquivo(fp,&removido_flag,&bytes_lidos); //le um registro do arquivo

      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (removido_flag){ //o registro lido foi removido, continuar o loop
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }

      if (rgt_registro_compara(reg_busca,reg_lido)){ //compara o registro buscado e o lido
            rgt_registro_print(reg_lido); //caso o reg lido tenha as características do buscado, vamos printar ele e seta a flag de achado como true
            achou = true;
            if (reg_busca->id != FIX_CAMP_NULL_FLAG ){ //ID não é nulo e tivemos um match na busca, paramos o loop por que o ID é único
                 rgt_registro_destruir(&reg_lido); //libera memória do registroo
                 break; //como o ID é único, vamos dar break do loop
            } 
         }
         rgt_registro_destruir(&reg_lido); //libera memoria alocada pro registro
   }

   rgt_header_destruir(&buffer_header); //libera memoria do header

   int result_seek1 = fseek(fp,0,SEEK_SET); //volta o ponteiro do arquivo para o começo
   b_errno_check(result_seek1); //retorna false se o resultado do seek der errado
   
   return achou;
}

//Processa o input do usuario para busca e cria um registro com os campos lidos, chamando a função __query_busca_arquivo() para buscar registros
//com esses parâmetros, retorna false se o registro não foi encontrado
Registro* query_processa_linha_comando(const int num_busca){

   int idade  = -1; //valores dos campos inicializados como nulos
   int id = -1;
   char nome[MAX_FIELD_SIZE];
   nome[0] = '\0';
   char nacionalidade[MAX_FIELD_SIZE];
   nacionalidade[0] = '\0';
   char clube[MAX_FIELD_SIZE];
   clube[0] = '\0';
 
   int numero_campos; //numero de campos da busca
   char campo_buffer[MAX_FIELD_SIZE]; //buffer para ler os campos

   scanf("%d",&numero_campos); //le o numero de campos
   for (int i = 0; i < numero_campos; i++){ //loop pelo numero de campos da busca
      scanf("%s",campo_buffer); //le o nome do campo

      if(strcmp(campo_buffer, "id") == 0){ //vamos ver qual o tipo do campo que vamos ler
         scanf("%d",&id); //le campos inteiros
      }
      else if(strcmp(campo_buffer, "idade") == 0){
         scanf("%d",&idade);
      }
      else if(strcmp(campo_buffer, "nomeJogador") == 0){
         misc_scan_quote_string(nome); //le campos de string entre ""
      }
      else if(strcmp(campo_buffer, "nacionalidade") == 0){
         misc_scan_quote_string(nacionalidade);
      }
      else if(strcmp(campo_buffer, "nomeClube") == 0){
         misc_scan_quote_string(clube);
      }
    
   }

   if (num_busca != -1)
      printf("Busca %d\n\n",num_busca); //print o numero da busca
   Registro* reg_busca = rgt_registro_criar(id,idade,nome,nacionalidade,clube); //cria registro com esses dados
   
   return reg_busca;
}

//le a linha de comando do terminal e cria um registro com esses dados, usado na funcionalidade 6 (Inserção)
Registro* query_processa_linha_comando_insercao(){

   const char campo_nulo[] = "NULO"; //para identificar os campos nulos no input
   char linha[MAX_QUERY_LINE_SIZE]; //buffer para a linha lida, e uma cópia dela
   linha[0] = '\0'; //coloca \0 pra virar uma string vazia
   char linha2[MAX_QUERY_LINE_SIZE];
   linha2[0] = '\0';

   int idade  = FIX_CAMP_NULL_FLAG; //valores dos campos inicializados como nulos
   int id = FIX_CAMP_NULL_FLAG;
   char nome[MAX_FIELD_SIZE];
   nome[0] = '\0';
   char nacionalidade[MAX_FIELD_SIZE];
   nacionalidade[0] = '\0';
   char clube[MAX_FIELD_SIZE];
   clube[0] = '\0';

   bool nomeNulo,naciNulo,clubeNulo = false; //flags para dizer se esses campos são nulos ou não
 
   fgets(linha, sizeof(linha), stdin); //ao usar o fgets: o que tem na posição strlen: \0 , oq tem na posição strlen-1: \n, remover esse \n
   if (linha != NULL){
      misc_remove_chars_lixo(linha); //remove newlines e carriage return da linha lida
   } else { //fgets leu uma linha null
      warn_printf("fgets leu uma linha nula, falha no processamento\n");
      return NULL;
   }

   strcpy(linha2,linha); //linha2 é um segundo buffer usado na segunda passada do STRTOK
   
   char* campo = strtok(linha, " "); //le o ID
   if (campo != NULL && strcmp(campo,campo_nulo) != 0) {
      id = atoi(campo); //transforma em int
   }
   campo = strtok(NULL, " "); //le a idade
   if (campo != NULL && strcmp(campo,campo_nulo) != 0) {
      idade = atoi(campo);
   }

   //passada inicial do STRTOK para ver quais campos variáveis são escritos como NULO ou não
   campo = strtok(NULL, " ");
   if (campo != NULL) {
      if (strcmp(campo, campo_nulo) == 0) { //campo é escrito NULO
         nomeNulo = 1; //seta a flag
      } else {
         nomeNulo = 0; //campo não é nulo
         char final = campo[strlen(campo)-1]; //ve se o campo acaba com "
         if (campo[0] == '"' && final != '"') { //se ele for um campo variável com aspas e espaço no meio ex "L. WALSH" então é preciso dar um strtok de aspas
            campo = strtok(NULL, "\""); //para chegar no final desse campo, caso n tenha espaço, n precisa
         }
      }
   }

   campo = strtok(NULL, " "); //mesma lógica de acima 
   if (campo != NULL) {
      if (strcmp(campo, campo_nulo) == 0) {
         naciNulo = 1;
      } else {
         naciNulo = 0;
         char final = campo[strlen(campo)-1];
         if (campo[0] == '"' && final != '"') {
            campo = strtok(NULL, "\"");
         }
      }
   }

   campo = strtok(NULL, " "); //mesma lógica de acima
   if (campo != NULL) {
      if (strcmp(campo, campo_nulo) == 0) {
         clubeNulo = 1;
      } else {
         clubeNulo = 0;
         char final = campo[strlen(campo)-1];
         if (campo[0] == '"' && final != '"') {
            campo = strtok(NULL, "\"");
         }
      }
   }

   //passada final para pegar os campos de tam variável
   char* campo2 = strtok(linha2, " "); //novo strtok e pula id
   campo2 = strtok(NULL, " "); //pula idade

   if (!nomeNulo){ //campo não nulo
      campo2 = strtok(NULL, "\""); //separa ele por aspas
      strcpy(nome,campo2); //copia valor pro buffer
      if (!naciNulo) //se o proximo não for nulo, chama strtok denovo pra "consumir" o espaço vazio entre as aspas dos 2 campos
         campo2 = strtok(NULL, "\"");
   }else 
      campo2 = strtok(NULL, " "); //caso o campo seja nulo, faz strtok do whitespace
   
   if (!naciNulo){ //mesma lógica de acima 
      campo2= strtok(NULL, "\"");
      strcpy(nacionalidade,campo2);
      if (!clubeNulo)
         campo2= strtok(NULL, "\"");
   }else 
      campo2 = strtok(NULL, " ");
   
   if (!clubeNulo){
      campo2 = strtok(NULL, "\"");
      strcpy(clube,campo2);
   }else 
      campo2 = strtok(NULL, " ");

   Registro* reg_busca = rgt_registro_criar(id,idade,nome,nacionalidade,clube); //cria registro com esses dados
   return reg_busca;
}

//Dado o nome de um arquivo e quantas buscas serão feitas, realiza essa busca, returna false caso ocorra ERRO na busca
//ABRE E FECHA ARQUIVO
bool query_realizar_pesquisa(const char* nome_arquivo, const int numero_queries){
   assert(nome_arquivo);
   assert(numero_queries > 0); //numero de queries tem que ser maior que 0

   FILE* fp = fopen(nome_arquivo,R); //tenta abrir arquivo
   if (!fp){
      printf("Registro inexistente.\n\n");
      return false;
   } //retorna false para significar que a operação falhou
   //a verificação da consistência do arquivo ocorre na função __query_busca_arquivo  

   for (int i = 0; i < numero_queries; i++){ //loop pelo número de buscas
     bool arq_inconsistente = false; 
     Registro* reg_busca = query_processa_linha_comando(i+1); //chama a função para pegar a linha e realizar realizar a busca
     bool resultado_busca = __query_busca_arquivo(reg_busca, fp, &arq_inconsistente); //resultado da busca no arquivo
     rgt_registro_destruir(&reg_busca); //free na memoria do registro criado pra busca
     
     if (arq_inconsistente) //arquivo ta inconsistente, a msg de erro foi printada ja e o fp foi fechado tbm so retorna false
         return false;
     
     if (!resultado_busca)
         printf("Registro inexistente.\n\n"); //não achou nenhum registro correspondente
   }

   fclose(fp); //fecha o arquivo
   return true; //nao ocorreu nenhum erro
}


//Funcionalidade de busca relacionadas com a remoção:


//faz uma busca sequencial no arquivo e preenche 2 listas encad, uma com os matches da busca e uma com os registros logicamente removidos 
//precisa de um fp no começo do arquivo e dá fseek pro começo
void query_busca_arquivo_e_removidos(const Registro* reg_busca , FILE* fp, ListaEncad* lista_matches, ListaEncad* lista_removidos){
   assert_3ptrs(fp,lista_matches,lista_removidos);
   assert(reg_busca);

   long int byte_offset_anterior = 0; //variavel para guardar qual o byteoffset atual
   Header* buffer_header = rgt_header_ler_arquivo(fp); //le o header do arquivo, avançando o FILE* fp para os registros
  
   byte_offset_anterior += HEADER_SIZE; //avança o offset pelo tamanho do header (já lido)
  
   while(1){ //loop ate houver falha na leitura de um registro ou chegar no fim do arquivo
      bool removido_flag = false;    //flag de caso o registro esteja removido, para saber se o NULL veio de um removido ou de um erro/fim do arquivo
      
      int bytes_lidos = 0; //vai receber o valor na função abaixo de quantos bytes foram lidos
      Registro* reg_lido = rgt_registro_ler_arquivo(fp,&removido_flag,&bytes_lidos); //le um registro do arquivo
      
      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (removido_flag){ //o registro lido foi removido
         li_encad_add_ordem(lista_removidos,byte_offset_anterior,reg_lido->tamanhoRegistro,reg_lido->id); //add na lista encadeada em ordem
         byte_offset_anterior += bytes_lidos;
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }

      if (rgt_registro_compara(reg_busca,reg_lido)){ //compara o registro buscado e o lido, se forem iguais, coloca na lista de removidos
         li_encad_add_ordem(lista_matches,byte_offset_anterior,reg_lido->tamanhoRegistro,reg_lido->id);
      } 
      
      byte_offset_anterior += bytes_lidos; //byteoffset anterior é avançado
      rgt_registro_destruir(&reg_lido); //libera memoria alocada pro registro
   }

   rgt_header_destruir(&buffer_header); //libera memoria do header

   int result_seek1 = fseek(fp,0,SEEK_SET); //volta o ponteiro do arquivo para o começo
   errno_check(result_seek1); //verifica se o resultado do seek der errado
}


//cria uma lista encad com os registros marcados como removidos, o FP tem que estar no começo do arquivo.
//no final da fseek pro começo
ListaEncad* query_lista_removidos_arquivo(FILE* fp){
   assert(fp);

   ListaEncad* lista = li_encad_criar(); //cria lista vazia
   Header* header = rgt_header_ler_arquivo(fp); //le o header

   if(header->topo != -1){  //caso não tenha removidos no arquivo retorna lista vazia, senão vai buscar no arquivo a lista
      long int byte_offset_atual = header->topo; //byteoffset começa do prox do header

      while(byte_offset_atual != -1){

         fseek(fp,byte_offset_atual,SEEK_SET);//fseek pro offset atual

         char buffer; //buffer para ler o status e o tamanho do registro
         int read_status = fread(&buffer,sizeof(char),1,fp); //le o status do registro
         if (read_status != 1){
            d_printf("erro no fread");
            break;
         }
         if (buffer != REMOVED_REG){
            warn_printf("Existe um registro na lista de encadeado de removidos que nao está marcado como logicamente removido");
            break;
         }

         int tam_registro;
         read_status = fread(&tam_registro,sizeof(int),1,fp); //le o tamanho do registro
         if (read_status != 1){
            d_printf("erro no fread");
            break;
         }

         long int prox_offset;
         read_status = fread(&prox_offset,sizeof(long int),1,fp);
         if (read_status != 1){
               d_printf("erro no fread");
               break;
         }

         int id;
         read_status = fread(&id,sizeof(int),1,fp);
         if (read_status != 1){
               d_printf("erro no fread");
               break;
         }

         li_encad_add_ordem(lista,byte_offset_atual,tam_registro,id);
         byte_offset_atual = prox_offset;
      }
   }

   rgt_header_destruir(&header); //libera memoria do header
   int result_seek1 = fseek(fp,0,SEEK_SET); //volta o ponteiro do arquivo para o começo
   b_errno_check(result_seek1); //retorna false se o resultado do seek der errado
   return lista;
}

