#include "funct_insercao.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/


//INSERÇÃO: lógica de inserção com reaproveitamento de espaço

//atualiza a lista encadeada de removidos no arquivo depois da inserção, isso é feito com o byte offset dos nós anteriores e posteriores ao nó removido
bool __ins_atualiza_li_encad_arquivo(FILE*fp, const long int offset_anterior, const long int offset_prox){
   assert(fp);

   if (offset_anterior == -1){ //nesse caso o registro tirado da lista de remoção estava na cabeça da lista
      fseek(fp,0,SEEK_SET); //vai pro começo do arquivo
      char status;
      b_file_io_check(fread(&status,sizeof(char),1,fp),1); //le o status do header
      b_file_io_check(fwrite(&offset_prox,sizeof(long int),1,fp),1); //escreve o topo do header como o offset do prox reg depois do registro que foi tirado
   }else { //retirado não estava na cabeça
      fseek(fp,offset_anterior,SEEK_SET); //vai pro offset do anterior
      char removido_buf;
      b_file_io_check(fread(&removido_buf,sizeof(char),1,fp),1); //le o campo removido do registro
      int tam_buf;
      b_file_io_check(fread(&tam_buf,sizeof(int),1,fp),1); //le o campo de tamanho do registro
      b_file_io_check(fwrite(&offset_prox,sizeof(long int),1,fp),1); //escreve o prox do registro antes do removido como o prox do removido
   }

   return true;
}

//Realiza a inserção de um registro num arquivo, usando reaproveitamento de memória se possível
bool __ins_logica_insercao(FILE*fp, Header*header , Registro* registro, ListaEncad*  lista_removidos, long int *offset_colocado){
   assert_3ptrs(registro,fp,header);
   assert(offset_colocado);
   
   long int offset_anterior; //vamos pegar o offset dos registros anterior e posterior do removido na lista encadeada
   long int  offset_prox;
   No* best_fit = li_encad_best_fit_pop(lista_removidos,registro->tamanhoRegistro,&offset_anterior,&offset_prox); //acha o registro que tem o best fit para reaproveitar espaço
   bool resultado;

   if (!best_fit){ //best fit é null, vamos escrever no fim do arquivo
      fseek(fp, header->proxByteOffset, SEEK_SET); //vai pro final do arquivo
      *offset_colocado = header->proxByteOffset; //seta o parametro de offset que o reg foi colocado
      resultado = rgt_header_escreve_registros(header,registro,fp); //escreve o registro
      header->proxByteOffset += registro->tamanhoRegistro; //atualiza o campo proxoffset do header
   } else { //vamos reaproveitar memória
      long int offset = best_fit->offset; //offset do best fit
      *offset_colocado = offset; //seta o parametro de offset que o reg foi colocado
      fseek(fp,offset,SEEK_SET); //vai pro offset certo

      int bytes_usados = registro->tamanhoRegistro; //qntd de bytes realmente usados no novo registro
      registro->tamanhoRegistro = best_fit->tam_registro; //o tamanho do registro em si que vamos escrever é o tamanho antigo do registro removido
      //que é maior ou igual ao inserido

      resultado = rgt_header_escreve_registros(header,registro,fp);//depois de escreve toda as informações do novo registro, o fp vai estar sobre a parte que deve 
      
      //Escrever como lixo de memória se sobrar espaço
      int espaco_sobrando = best_fit->tam_registro - bytes_usados;
      if (espaco_sobrando > 0){ //se tiver espaço sobrando, escreve lixo no lugar
         char lixo = GARBAGE_DATA_TOKEN;
         for (int i = 0; i < espaco_sobrando; i++){ //loop para escrever o lixo
            int write_result = fwrite(&lixo,sizeof(char),1,fp);
            fflush(fp);
            if (write_result != 1){
               resultado = false; //falha na escrita do lixo
               warn_printf("falha no fwrite");
            }
         }
      }
      resultado = resultado && __ins_atualiza_li_encad_arquivo(fp,offset_anterior,offset_prox); //tenta atualizar a lista encadeada de removidos depois de reaproveitar espaço
      
      if (resultado) //caso o reaproveitamento de espaço tenha tido sucesso
         header->nroRegRem--; //decrementar num de registros removidos
   }

   fseek(fp,0,SEEK_SET); //volta pro começo do arquivo
   if (resultado){ //escrita teve sucesso
      if (lista_removidos->cabeca)
         header->topo = lista_removidos->cabeca->offset; //altera o topo da struct header em memória
      else 
         header->topo = -1;
   }
   if (best_fit) //da free no nó best fit caso ele n seja null
      free(best_fit);

   return resultado;
}

//Dado um nome de arquivo de dados, de índice e o número de inserções, le a linha de comando e realiza a inserção de um registro, reaproveitando espaço de registros removidos
//tambem aproveita o FP do arquivo de dados e cria um índice a partir do arq. de dados atualizado
bool ins_insere_registros(const char* nome_arquivo_dados,const char* nome_arquivo_index , const int numero_insercoes){
   assert_2ptrs(nome_arquivo_dados,nome_arquivo_index);

   Header* header = rgt_header_criar();
   FILE* fp = rgt_header_abre_modifica(nome_arquivo_dados,header); //abre o arquivo de dados e carrega o header dele
   if (!fp){
      return false;
   }

   ListaEncad* lista_removidos = query_lista_removidos_arquivo(fp); //carrega a lista encadeada de registros removidos em memória
   bool sucesso_insercao = true;
  
   for (int i = 0; i < numero_insercoes; i++){ //loop por todas as inserções
      Registro* reg = query_processa_linha_comando_insercao(); //le a linha de comando e cria um registro a partir disso
      printf("reg lido id: %d",reg->id);
      long int offset_colocado;
      bool resultado = __ins_logica_insercao(fp,header,reg,lista_removidos,&offset_colocado); //tenta inserir o registro
      sucesso_insercao = sucesso_insercao && resultado; //and com o resultado da inserção anterior e o atual
      rgt_registro_destruir(&reg); //destroi o registro usado na inserção
   }

   li_encad_destruir(&lista_removidos); //destroi a lista encad de removidos em memória

   rgt_header_fecha_modifica(&fp,header); //escreve o header atualizado e fecha o arquivo
   rgt_header_destruir(&header);

   return true;
}
