#include "tad_reg_variavel.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/

//MISC, funções internas para ajudar as outras funções em diversas funcionalidades, não são expostas no header


//Implementação da strcmp que tem um tamanho máximo para percorrer as 2 strings
//se ate esse tamanho elas forem iguais, retorna true
bool rgt_strcmp_tam_max(const char* str1, const char* str2, const int tam_maximo){
   for (int i = 0; i < tam_maximo; i++){ //loop pelo tamanho máximo
   
      if (str1[i] != str2[i]) //strings diferentes
         return false;
   }

   return true; //string iguais
   
}


//Cria uma struct registro com campos fixos e variaveis, usados para escrever, ler e buscar dados de um registro binario
Registro* rgt_registro_criar(const int id, const int idade, const char* nome, const char*nacionalidade, const char*nomeClube){

      Registro* regis = (Registro*) malloc(sizeof(Registro)); //alocar a memoria pro registro
      assert(regis);

      regis->removido = '0'; //Registro nao está como removido
      regis->Prox = -1; //valor padrão do prox
      regis->id = id;
      regis->idade = idade;
      
      if (nome != NULL && strcmp(nome, "") != 0){ //caso o campo nome seja valido
         int tamNomeJog = strlen(nome); //pega o tamanho do nome, sem contar com \0 
         regis->tamNomeJog = tamNomeJog; //configura o tam do nome
         regis->nomeJogador = (char*) malloc(sizeof(char)* tamNomeJog); //aloca espaço para a string nome
         memcpy(regis->nomeJogador,nome,tamNomeJog); //copia o argumento nome para o campo nome, usando o memcpy pq o strcpy usa o \0, que nao vamos usar nesses campos
      } else //caso ele seja vazio
         regis->tamNomeJog = VAR_CAMP_NULL_FLAG;
      

      if (nacionalidade != NULL && strcmp(nacionalidade, "") != 0){ //mesma logica do nome, mas com a nacionalidade
         int tamNacionalidade = strlen(nacionalidade);
         regis->tamNacionalidade = tamNacionalidade;
         regis->nacionalidade = (char*) malloc(sizeof(char)* tamNacionalidade);
         memcpy(regis->nacionalidade ,nacionalidade,tamNacionalidade);
      } else
          regis->tamNacionalidade = VAR_CAMP_NULL_FLAG;

      if (nomeClube != NULL && strcmp(nomeClube, "") != 0){ //mesma logica do nome, mas com o nomeClube
         int tamNomeClube = strlen(nomeClube);
         regis->tamNomeClube = tamNomeClube;
         regis->nomeClube = (char*) malloc(sizeof(char)* tamNomeClube);
         memcpy(regis->nomeClube,nomeClube,tamNomeClube);
      } else
         regis->tamNomeClube = VAR_CAMP_NULL_FLAG;
      

      int tamanho_campos_fixo = sizeof(char) + (6*sizeof(int)) + sizeof(long int); //conta os os campos que guardam o tamanho dos campos variaveis
      int tamanho_campos_variaveis = regis->tamNomeClube + regis->tamNacionalidade + regis->tamNomeJog;
      regis->tamanhoRegistro = tamanho_campos_fixo + tamanho_campos_variaveis; //tamanho total do registro

      return regis;
}

//Printa os conteudos de uma struct registro
void rgt_registro_print(const Registro* registro){
   assert(registro);

   char nome_buffer[MAX_FIELD_SIZE]; //buffers para os campos de tamanho variável
   char nacionali_buffer[MAX_FIELD_SIZE];
   char clube_buffer[MAX_FIELD_SIZE];

   char sem_dado[] = "SEM DADO\0"; //string caso o campo nao exista

   if (registro->tamNomeJog == 0)
      strcpy(nome_buffer,sem_dado);
   else{
      strcpy(nome_buffer,registro->nomeJogador);
      nome_buffer[registro->tamNomeJog] = '\0';

   }

   if (registro->tamNacionalidade== 0)
      strcpy(nacionali_buffer,sem_dado);
   else{
      strcpy(nacionali_buffer,registro->nacionalidade);
      nacionali_buffer[registro->tamNacionalidade] = '\0';
   }

   if (registro->tamNomeClube == 0)
      strcpy(clube_buffer,sem_dado);
   else{
      strcpy(clube_buffer,registro->nomeClube);
      clube_buffer[registro->tamNomeClube] = '\0';
   }

   printf("Nome do Jogador: %s\n", nome_buffer);
   printf("Nacionalidade do Jogador: %s\n", nacionali_buffer);
   printf("Clube do Jogador: %s\n", clube_buffer);
   printf("\n");
}

//Dado um registro principal, vamos comparar seus campos validos com o de um registro auxiliar
bool rgt_registro_compara(const Registro* reg_principal, const Registro* reg_auxiliar){
   assert_2ptrs(reg_principal,reg_auxiliar);
   bool nao_todos_nulos = false; //flag para ditar se todos os campos não são nulos

   //campos fixos
   if (reg_principal->id != FIX_CAMP_NULL_FLAG) { //caso o registro principal tenha conteudo nao nulo
      if (reg_auxiliar->id == FIX_CAMP_NULL_FLAG) //se o registro principal é valido e o outro é nulo, a comparação é falsa
         return false;
      if (reg_principal->id != reg_auxiliar->id) //se tiverem conteudos validos mas diferentes
         return false;
      else
        nao_todos_nulos = true; //nesse caso nem todos são nulos
   }
   
   if (reg_principal->idade != FIX_CAMP_NULL_FLAG) { //mesma logica mas para o outros campos
      if (reg_auxiliar->id == FIX_CAMP_NULL_FLAG)
         return false;
      if (reg_principal->idade != reg_auxiliar->idade) {
         return false;
      } else {
         nao_todos_nulos = true;
      }
   }
   
   //campos variaveis, vamos usar uma função de comparar strings
   if (reg_principal->tamNomeJog != VAR_CAMP_NULL_FLAG ) {
      if (reg_auxiliar->tamNomeJog == VAR_CAMP_NULL_FLAG)
         return false;
      
      if (!rgt_strcmp_tam_max(reg_principal->nomeJogador, reg_auxiliar->nomeJogador,reg_principal->tamNomeJog)) {
         return false;
      } else {
         nao_todos_nulos = true;
      }
   }
   
   if (reg_principal->tamNacionalidade != VAR_CAMP_NULL_FLAG ) {
      if (reg_auxiliar->tamNacionalidade == VAR_CAMP_NULL_FLAG)
         return false;
      
      if (!rgt_strcmp_tam_max(reg_principal->nacionalidade, reg_auxiliar->nacionalidade,reg_principal->tamNacionalidade)) {
         return false;
      } else {
         nao_todos_nulos = true;
      }
   }
   
   if (reg_principal->tamNomeClube != VAR_CAMP_NULL_FLAG) {
      if (reg_auxiliar->tamNomeClube == VAR_CAMP_NULL_FLAG)
         return false;
      if (!rgt_strcmp_tam_max(reg_principal->nomeClube, reg_auxiliar->nomeClube,reg_principal->tamNomeClube)) {
         return false;
      } else {
         nao_todos_nulos = true;
      }
   }
   
   return nao_todos_nulos; //se chegarmos nesse ponto, não retornamos false e temos campos não nulos no reg principal, retorna true
}

//libera a memoria de um registro e seta seu ponteiro pra NULL
void rgt_registro_destruir(Registro** registro){
   assert_2ptrs(registro, *registro);

   if ((*registro)->tamNomeJog != VAR_CAMP_NULL_FLAG) //Apenas da free nas strings dos campos variaveis se elas existirem
      free((*registro)->nomeJogador);

   if ((*registro)->tamNacionalidade != VAR_CAMP_NULL_FLAG)
      free((*registro)->nacionalidade);

   if ((*registro)->tamNomeClube != VAR_CAMP_NULL_FLAG)
      free((*registro)->nomeClube);

   free(*registro);
   *registro = NULL;
}

//Dado um FILE* fp, le os conteudos de um registro no disco e retorna uma struct registro com esses dados, retorna NULL caso
//a operação falhe. Caso um registro seja removido, le campos importantes como tamanho e prox e retorna o registro, mas seta uma flag 
//passada como argumento como true, permitindo identificar registros que foram removidos
//MOVE O *fp
Registro* rgt_registro_ler_arquivo(FILE* fp, bool* removido_flag, int* bytes_lidos){
   assert_3ptrs(fp,removido_flag,bytes_lidos);
   *bytes_lidos = 0; //seta a variavel como zero

   char removido; //variaveis buffers para leitura
   int tamanhoRegistro, id, idade, tamNomeJog,tamNacionalidade, tamNomeClube;
   long int Prox;

   char nome_buffer[MAX_FIELD_SIZE]; //buffers para os campos de tamanho variável
   char nacionali_buffer[MAX_FIELD_SIZE];
   char clube_buffer[MAX_FIELD_SIZE];

   char sem_dado[] = "SEM DADO\0"; //string caso o campo nao exista

   int campos_lidos = 0; //variavel para ver quantos campos sao lidos, usados para ver erros nas funcoes de I/O de arquivos

   campos_lidos = fread(&removido,sizeof(char),1,fp); //le o status do arquivo, se foi removido ou não
   if (campos_lidos == 0){  //nenhum campo foi lido, erro
      return NULL;
   } else {
      (*bytes_lidos) +=1;
   }
        
   if (removido == REMOVED_REG){ //caso o registro esteja removido
         
         int items_lidos = fread(&tamanhoRegistro,sizeof(int),1,fp); //le o tamanho do registro
         if (items_lidos== 0){ //fread leu 0 items, erro, retorna NULL
            return NULL;
         } else {
            (*bytes_lidos) += sizeof(int);
         }

         items_lidos = fread(&Prox,sizeof(long int),1,fp); //le o prox do registro da lista encad de removidos
         if (items_lidos== 0){ //fread leu 0 items, erro, retorna NULL
            return NULL;
         } else {
            (*bytes_lidos) += sizeof(long int);
         } 

         int id;
         items_lidos = fread(&id,sizeof(int),1,fp); //le o id do registro
         if (items_lidos== 0){ //fread leu 0 items, erro, retorna NULL
            return NULL;
         } else {
            (*bytes_lidos) += sizeof(int);
         } 
         
         int tamanho_ler = tamanhoRegistro - sizeof(char) - (2*sizeof(int)) - sizeof(long int); //vamos ler o tamanho do registro menos o campo de status e tamanho já lidos anteriormente
         char buffer_pular[tamanho_ler+1]; //buffer para leitura do fread
         items_lidos = fread(buffer_pular,tamanho_ler,1,fp); //pula pro final do registro
         if (items_lidos == 0){ //fread leu 0 items, erro
            return NULL;
         } else {
            (*bytes_lidos) += tamanho_ler;
         }

         *removido_flag = true; //seta a flag removido para true
         Registro* reg_lido = rgt_registro_criar(id,-1,NULL,NULL,NULL); //cria registro nulo 
         reg_lido->removido = REMOVED_REG; //seta os campos desse registro para guardar os dados de prox e tamanho
         reg_lido->Prox = Prox;
         reg_lido->tamanhoRegistro = tamanhoRegistro;

         return reg_lido; //retorna NULL de pular o removido
   }

      campos_lidos  = fread(&tamanhoRegistro,sizeof(int),1,fp);
      campos_lidos += fread(&Prox,sizeof(long int),1,fp);
      campos_lidos += fread(&id,sizeof(int),1,fp);
      campos_lidos += fread(&idade,sizeof(int),1,fp);
      if (campos_lidos != 4){ //leu menos campos do que deveria, erro
         return NULL;
      } else {
         (*bytes_lidos) += (sizeof(int) *3) + sizeof(long int);
      }

      //campos de tamanho variavel
      fread(&tamNomeJog,sizeof(int),1,fp);
      fread(nome_buffer,sizeof(char),tamNomeJog,fp); //le o nome e guarda no buffer
      (*bytes_lidos) += (sizeof(int) + tamNomeJog);
      if (tamNomeJog == VAR_CAMP_NULL_FLAG)
         strcpy(nome_buffer,sem_dado); //caso nao tenha nenhum dado no campo, copia a msg de SEM DADO para o buffer
      else
         nome_buffer[tamNomeJog] = '\0'; //\0 no final da string do BUFFER, apenas usado para imprimir o dado
  
      fread(&tamNacionalidade,sizeof(int),1,fp);
      fread(nacionali_buffer,sizeof(char),tamNacionalidade,fp); //ler o nome  guarda no buffer
      (*bytes_lidos) += (sizeof(int) + tamNacionalidade);
      if (tamNacionalidade == VAR_CAMP_NULL_FLAG)
         strcpy(nacionali_buffer,sem_dado);
      else
         nacionali_buffer[tamNacionalidade] = '\0';

      fread(&tamNomeClube,sizeof(int),1,fp);
      fread(clube_buffer,sizeof(char),tamNomeClube,fp); //le o nome  e guarda no buffer
      (*bytes_lidos) += (sizeof(int) + tamNomeClube);

      if (tamNomeClube == VAR_CAMP_NULL_FLAG)
         strcpy(clube_buffer,sem_dado);
      else
         clube_buffer[tamNomeClube] = '\0';

      int tam_pular = tamanhoRegistro - (int) (*bytes_lidos);
      if (tam_pular > 0) {
        // d_printf("Antigo removido, tam atual %d \n",tamanhoRegistro);
      }
      char buffer_pular[tam_pular+1];
      file_io_check(fread(buffer_pular,sizeof(char),tam_pular,fp),tam_pular);
      (*bytes_lidos) += tam_pular; //vamos contar tbm os bytes lidos um pular para depois do lixo

      Registro* reg_lido = rgt_registro_criar(id,idade,nome_buffer,nacionali_buffer,clube_buffer); //cria registro a partir dos dados lidos
      reg_lido->tamanhoRegistro = tamanhoRegistro;
      return reg_lido;
}


//dado um registro* e um FILE* fp corretamente posicionado, escreve todos os dados do registro no arquivo campo a campo
bool rgt_registro_escreve_arq(const Registro* regis, FILE* fp){
   assert_2ptrs(fp,regis);
   bool return_result = true;

   //campos de tamanho fixo
   fwrite(&regis->removido,sizeof(char),1,fp);
   fwrite(&regis->tamanhoRegistro,sizeof(int),1,fp);
   fwrite(&regis->Prox,sizeof(long int),1,fp);
   fwrite(&regis->id,sizeof(int),1,fp);
   fwrite(&regis->idade,sizeof(int),1,fp);
   
   int write_result;
   //campos de tamanho variavel, le o tamanho do campo primeiro e usa esse dado pra saber quantos chars serão escritos
   fwrite(&regis->tamNomeJog,sizeof(int),1,fp); //escreve o campo fixo, ja que ele sempre estará la, mesmo que nulo
   if (regis->tamNomeJog > 0){ //vamos escrever apenas se o tamanho do campo for maior que zero
       write_result = fwrite(regis->nomeJogador,sizeof(char),regis->tamNomeJog,fp);
      if (write_result != regis->tamNomeJog) //caso nao tenhamos escrito a qntd certa de chars, retornar false
         return_result = false;
   }

   fwrite(&regis->tamNacionalidade,sizeof(int),1,fp); //mesma coisa pros próximos campos variaveis
   if (regis->tamNacionalidade > 0){
      write_result = fwrite(regis->nacionalidade,sizeof(char),regis->tamNacionalidade,fp);
      if (write_result != regis->tamNacionalidade)
         return_result = false;
   }


   fwrite(&regis->tamNomeClube,sizeof(int),1,fp);
   if (regis->tamNomeClube > 0){
      write_result = fwrite(regis->nomeClube,sizeof(char),regis->tamNomeClube,fp);
      if(write_result != regis->tamNomeClube)
         return_result = false;
   }


   if (!return_result) //se o resultado de retorno for false printa uma mensagem de warning
      warn_printf("Escrita teve alguma falha, algum campo nao foi escrito corretamente");
   

   return return_result;
}