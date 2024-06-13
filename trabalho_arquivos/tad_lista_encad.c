#include "tad_lista_encad.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/

//aloca memória para o nó
No* __no_criar(const long int offset , const int tam_registro , const int id){
   No* no = (No*) malloc (sizeof(No));
   assert(no);

   no->offset = offset;
   no->tam_registro = tam_registro;
   no->id = id;
   no->prox = NULL;
   return no; 
}

//aloca memória para a lista
ListaEncad* li_encad_criar(){

   ListaEncad* lista = (ListaEncad*) malloc(sizeof(ListaEncad));
   assert(lista);

   lista->cabeca = NULL;
   lista->num_elem = 0;
   return lista;
}

//printa a lista encad
void li_encad_print(const ListaEncad* lista){
   if (!lista->cabeca){
      printf("Lista vazia \n");
      return;
   }

   No* atual = lista->cabeca;
   printf("Lista encadeada de tamanho %d : \n [ |", lista->num_elem);
   while(atual){
         printf("tam: %d , offset: %ld  , id %d | ", atual->tam_registro, atual->offset,atual->id);
         atual = atual->prox;
   }
   printf(" ]\n");
}
//libera memória de uma lista encad
void li_encad_destruir(ListaEncad** lista){
   assert_2ptrs(lista,*lista);

   No* no_atual = (*lista)->cabeca;
   No* temp;
   while(no_atual){
      temp = no_atual->prox;
      free(no_atual);
      no_atual = temp;
   }

   free(*lista);
   *lista = NULL;
}

//essa função adiciona nós na lista encadeada em ordem
void li_encad_add_ordem(ListaEncad* lista, const long int offset, const int tam_registro , const int id){
   assert(lista);

   if (li_encad_offset_existe(lista,offset)){
      return; //item repetido na lista
   }

   No* novo_no = __no_criar(offset,tam_registro,id);
   assert(novo_no);

   if (!lista->cabeca){ //lista vazia
      lista->cabeca = novo_no; //coloca no na cabeça
   } else {
      
      if(lista->cabeca->tam_registro > tam_registro){ //novo tamanho é menor que a cabeça atual, vira nova cabeça
         novo_no->prox = lista->cabeca;
         lista->cabeca = novo_no;
      } else { //achar lugar para colocar o novo no

         No* no_atual = lista->cabeca;
         No* prox;
         while (no_atual){
            prox = no_atual->prox;
            if (!prox || (prox->tam_registro > tam_registro)){
               no_atual->prox = novo_no;
               novo_no->prox = prox;
               break;
            }
            no_atual = prox;
         }
      }
   }
   lista->num_elem++;
}

//retorna uma lista com os conteúdos da lista1 e lista2 em ordem
ListaEncad*  li_encad_juntar_listas(const ListaEncad* lista1, const ListaEncad* lista2){
   assert_2ptrs(lista1,lista2);
   ListaEncad* nova_lista = li_encad_criar();

   No* no1 = lista1->cabeca;
   while(no1){
      li_encad_add_ordem(nova_lista,no1->offset,no1->tam_registro,no1->id);
      no1 = no1->prox; 
   }

   No* no2 = lista2->cabeca;
   while(no2){
      li_encad_add_ordem(nova_lista,no2->offset,no2->tam_registro,no2->id);
      no2 = no2->prox; 
   }

   return nova_lista;
}

//add os conteudos da lista 2 na lista 1
void  li_encad_juntar_listas_inplace(ListaEncad* lista1, const ListaEncad* lista2){
   assert_2ptrs(lista1,lista2);
   No* no2 = lista2->cabeca;
   while(no2){
      li_encad_add_ordem(lista1,no2->offset,no2->tam_registro,no2->id);
      no2 = no2->prox; 
   }
}

//retorna true se um offset existe na lista encadeada, false se não
bool li_encad_offset_existe(const ListaEncad*lista, const long int offset){
   assert(lista);
   if (lista->num_elem == 0) //lista vazia
      return false;
   No* no_atual = lista->cabeca;
   while (no_atual){ //loop pelos nos
      if (no_atual->offset == offset){
         return true;
      }
      no_atual = no_atual->prox;
   }
   return false; //não achou
}

//acha o nó da lista encadeada que corresponde ao best-fit e remove e retorna ele
//tambem muda o valor de variaveis parametro que ditam qual o offset do nó anterior e do nó prox do removido
No* li_encad_best_fit_pop(ListaEncad* lista, const int tamanho, long int* offset_anterior,long int* offset_prox){
   assert(lista);

   if (!lista->cabeca){ //lista vazia
      return NULL;
   }
   No* atual = lista->cabeca;
   No* prox;
   No* no_retorno;

   if(atual->tam_registro >= tamanho){//caso de que vamos tirar da lista a cabeça
      no_retorno = __no_criar(atual->offset,atual->tam_registro,atual->id);  //cria um nó de retorno
      No* prox = atual->prox;
      *offset_anterior = -1; //offset anterior é nulo
      if (prox)
         *offset_prox = prox->offset;
      else
         *offset_prox = -1; //offset do prox é nulo
      free(atual); //libera memoria do nó
      lista->cabeca = prox; //atualiza cabeça
      return no_retorno;
   }

   while (atual){//se entrou nesse caso então o fit vai estar depois da cabeça
      prox = atual->prox;
      if(!prox){ //vamos sempre comparar com o tamanho do prox, se ele é nulo então a busca foi falha
         return NULL;
      }

      if (prox->tam_registro >= tamanho){ //primeiro fit na lista ordenada crescente é o best-fit
         no_retorno = __no_criar(prox->offset,prox->tam_registro,prox->id); //cria um nó de retorno
         No*no_remo = prox;
         atual->prox = prox->prox;
         *offset_anterior = atual->offset;
         if (no_remo->prox)
            *offset_prox = no_remo->prox->offset;
         else 
            *offset_prox = -1;
         free(no_remo);
         return no_retorno;
      }
      atual = prox; //vai pro próximo nó
   }

   return NULL;
}