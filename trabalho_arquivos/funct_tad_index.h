#ifndef INDEX_H
   #define INDEX_H
   #include "tad_header.h"

   /*
   Desenvolvido por: 

   Cauê Paiva Lira - NUSP: 14675416
   João Pedro Alves Notari Godoy - NUSP: 14582076

   */


   #define CONSISTENT_INDEX '1' //defines para index consistente ou não
   #define INCONSISTENT_INDEX '0'


   typedef struct {
      char status;
   }IndexHeader;

   typedef struct { //struct para cada registro do índice
      int id;
      long int byteoffset;
   }IndexRegistro;

   typedef struct { //struct para o índice como um todo em memória primária
      IndexHeader header;
      IndexRegistro* arr_registros;
      int num_registros;
   }Index;


   /*
   Esse arquivo fornece as funcionalidades para a criação de manipulação de um arquivo de índice, tanto na memória principal, com um tipo de dado struct
   e operações sobre ela, quanto em disco, com a leitura de arquivos parA gerar a struct e escrita da struct nos arquivos.

   Esse arquivo pode ser utilizado para acelerar as buscas nos registros, a exemplo do seu uso na remoção.

   Esse arquivo é classificado com funct_tad por expor tanto funções da funcionalidade 4 quando criar a struct Index e prover operações sobre ela
   */


   //funções de lidar com o índice em disco
   bool ind_atualiza_index(const char* arquivo_index, const Index* index_mem); //atualiza um index em disco a partir do index em memória prim
   Index* ind_cria_carrega_index(const char* arquivo_dados, const char* arquivo_index); //cria um index a partir de um arquivo binário, escreve ele em disco e retorna uma struct dele em memória
   //funcionalidade 4 baixo
   bool ind_cria_index(const char* arquivo_dados, const char* arquivo_index); //le um arquivo de dados e escreve um arquivo de indice de forma ordenado a partir dos dados
   bool ind_cria_index_fp(FILE* fp_dados, const char* arquivo_index); //cria um index a partir de uma FP aberto


   //funções de lidar com o índice em memória
   
   void ind_index_mem_destroi(Index** index); //destroi a struct do index
   void ind_print_index_mem(const Index* index_mem); //print na struct do index
   long int ind_busca_offset_por_id(const Index* index, const int id); //dado um id de um registro, acha seu offset
   bool ind_index_mem_add(Index* index, const int id, const long int offset); //add um registro ao índice em memória
   bool ind_index_mem_remove_id(Index*index, const int id); //remove um registro do índice em memória


#endif