#include "tad_header.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/

//HEADER

//cria um struct header para guardar o estado do header durante runtime e ser escrita no arquivo
Header* rgt_header_criar() {
   Header* header = (Header*) malloc(sizeof(Header)); //aloca memória pro header
   assert(header);

   header->status = INCONSISTENT_HEADER; //header só fica consistente mesmo dps de uma escrita com sucesso
   header->topo = -1;  //topo nao existe de começo
   header->proxByteOffset = 0; //o prox byte offset comeca como zero
   header->nroRegArq = 0; //valores iniciais
   header->nroRegRem = 0;

   return header;
}



//imprime os conteudos de um header
void __rgt_header_print(const Header* header){
   assert(header);

   printf("Header: Status %c | Topo: %ld | Prox byte offset: %ld |\nnum registros: %d | num regis remov: %d \n",
         header->status, header->topo, header->proxByteOffset, header->nroRegArq, header->nroRegRem
   );
}

//libera a memoria de um header e seta seu ponteiro pra NULL
void rgt_header_destruir(Header** header){
   assert_2ptrs(header, (*header));
   free(*header);
   *header = NULL;
}

//Dado um arquivo FILE* fp, le os campos dos headers um a um e cria um novo struct header 
Header* rgt_header_ler_arquivo(FILE* fp){
   assert(fp);

   char status; //variaveis do header
   long int topo;
   long int proxByteOffset;
   int nroRegArq;
   int nroRegRem;

   fread(&status, sizeof(char), 1, fp); //le conteudos do arquivo e guarda nas variáveis
   fread(&topo, sizeof(long int), 1, fp);
   fread(&proxByteOffset, sizeof(long int), 1, fp);
   fread(&nroRegArq, sizeof(int), 1, fp);
   fread(&nroRegRem, sizeof(int), 1, fp);

   Header* header_novo = (Header*) malloc(sizeof(Header)); //malloc num novo header
   assert(header_novo);

   header_novo->status = status; //assinala valores ao novo header
   header_novo->topo = topo;
   header_novo->proxByteOffset = proxByteOffset;
   header_novo->nroRegArq = nroRegArq;
   header_novo->nroRegRem = nroRegRem;

   return header_novo;
}


//Escreve uma struct header (campo a campo) num arquivo
//MOVE O FILE* fp
bool rgt_header_escreve_arquivo(const Header* header,FILE*fp){
   assert_2ptrs(fp,header);

   printf("header a ser escrito, status %c , num de reg %d\n",header->status,header->nroRegArq);

   int numero_writes = 0; //numero de writes com sucesso
   numero_writes += fwrite(&header->status, sizeof(char), 1, fp); //cada fwrite deveria incrementar a variavel em 1
   numero_writes += fwrite(&header->topo, sizeof(long int), 1, fp);
   numero_writes += fwrite(&header->proxByteOffset, sizeof(long int), 1, fp);
   numero_writes += fwrite(&header->nroRegArq, sizeof(int), 1, fp);
   numero_writes += fwrite(&header->nroRegRem, sizeof(int), 1, fp);
   if (numero_writes != 5) //se o numero total de write for diferente de 5, tivemos erros na escrita
      return false;
   
   return true;
}

//Dado um registro e um header, escreve os conteudos do registro no arquivo e atualiza a struct header de acordo (incrementa numero de registros)
//MOVE o FILE* fp
bool rgt_header_escreve_registros(Header* header, const Registro* regis, FILE* fp){
   assert_3ptrs(header,regis,fp);

   bool resultado = rgt_registro_escreve_arq(regis,fp); //escreve registro no arquivo
   header->status = INCONSISTENT_HEADER; //header é setada novamente para inconsistente por segurança, apenas ao fechar a escrita ele será tido como consistente

   if (resultado == true) //escrita com sucesso
      header->nroRegArq++; //numero de registros no header é incrementado
   return resultado;
}


//Abre um arquivo pela primeira vez para escrita e escreve um header nele, esse header é escrito como inconsistente já que acabamos de começar a escrita
//Retorna o FILE* fp desse arquivo aberto
FILE* rgt_header_abre_escrita(Header* header, const char* nome_arquivo){
   assert_2ptrs(header,nome_arquivo);

   FILE* fp = fopen(nome_arquivo,W); //abre arquivo pra escrita
   null_errno_check(fp);
   if (!fp){
      printf(DEFAULT_ERROR_MSG);
      return NULL; //como fp == NULL, nao vamos dar fclose
   }

   header->status = INCONSISTENT_HEADER; //header é colocado novamente, por segurança, como inconsistente
   int resultado_write = rgt_header_escreve_arquivo(header,fp); //tenta escrever o header no arquivo

   if (resultado_write == 0){ //caso a escrita do header falhe
      printf(DEFAULT_ERROR_MSG);
      fclose(fp); //fecha fp depois da escrita falha
      return NULL;
   }
   return fp; //retorna FILE* fp com sucesso
}

//Termina o processo de escrita nume FILE*fp, escrevendo um header atualizado nele (e com status consistente)
// Dá FCLOSE e seta o fp como NULL
bool rgt_header_fecha_escrita(Header* header, FILE** fp){
   assert_2ptrs(header,fp);

   int result_seek = fseek(*fp,0,SEEK_SET); //fseek pro começo
   if (result_seek == -1){
       errno_check(result_seek); //printa o erro que deu no fseek
       fclose(*fp); //fecha o arquivo dps da operação falha
       *fp = NULL;
       return false; 
   }

   header->status = CONSISTENT_HEADER; //vamos tentar escrever o header como consistente
   int resultado_write = rgt_header_escreve_arquivo(header,*fp); //escreve header
   
   if (resultado_write == 0){ //escrita do header falhou
      printf(DEFAULT_ERROR_MSG);
      header->status = INCONSISTENT_HEADER; //header  volta a ser inconsistente
      fclose(*fp); //fecha arquivo
      *fp = NULL;
      return false;
   }
   fclose(*fp); //fecha arquivo e seta fp como NULL
   *fp = NULL;
   return true;
}


//Funções para lidar com arquivos que já existem e precisam ter registros removidos ou inseridos

//Abre um arquivo já existente para modificação, escreve o arquivo como inconsistente ,escreve os conteudos do header desse arquivo num struct header e não avança o FP
FILE* rgt_header_abre_modifica(const char* nome_arquivo, Header* header){
   assert_2ptrs(nome_arquivo,header);

   FILE* fp = fopen(nome_arquivo,"r+b"); //abre arquivo pra escrita
   null_errno_check(fp);
   if (!fp){
      printf(DEFAULT_ERROR_MSG);
      return NULL; //como fp == NULL, nao vamos dar fclose
   }

   char status = INCONSISTENT_HEADER; //header é colocado  como inconsistente
   int resultado_write = fwrite(&status,sizeof(char),1,fp);
   fflush(fp);
   header->status = INCONSISTENT_HEADER;
   if (resultado_write == 0){ //caso a escrita do header falhe
      printf(DEFAULT_ERROR_MSG);
      fclose(fp); //fecha fp depois da escrita falha
      return NULL;
   }

   long int topo;
   long int proxByteOffset;
   int nroRegArq;
   int nroRegRem;
  

   fread(&topo, sizeof(long int), 1, fp); //ler campos do header
   fread(&proxByteOffset, sizeof(long int), 1, fp);
   fread(&nroRegArq, sizeof(int), 1, fp);
   fread(&nroRegRem, sizeof(int), 1, fp);

   header->topo = topo; //coloca valores na struct header
   header->proxByteOffset = proxByteOffset;
   header->nroRegArq = nroRegArq;
   header->nroRegRem = nroRegRem;


   int seek_result = fseek(fp,0,SEEK_SET); //retorna 25 bytes, nesse caso não vai resultar em mais um acesso a página de disco pois do byteoffset 0-24 está tudo na mesma página
   errno_check(seek_result);               //esse fseek de volta foi feito pois a maioria das funcões precisa de um fp no byte 0 (padronização)  

   if (DEBUG){
      __rgt_header_print(header);
   }

   return fp; //retorna FILE* fp com sucesso
}

//dado um FP e o header desse arquivo, escreve o arquivo como consistente e fecha ele, também seta o FP como NULL
//precisa de um FP no começo do arquivo
bool rgt_header_fecha_modifica(FILE** fp, Header* header){
   assert_2ptrs(fp,*fp);
   
   header->status = CONSISTENT_HEADER;
   bool resultado = rgt_header_escreve_arquivo(header,*fp); //escreve o header atualizado e como consistente
   if (resultado != 1){
      warn_printf("Falha ao escrever o header atualizado \n");
      fclose(*fp);
      *fp = NULL;
      return false;
   }

   fclose(*fp); //fecha arquivo
   *fp = NULL;
   return true;
}


//Dado apenas um nome de arquivo, imprime seus conteudos na tela ou retorna uma mensagem de erro 
void rgt_printa_arquivo(const char*nome_arquivo){

   FILE* fp = fopen(nome_arquivo,R); //abre o arquivo pra leitura
   if (fp == NULL){  //checa se o ponteiro do arquivo é NULL
      printf(DEFAULT_ERROR_MSG); //se for null printa msg de erro padrao
      return;
   }

   Header* buffer_header = rgt_header_ler_arquivo(fp); //le o header do arquivo, avançando o FILE* fp para os registros
   bool existe_registros = false; //flag pra ditar se  existe pelo menos um registro num arquivo
   if (buffer_header->status == INCONSISTENT_HEADER){
         printf(DEFAULT_ERROR_MSG);
         fclose(fp);
         return;
   }

   long int byte_offset_atual = HEADER_SIZE;



   while(1){ //Loop ate falha na leitura de um registro ou chegar no fim do arquivo
      bool registro_removido = false; //flag para dizer se o retorno NULL foi do fim do arquivo ou de um registro removido

      int bytes_lidos;
      Registro* reg_lido = rgt_registro_ler_arquivo(fp,&registro_removido,&bytes_lidos); //ler registro do arquivo

      if (!reg_lido) //função de ler retornou NULL do fim do arquivo ou erro, break no loop
          break;

      if (registro_removido){ //o registro lido foi removido, continuar o loop
         d_printf("registro removido no offset %ld, com tamanho %d \n",byte_offset_atual,reg_lido->tamanhoRegistro);
         byte_offset_atual+= bytes_lidos;
         rgt_registro_destruir(&reg_lido); //liberar memória do registro
         continue;
      }
      d_printf("registro normal no offset %ld, com tamanho %d \n",byte_offset_atual,reg_lido->tamanhoRegistro);
      // printf("id: %d  no offset: %ld  ", reg_lido->id,byte_offset_atual);
      byte_offset_atual+= bytes_lidos;
      rgt_registro_print(reg_lido); //funcao de ler retornou valor normal, printa o registro
      rgt_registro_destruir(&reg_lido); //deleta o registro alocado
      existe_registros = true; //temos pelo menos um registro válido
   }

   if(!existe_registros) //caso nenhum registro valido foi encontrado no arquivo
      printf("Registro inexistente.\n\n");

   rgt_header_destruir(&buffer_header); //deleta o header alocado
   fclose(fp); //fecha o arquivo

}