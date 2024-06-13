#ifndef LISTA_ENCAD_H
   #define LISTA_ENCAD_H
   #include "tad_header.h"


  typedef struct no No;

  struct no {
      long int offset;
      int tam_registro;
      int id;
      No* prox; 
  };

  typedef struct {
      No* cabeca;
      int num_elem;
  } ListaEncad;

   ListaEncad* li_encad_criar(); //cria uma lista encad
   void li_encad_print(const ListaEncad* lista);
   void li_encad_destruir(ListaEncad** lista); //libera memoria da lista
   void li_encad_add_ordem(ListaEncad* lista, const long int offset, const int tam_registro , const int id);    //add um elemento em ordem
   ListaEncad*  li_encad_juntar_listas(const ListaEncad* lista1, const ListaEncad* lista2); //retorna uma lista ordenada da junção de 2 outras
   bool li_encad_offset_existe(const ListaEncad*lista, const long int offset); //diz se um offset existe na lista
   No* li_encad_best_fit_pop(ListaEncad* lista, const int tamanho, long int* offset_anterior,long int* offset_prox); //remove e retorna o nó que corresponde ao best fit
   void li_encad_juntar_listas_inplace(ListaEncad* lista1, const ListaEncad* lista2); 

#endif