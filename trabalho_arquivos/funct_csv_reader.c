#include "funct_csv_reader.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/






//INPLACE: Caso exista um campo ,, no CSV, precisamos colocar um espaço entre as virgulas para o strtok reconhecer
void __csv_preprocessar_linha(char *linha) {
   assert(linha); //str n pode ser null
   int tam = strlen(linha); //pega o tamanho da linha
   for (int i = 0; i < tam-1; i++) {
      if (linha[i] == ',' && linha[i+1] == ',') {
         memmove(linha + i + 2, linha + i + 1, tam - i); //shifta os caracteres das str
         linha[i + 1] = ' ';  //coloca o whitespace
         tam++; //tamanho aumenta em um
         i++; //pular o novo whitespace
      }
   }
}

bool csv_salva_em_registros(const char* nome_arquivo_csv,const char* nome_arquivo_bin){
   assert_2ptrs(nome_arquivo_bin,nome_arquivo_csv);

   Header* header = rgt_header_criar(); //cria uma struct header para guardar informações sobre o arquivo, tipo numero de registros apos as escritas, ela depois
   //sera escrita no final da operação de write
  
   FILE* fp_csv = fopen(nome_arquivo_csv, R); //abre o CSV em modo leitura
   if (fp_csv == NULL) {
      printf(DEFAULT_ERROR_MSG);   
      return false;  //printa uma msg de erro e retorna false se o fp for null
   }
   

   FILE* fp_bin = rgt_header_abre_escrita(header,nome_arquivo_bin); //abre o arquivo do handler para escrita, escreve o header como inconsistente e proxByteOffset como 0
   if (fp_bin == NULL) {
      printf(DEFAULT_ERROR_MSG);   
      return false;  //printa uma msg de erro e retorna false se o fp for null
   }

   int registros_escritos = 0; //controle de quantas linhas (e registros vamos ler do CSV)
   header->proxByteOffset += HEADER_SIZE; //agora que escrevemos no arquivo o proxByteOffset como zero, podemos somar o valor de bytes do header em si nessa variavel
   int id;  //campos de tamanho fixo
   int idade;

   const int MAX_LINE_SIZE = MAX_FIELD_SIZE * 3; //tamanho do buffer para a leitura das linhas do CSV
   char linha_buffer[MAX_LINE_SIZE]; //buffers para leitura da linha do CSV e de cada campo variavel
   char nome_buffer[MAX_FIELD_SIZE];
   char nacionali_buffer[MAX_FIELD_SIZE];
   char clube_buffer[MAX_FIELD_SIZE];

   fgets(linha_buffer,MAX_LINE_SIZE,fp_csv); //le a linha inicial das colunas do CSV
   int registros_lidos = 0;
   
   while (fgets(linha_buffer, MAX_LINE_SIZE, fp_csv)){ //le todas as linhas do CSV
     
      __csv_preprocessar_linha(linha_buffer); //coloca espaço entre duas virgulas juntas para o STRTOK funcionar
      misc_str_remove_newline(linha_buffer); //remove o char \n (newline) para que ele não seja escrito nos campos
      char* campo = strtok(linha_buffer,","); //dividir a str em tokens pela virgula
      if (campo && !misc_str_somente_whitespace(campo)) //se o campo não for NULL ou vazio
         id = atoi(campo); //str para inteiro
      else
         id  = -1; //se o campo não existir, a valor é -1
      
      campo = strtok(NULL,","); //proximo campo separado pela virgula
      if (campo && !misc_str_somente_whitespace(campo)) //se o campo não for NULL ou vazio
         idade = atoi(campo);
      else
         idade = -1;
      
      campo = strtok(NULL, ",");
      if (campo && !misc_str_somente_whitespace(campo)) {
         strcpy(nome_buffer, campo); //str não nula e não vazia
      } else {
         strcpy(nome_buffer, ""); //str nula ou vazia
      }
      
      campo = strtok(NULL, ",");
      if (campo && !misc_str_somente_whitespace(campo)) {
         strcpy(nacionali_buffer, campo);
      } else {
         strcpy(nacionali_buffer, "");
      }
      
      campo = strtok(NULL, ",");
      if (campo && !misc_str_somente_whitespace(campo)) {
         strcpy(clube_buffer, campo);   
      } else {
         strcpy(clube_buffer, "");
      }
      
      Registro* reg = rgt_registro_criar(id,idade,nome_buffer,nacionali_buffer,clube_buffer); //cria um registro para armazenar esses dados
      bool resultado_escrita = rgt_header_escreve_registros(header,reg,fp_bin); //escreve o registros no arquivo binário


      if (resultado_escrita){ //caso a escrita do registro tenha sido um sucesso
         registros_escritos++; //mais um registro escrito
         header->proxByteOffset += reg->tamanhoRegistro; //caso a escrita seja com sucesso, proxbyteOffset irá avançar pelo tamanho do registro
      } else { //um dos registros falhou em ser lido
         printf(DEFAULT_ERROR_MSG); //printa erro
         rgt_registro_destruir(&reg); //libera memoria do registro
         fclose(fp_csv); //fecha arquivo CSV
         fclose(fp_bin); //o header do arquivo binario foi modificado para ser inconsistente, o Fhandler tbm tem esse valor, então o arquivo
         //sera modificado e fechado marcado como inconsistente
         return false;
      }
      
      rgt_registro_destruir(&reg); //libera a memória do registro alocado
      registros_lidos++;
   };
   fclose(fp_csv); //fecha o arquivo CSV
   fp_csv = NULL;
   bool sucesso_escrita = rgt_header_fecha_escrita(header,&fp_bin); //atualiza o header e fecha o arquivo binario
   rgt_header_destruir(&header);
   return sucesso_escrita;
}

