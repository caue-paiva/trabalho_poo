#ifndef MISC_FN_H
   #define MISC_FN_H
   #include "tad_header.h"

   /*
   Funções miscelâneas utilizadas por diversas funcionalidades e sem vínculo forte a nenhum módulo ou subrotina
   */


   //funções de manipulação de string, usadas na funcionalidade de query, ler CSV entre outras
   void misc_remove_chars_lixo(char* linha);
   void misc_scan_quote_string(char *str);
   void misc_str_remove_newline(char* linha);
   bool misc_str_somente_whitespace(const char *linha);
   void misc_binarioNaTela(char *nomeArquivoBinario);


#endif

