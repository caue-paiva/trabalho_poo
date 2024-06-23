#ifndef HEADER_H
   #define HEADER_H
   #include "tad_reg_variavel.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/
   //Esse .h expoẽ as funcionalidades e structs para a struct que vai servir de header/cabeçalho do arquivo

   #define HEADER_SIZE (sizeof(char) + (2*sizeof(long int)) + (2*sizeof(int))) //define para o numero de bytes dos campos do header

   typedef struct f_header Header; //typedefs na struct do Header

   struct f_header { //struct para lidar com o header do arquivo, incluindo o estado do arquivo durante modificação (add de registros .etc)
      char status; //campos do header
      long int topo;
      long int proxByteOffset;
      int nroRegArq;  
      int nroRegRem;
   };

   #define INCONSISTENT_HEADER '0' //defines para status contido no HEADER
   #define CONSISTENT_HEADER '1'

   //funcoes de lidar com o header do arquivo, utilizado para escrita no arquivo e para manter seu estado (num de registros...)
   
   Header* rgt_header_criar(); 
   Header* rgt_header_ler_arquivo(FILE* fp);
   void rgt_header_destruir(Header** header);
   bool rgt_header_escreve_registros(Header* header, const Registro* regis, FILE* fp);
   bool rgt_header_escreve_arquivo(const Header* header,FILE*fp);
   FILE* rgt_header_abre_escrita(Header* header, const char* nome_arquivo);
   bool rgt_header_fecha_escrita(Header* header, FILE** fp);
   FILE* rgt_header_abre_modifica(const char* nome_arquivo, Header* header);
   bool rgt_header_fecha_modifica(FILE** fp, Header* header);


   //funcionalidade 2, colocada aqui pois depende da struct header
   void rgt_printa_arquivo(const char*nome_arquivo);


#endif