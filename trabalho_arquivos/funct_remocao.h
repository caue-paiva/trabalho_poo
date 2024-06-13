#ifndef REMOCAO_H
   #define REMOCAO_H

   #include "funct_query_file.h"
   #include "funct_tad_index.h"

   /*
   Desenvolvido por: 

   Cauê Paiva Lira - NUSP: 14675416
   João Pedro Alves Notari Godoy - NUSP: 14582076

   */



   /*
   Esse arquivo implementa a funcionalidade 5

   Remoção: Utiliza o index se for possível para acelerar a busca pelos registros a serem removidos
   */

   bool rm_remove_registros(const char* nome_arquivo_dados, const char*  nome_arquivo_index, int numero_buscas); //inicia funcionalidade de remoção de registros
   
#endif