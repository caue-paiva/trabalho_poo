#ifndef QUERY_FILE_H
   #define QUERY_FILE_H
   #include "tad_lista_encad.h"
   #include "funcoes_miscelaneas.h"


   /*
   Desenvolvido por: 

   Cauê Paiva Lira - NUSP: 14675416
   João Pedro Alves Notari Godoy - NUSP: 14582076

   */

   /*
      Arquivo para a lógica de busca em um arquivo de registros.
   */
   #define MAX_QUERY_LINE_SIZE 256 //define para o tamanho max da linha de busca

   #define OFFSET_LIST_BLOCK 25 //Define para a lista de offsets, que vai ser alocada em blocos

   bool query_realizar_pesquisa(const char* nome_arquivo, const int numero_queries);    //funcionalidade 3, realiza uma busca no arquivo
   
   Registro* query_processa_linha_comando(const int num_busca);    //processa linha de comando e gera registros para remoção 
   Registro* query_processa_linha_comando_insercao(); //mesma coisa da acima mas paa inserção

   //funções para buscar no arquivo a lista encadeada de registros removidos:
   
   void query_busca_arquivo_e_removidos(const Registro* reg_busca , FILE* fp, ListaEncad* lista_matches, ListaEncad* lista_removidos); //enche 2 listas encadeadas, uma dos ja removidos e outros que batem com um registro buscado
   ListaEncad* query_lista_removidos_arquivo(FILE* fp); //retorna apenas a lista de removidos do arquivo

#endif