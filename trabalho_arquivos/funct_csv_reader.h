#ifndef CSV_READER_H
  #define CSV_READER_H
  #include "tad_header.h"
  #include "funcoes_miscelaneas.h"

  /*
  Desenvolvido por: 

  Cauê Paiva Lira - NUSP: 14675416
  João Pedro Alves Notari Godoy - NUSP: 14582076

  */

  /*
  Arquivo para a leitura dos contéudos de um arquivo CSV,guardando suas linhas em registros, para escrita em um binário.
  Expoe uma função principal, com outras funções internas para ajudar nesse objetivo.
  
  */

  //Salva os conteudos de um arquivo CSV em um arquivo binário. Realiza operções de abrir e fechar o arquivo para escrita e modifica/atualiza o seu
  //struct header, que é escrito no arquivo durante o começo e final da escrita. 
  bool csv_salva_em_registros(const char* nome_arquivo_csv,const char* nome_arquivo_bin);

#endif
