#ifndef INSER_H
    #define INSER_H

  #include "funct_query_file.h"
  #include "funct_tad_index.h"


  /*
  Desenvolvido por: 

  Cauê Paiva Lira - NUSP: 14675416
  João Pedro Alves Notari Godoy - NUSP: 14582076

  */


  #define GARBAGE_DATA_TOKEN '$' //char para ser escrito no arquivo pra representar lixo

  /* 
  Esse arquivo implementa a funcionalidade 6
  Inserção: Tenta utilizar a lógica de reaproveitamento de espaço e no seu final, cria um index com o registro de dados atualizado depois da inserção
  */

  
  bool ins_insere_registros(const char* nome_arquivo_dados,const char* nome_arquivo_index , const int numero_insercoes);
#endif