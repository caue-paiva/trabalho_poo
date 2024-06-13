#include "funct_tad_index.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/


#define REGISTRY_LIST_BLOCK 50 //tam do bloco para o array de registros do struct index

//MISC: Funções miscelaneas para auxilio

//função de comparação passado como arg pro quicksort no carregamento do índice
int __func_compara_id(const void* reg1, const void* reg2){
   int id1 =  ((IndexRegistro*)reg1)->id; 
   int id2 =  ((IndexRegistro*)reg2)->id; 

   return (id1 - id2);
}

//DEBUG: abre o arquivo de índice e printar ele
//não usar nas funcionalidades
void __ind_print_index(const char* arquivo_index){
   assert(arquivo_index);

   FILE* fp = fopen(arquivo_index,R);
   null_errno_check(fp);

   char status;
   file_io_check(fread(&status,sizeof(char),1,fp),1);
   if (status == INCONSISTENT_INDEX){
      warn_printf("Index está inconsistente \n");
      return;
   }

   printf(" [  ");
   while (1) {
      int id;
      long int offset;

      int read_result = fread(&id,sizeof(int),1,fp);
      if (read_result != 1){
         d_printf("fread falhou \n");
         break;
      }
      

      read_result = fread(&offset,sizeof(long int),1,fp);
      if (read_result != 1){
         d_printf("fread falhou \n");
         break;
      }

      printf("id: %d , offset: %ld ",id,offset);
   }
   printf("  ]\n");
}

//IN-MEMORY: Funções de manipular a estrutura em memória

//shiftar o array do índice para adição ou remoção (vai alterar a direção do shift)
void __shift_index_arr(Index* index, const int posicao_ser_alterada ,const bool shift_esquerda){
   assert(index); 
   assert(posicao_ser_alterada >= 0 && posicao_ser_alterada < index->num_registros);

   int num_registros_atual = index->num_registros;
   if ( ((num_registros_atual + 1) % REGISTRY_LIST_BLOCK == 0) && !shift_esquerda ){ //precisa fazer realloc se for shift na direita e n tiver espaço
      IndexRegistro* temp = (IndexRegistro*) realloc(index->arr_registros, (num_registros_atual + 1 + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
      if (temp){//realloc com sucesso
            index->arr_registros = temp;
      }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index);
            return;
      }
   }

   if (shift_esquerda) { //shift left para remoção 
      for (int i = posicao_ser_alterada; i < index->num_registros-1; i++){
         IndexRegistro prox = index->arr_registros[i+1];
         index->arr_registros[i] = prox; 
      }

   } else { //shift right para inserção 
      for (int i = index->num_registros-1; i >= posicao_ser_alterada; i--){
         IndexRegistro atual = index->arr_registros[i];
         index->arr_registros[i+1] =  atual; 
      }
   }

}

//dado um id, faz uma busca binário no array e se achar retorna o index desse elemento
int __ind_busca_posi_por_id(const Index* index, const int id){
   assert(index);

   int esq = 0;
   int dir = index->num_registros - 1;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;  // previne overflow
        if (index->arr_registros[meio].id == id) { //achou o id
            return meio;
        } else if (index->arr_registros[meio].id < id) { //id atual é menor
            esq = meio + 1;
        } else { //id atual é maior
            dir = meio - 1;
        }
    }

    return -1; // não achou
}

//printa os valores do index de memória
void ind_print_index_mem(const Index* index_mem){
   assert(index_mem);

   printf(" Status: %c \n Num Elementos: %d", index_mem->header.status, index_mem->num_registros);

   printf(" [");
   for (int i = 0; i < index_mem->num_registros; i++){
      printf("{id: %d ,offset: %ld} ,", index_mem->arr_registros[i].id ,index_mem->arr_registros[i].byteoffset);
   }
   printf("] \n");
}

//libera memória do struct index em RAM
void ind_index_mem_destroi(Index** index){
   assert_2ptrs(index,*index);

   free((*index)->arr_registros);
   free(*index);
   *index=NULL;
}

//realiza uma busca binária no index em memória ordenado para buscar o offset relacionado ao id de argumento
long int ind_busca_offset_por_id(const Index* index, const int id){
   assert(index);

   int esq = 0;
   int dir = index->num_registros - 1;

    while (esq <= dir) { //busca binária
        int meio = esq + (dir - esq) / 2;  // previne overflow
        if (index->arr_registros[meio].id == id) { //achou o id
            return index->arr_registros[meio].byteoffset;
        } else if (index->arr_registros[meio].id < id) { //id atual é menor
            esq = meio + 1;
        } else { //id atual é maior
            dir = meio - 1;
        }
    }

    return -1; // não achou
}

//adiciona um registro ao index em memória
bool ind_index_mem_add(Index* index, const int id, const long int offset){
   assert(index); 

   int num_reg = index->num_registros; //num de registros atuais
   IndexRegistro novo_reg = {.id = id, .byteoffset = offset}; //novo registro a ser inserido

   if ((num_reg+1) % REGISTRY_LIST_BLOCK == 0){ //caso precise reallocar o array do index em memória
      IndexRegistro* temp = (IndexRegistro*) realloc(index->arr_registros, (num_reg + 1 + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
         if (temp){//realloc com sucesso
            index->arr_registros = temp;
         }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index);
            return false;
         }
   }

   for (int i = 0; i < num_reg; i++){ //loop pelos array de registros
      int id_atual = index->arr_registros[i].id;

      if (id_atual == id){ //o id inserido é duplicado, retorna false e n adiciona
         return false;
      }

      if (id_atual > id) { //achamos a posição para adicionar
         __shift_index_arr(index,i,false); //shift para direita dos elementos
         index->arr_registros[i] = novo_reg;
         index->num_registros++;
         return true; 
      }
   }

   //não achou nenhum id maior que o que vai ser add, vamos colocar no final
   index->arr_registros[num_reg] = novo_reg;
   index->num_registros++;
   return true; 
}

//remove um registro do index em memória dado seu ID, usado na remoção
bool ind_index_mem_remove_id(Index*index, const  int id){
   assert(index);

   int posicao_reg_array = __ind_busca_posi_por_id(index,id); //tenta achar o registro no array pelo seu offset
   if (posicao_reg_array == -1)
      return false;//o registro não existe, operação falhou

   __shift_index_arr(index,posicao_reg_array,true); //shifta os valores a partir daquela posição para frente pra esquerda
   index->num_registros--;
   return true;
}

//FILE I/O: Funções principais de manipular o árquivo índice em disco

//Funcionalidade 4
//dado um arquivo binário de dados e um nome de arquivo pro índice, le o arquivo de dados e escreve no arquivo de índice os dados ordenados
bool ind_cria_index(const char* arquivo_dados, const char* arquivo_index){
   assert_2ptrs(arquivo_dados,arquivo_index);

   FILE* fp_dados = fopen(arquivo_dados,R); //abre o arquivo de dados para leitura
   if (!fp_dados){
      printf(DEFAULT_ERROR_MSG);
      return false;
   }

   Index* index_mem = (Index*) malloc(sizeof(Index)); //alloca memoria pro index em memória
   assert(index_mem);
   IndexRegistro* arr_registros = (IndexRegistro*) malloc(sizeof(IndexRegistro) * REGISTRY_LIST_BLOCK); //aloca memória pro arr de registros
   assert(arr_registros);
   index_mem->arr_registros = arr_registros;//coloca o array de registros

   index_mem->header.status = INCONSISTENT_INDEX; //struct em memória ta como inconsistente
   int num_registros_index = 0;

   Header* buffer_header = rgt_header_ler_arquivo(fp_dados); //le o header do arquivo, avançando o FILE* fp_dados para os registros
   if (buffer_header->status == INCONSISTENT_HEADER){
         printf(DEFAULT_ERROR_MSG); //header do arquivo de dados inconsistente, printa erro, fecha arquivos e retorna
         fclose(fp_dados);
         rgt_header_destruir(&buffer_header);
         ind_index_mem_destroi(&index_mem);
         return false;
   }  

   long int byte_offset_anterior = 0;
   //o byteoffset quando adicionado a um tamanho, já esta no byte imediatamente após esse tamanho, então ele começo no byte 25, que é do primeiro reg e 
   //não do header
   byte_offset_anterior += HEADER_SIZE;

  
   while(1){ //loop ate houver falha na leitura de um registro ou chegar no fim do arquivo
      bool removido_flag = false;    //flag de caso o registro esteja removido, para saber se o NULL veio de um removido ou de um erro/fim do arquivo
      
      int bytes_lidos = 0;
      Registro* reg_lido = rgt_registro_ler_arquivo(fp_dados,&removido_flag,&bytes_lidos); //le um registro do arquivo

      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (removido_flag){ //o registro lido foi removido, continuar o loop
         byte_offset_anterior += bytes_lidos;
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }

      IndexRegistro reg = {.id = reg_lido->id, .byteoffset = byte_offset_anterior}; //cria um struct com os dados lidos
      
      index_mem->arr_registros[num_registros_index] = reg; //coloca essa struct no array
      num_registros_index++;//incrementa o numero de elementos do array de registros do indice
      if ((num_registros_index % REGISTRY_LIST_BLOCK) == 0){//precisa dar realloc
         IndexRegistro* temp = (IndexRegistro*) realloc(index_mem->arr_registros, (num_registros_index + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
         if (temp){//realloc com sucesso
            index_mem->arr_registros = temp;
         }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index_mem);
            rgt_registro_destruir(&reg_lido);
            rgt_header_destruir(&buffer_header);
            fclose(fp_dados);
            return false;
         }
      }
      byte_offset_anterior += bytes_lidos; //byteoffset anterior é avançado
      rgt_registro_destruir(&reg_lido); //libera memoria alocada pro registro
   }
   rgt_header_destruir(&buffer_header); //libera memoria do header do reg de dados
   fclose(fp_dados);//fecha arquivo de dados
   index_mem->num_registros = num_registros_index; //coloca o numero de registros do struct
   

   qsort( //quicksort da stdlib do c para ordenar os valores do índice em memória
      index_mem->arr_registros,
      index_mem->num_registros,
      sizeof(IndexRegistro),
      __func_compara_id
   );
   
   //abrir e escrever no arquivo de índice
   FILE* fp_index = fopen(arquivo_index,W); //abre arquivo de índice para escrita
   if (!fp_index){
      printf(DEFAULT_ERROR_MSG);
      return false;
   }

   char index_status = INCONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de inconsistente
   
   for (int i = 0; i < index_mem->num_registros; i++){ //loop por todos os registros do indice em RAM
      IndexRegistro reg_index = index_mem->arr_registros[i];
      
      file_io_check(fwrite(&reg_index.id,sizeof(int),1,fp_index),1); //escreve o id do registro no arquivo de index
      file_io_check(fwrite(&reg_index.byteoffset,sizeof(long int),1,fp_index),1); //escreve o byteoffset do arquivo de índice
   }
   

   fseek(fp_index,0,SEEK_SET);//vai pro começo do arquivo de índice
   index_status = CONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de consistente
   index_mem->header.status =  CONSISTENT_INDEX; //index na memória ta consistente
   fclose(fp_index); //fecha arquivo do index
   ind_index_mem_destroi(&index_mem); //essa função não retorna o index em memória apenas cria ele

   return true;
}

//Dado um FP de um arquivo de dados no offset 0, cria um index com os registros não removidos presente nele
//da um fseek pro começo depois
//OBS1: Não faz verificação da consistência do arquivo, pois não ocorre a abertura do arquivo e sim apenas é passado um FP que foi manipulado
//e verificado antes
bool ind_cria_index_fp(FILE* fp_dados, const char* arquivo_index){
   assert_2ptrs(fp_dados,arquivo_index);
 
   Index* index_mem = (Index*) malloc(sizeof(Index)); //alloca memoria pro index em memória
   assert(index_mem);
   IndexRegistro* arr_registros = (IndexRegistro*) malloc(sizeof(IndexRegistro) * REGISTRY_LIST_BLOCK); //aloca memória pro arr de registros
   assert(arr_registros);
   index_mem->arr_registros = arr_registros;//coloca o array de registros

   index_mem->header.status = INCONSISTENT_INDEX; //struct em memória ta como inconsistente
   int num_registros_index = 0;


   long int byte_offset_anterior = 0;
   
   Header* buffer_header = rgt_header_ler_arquivo(fp_dados);
   rgt_header_destruir(&buffer_header); //so pra avançar o FP, não ha verificação  nessa função pois o arquivo ja foi aberto antes
   byte_offset_anterior += HEADER_SIZE;

  
   while(1){ //loop ate houver falha na leitura de um registro ou chegar no fim do arquivo
      bool removido_flag = false;    //flag de caso o registro esteja removido, para saber se o NULL veio de um removido ou de um erro/fim do arquivo
      
      int bytes_lidos = 0;
      Registro* reg_lido = rgt_registro_ler_arquivo(fp_dados,&removido_flag,&bytes_lidos); //le um registro do arquivo

      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (removido_flag){ //o registro lido foi removido, continuar o loop
         byte_offset_anterior += bytes_lidos;
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }

      IndexRegistro reg = {.id = reg_lido->id, .byteoffset = byte_offset_anterior}; //cria um struct com os dados lidos
      
      index_mem->arr_registros[num_registros_index] = reg; //coloca essa struct no array
      num_registros_index++;//incrementa o numero de elementos do array de registros do indice
      if ((num_registros_index % REGISTRY_LIST_BLOCK) == 0){//precisa dar realloc
         IndexRegistro* temp = (IndexRegistro*) realloc(index_mem->arr_registros, (num_registros_index + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
         if (temp){//realloc com sucesso
            index_mem->arr_registros = temp;
         }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index_mem);
            rgt_registro_destruir(&reg_lido);
            fclose(fp_dados);
            return false;
         }
      }
      byte_offset_anterior += bytes_lidos; //byteoffset anterior é avançado
      rgt_registro_destruir(&reg_lido); //libera memoria alocada pro registro
   }
   fseek(fp_dados,0,SEEK_SET); //fseek no fpdados
   index_mem->num_registros = num_registros_index; //coloca o numero de registros do struct
   

   qsort( //quicksort da stdlib do c para ordenar os valores do índice em memória
      index_mem->arr_registros,
      index_mem->num_registros,
      sizeof(IndexRegistro),
      __func_compara_id
   );
   
   //abrir e escrever no arquivo de índice
   FILE* fp_index = fopen(arquivo_index,W); //abre arquivo de índice para escrita
   if (!fp_index){
      printf(DEFAULT_ERROR_MSG);
      return false;
   }

   char index_status = INCONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de inconsistente
   
   for (int i = 0; i < index_mem->num_registros; i++){ //loop por todos os registros do indice em RAM
      IndexRegistro reg_index = index_mem->arr_registros[i];
      
      file_io_check(fwrite(&reg_index.id,sizeof(int),1,fp_index),1); //escreve o id do registro no arquivo de index
      file_io_check(fwrite(&reg_index.byteoffset,sizeof(long int),1,fp_index),1); //escreve o byteoffset do arquivo de índice
   }
   

   fseek(fp_index,0,SEEK_SET);//vai pro começo do arquivo de índice
   index_status = CONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de consistente
   index_mem->header.status =  CONSISTENT_INDEX; //index na memória ta consistente
   fclose(fp_index); //fecha arquivo do index
   ind_index_mem_destroi(&index_mem); //essa função não retorna o index em memória apenas cria ele

   return true;
}

//dado um arquivo binário de dados e um nome de arquivo pro índice, le o arquivo de dados e escreve no arquivo de índice ordenado e retorna a struct em
//RAM ordenada que representa esse índice
Index* ind_cria_carrega_index(const char* arquivo_dados, const char* arquivo_index){
   assert_2ptrs(arquivo_dados,arquivo_index);

   FILE* fp_dados = fopen(arquivo_dados,R); //abre o arquivo de dados para leitura
   if (!fp_dados){
      printf(DEFAULT_ERROR_MSG);
      return NULL;
   }

   Index* index_mem = (Index*) malloc(sizeof(Index)); //alloca memoria pro index em memória
   assert(index_mem);
   IndexRegistro* arr_registros = (IndexRegistro*) malloc(sizeof(IndexRegistro) * REGISTRY_LIST_BLOCK); //aloca memória pro arr de registros
   assert(arr_registros);
   index_mem->arr_registros = arr_registros;//coloca o array de registros


   
   index_mem->header.status = INCONSISTENT_INDEX; //struct em memória ta como inconsistente
   int num_registros_index = 0;

   Header* buffer_header = rgt_header_ler_arquivo(fp_dados); //le o header do arquivo, avançando o FILE* fp_dados para os registros
   if (buffer_header->status == INCONSISTENT_HEADER){
         printf(DEFAULT_ERROR_MSG); //header do arquivo de dados inconsistente, printa erro, fecha arquivos e retorna
         fclose(fp_dados);
         ind_index_mem_destroi(&index_mem);
         return NULL;
   }  

   long int byte_offset_anterior = 0;
   //o byteoffset quando adicionado a um tamanho, já esta no byte imediatamente após esse tamanho, então ele começo no byte 25, que é do primeiro reg e 
   //não do header
   byte_offset_anterior += HEADER_SIZE;

  
   while(1){ //loop ate houver falha na leitura de um registro ou chegar no fim do arquivo
      bool removido_flag = false;    //flag de caso o registro esteja removido, para saber se o NULL veio de um removido ou de um erro/fim do arquivo
      
      int bytes_lidos = 0;
      Registro* reg_lido = rgt_registro_ler_arquivo(fp_dados,&removido_flag,&bytes_lidos); //le um registro do arquivo

      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (removido_flag){ //o registro lido foi removido, continuar o loop
         byte_offset_anterior += bytes_lidos;
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }

      IndexRegistro reg = {.id = reg_lido->id, .byteoffset = byte_offset_anterior}; //cria um struct com os dados lidos
      
      index_mem->arr_registros[num_registros_index] = reg; //coloca essa struct no array
      num_registros_index++;//incrementa o numero de elementos do array de registros do indice
      if ((num_registros_index % REGISTRY_LIST_BLOCK) == 0){//precisa dar realloc
         IndexRegistro* temp = (IndexRegistro*) realloc(index_mem->arr_registros, (num_registros_index + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
         if (temp){//realloc com sucesso
            index_mem->arr_registros = temp;
         }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index_mem);
            rgt_registro_destruir(&reg_lido);
            rgt_header_destruir(&buffer_header);
            fclose(fp_dados);
            return NULL;
         }
      }
      byte_offset_anterior += bytes_lidos; //byteoffset anterior é avançado
      rgt_registro_destruir(&reg_lido); //libera memoria alocada pro registro
   }
   rgt_header_destruir(&buffer_header); //libera memoria do header do reg de dados
   fclose(fp_dados);//fecha arquivo de dados
   index_mem->num_registros = num_registros_index; //coloca o numero de registros do struct
   

   qsort( //quicksort da stdlib do c para ordenar os valores do índice em memória
      index_mem->arr_registros,
      index_mem->num_registros,
      sizeof(IndexRegistro),
      __func_compara_id
   );
   
   //abrir e escrever no arquivo de índice
   FILE* fp_index = fopen(arquivo_index,W); //abre arquivo de índice para escrita
   if (!fp_index){
      printf(DEFAULT_ERROR_MSG);
      return NULL;
   }

   char index_status = INCONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de inconsistente
   
   for (int i = 0; i < index_mem->num_registros; i++){ //loop por todos os registros do indice em RAM
      IndexRegistro reg_index = index_mem->arr_registros[i];
      
      file_io_check(fwrite(&reg_index.id,sizeof(int),1,fp_index),1); //escreve o id do registro no arquivo de index
      file_io_check(fwrite(&reg_index.byteoffset,sizeof(long int),1,fp_index),1); //escreve o byteoffset do arquivo de índice
   }
   

   fseek(fp_index,0,SEEK_SET);//vai pro começo do arquivo de índice
   index_status = CONSISTENT_INDEX;
   file_io_check(fwrite(&index_status,sizeof(char),1,fp_index),1); //escreve o header do index com status de consistente
   index_mem->header.status =  CONSISTENT_INDEX; //index na memória ta consistente
   fclose(fp_index); //fecha arquivo do index
  
   return index_mem;
}

//le um arquivo binário de índice e carrega ele na memória primária como uma struct
Index* ind_carrega_index(const char* arquivo_index){
   assert(arquivo_index);

   FILE* fp = fopen(arquivo_index,R); //abre index pra leitura
   if (!fp){
      printf(DEFAULT_ERROR_MSG);
      return NULL;
   }

   Index* index_mem = (Index*) malloc(sizeof(Index)); //alloca memoria pro index em memória
   assert(index_mem);

   IndexRegistro* arr_registros = (IndexRegistro*) malloc(sizeof(IndexRegistro) * REGISTRY_LIST_BLOCK); //aloca memória pro arr de registros
   assert(arr_registros);
   index_mem->arr_registros = arr_registros;//coloca o array de registros


   char status;
   file_io_check(fread(&status,sizeof(char),1,fp),1); //le o status do arquivo de index
   if (status == INCONSISTENT_INDEX){
      printf(DEFAULT_ERROR_MSG); //header do index inconsistente, cancela operação
      ind_index_mem_destroi(&index_mem);
      fclose(fp);
      return NULL;
   }

   index_mem->header.status = status; //Bota o status do struct como o status lido
   int num_registros_index = 0;

   while (1) { //loop até um fread falhar
      int id;
      long int offset;

      int read_result = fread(&id,sizeof(int),1,fp); //le o id
      if (read_result != 1) //se um dos freads falhar, da break no loop
         break;

      read_result = fread(&offset,sizeof(long int),1,fp); //le o offset
      if (read_result != 1)
         break;

      IndexRegistro reg = {.id = id, .byteoffset = offset}; //cria um struct com os dados lidos
      index_mem->arr_registros[num_registros_index] = reg; //coloca essa struct no array

      num_registros_index++;//incrementa o numero de elementos do array de registros do indice
      if ((num_registros_index % REGISTRY_LIST_BLOCK) == 0){//precisa dar realloc
         IndexRegistro* temp = (IndexRegistro*) realloc(index_mem->arr_registros, (num_registros_index + REGISTRY_LIST_BLOCK) * sizeof(IndexRegistro));
         if (temp){//realloc com sucesso
            index_mem->arr_registros = temp;
         }else { //falha no realloc
            warn_printf("falha realloc\n");
            ind_index_mem_destroi(&index_mem);
            break;
         }
      }
   }

   index_mem->num_registros = num_registros_index; //coloca o numero de registros do struct
   fclose(fp); //fecha o arquivo depois de ler tudo
   return index_mem;
}

//dado uma struct index em memória e um caminho para o arquivo de índice, escreve essa struct nesse arquivo e fecha o arquivo
bool ind_atualiza_index(const char* arquivo_index, const Index* index_mem){
   assert_2ptrs(arquivo_index,index_mem);

   FILE* fp = fopen(arquivo_index,W); //abre arquivo de índice para escrita
   null_errno_check(fp);

   char status = INCONSISTENT_INDEX; //escreve status do header como inconsistente
   file_io_check(fwrite(&status,sizeof(char),1,fp),1);

   bool op_sucesso = true;

   for (int i = 0; i < index_mem->num_registros; i++){//loop pelo array de registros do indice
      int id = index_mem->arr_registros[i].id; 
      long int offset = index_mem->arr_registros[i].byteoffset;

      int write_result = fwrite(&id,sizeof(int),1,fp); //escreve o id e depois o offset
      if (write_result != 1){
         op_sucesso = false;
         break;
      }

      write_result = fwrite(&offset,sizeof(long int),1,fp);
      if (write_result != 1){
         op_sucesso = false;
         break;
      }
   }

   if (op_sucesso){//se a operação foi um sucesso, vamos escrever o header como consistente
      fseek(fp,0,SEEK_SET);
      status = CONSISTENT_INDEX; //escreve status do header como consistente
      file_io_check(fwrite(&status,sizeof(char),1,fp),1);
   }

   fclose(fp); //fecha o arquivo
   return op_sucesso;
}

