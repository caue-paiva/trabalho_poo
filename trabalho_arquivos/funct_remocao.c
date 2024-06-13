#include "funct_remocao.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/

//REMOÇÃO: funções principais da lógica de remoção

//escreve uma lista encadeada de registros logicamente removidos no arquivo de dados
//dá fseek pro começo
bool __rm_escreve_li_encad_novos_removidos(FILE*fp, const ListaEncad*lista_todos){
   assert_2ptrs(fp,lista_todos);
   assert(lista_todos->num_elem > 0);


   //escreve partes da lista no header
   char buffer;
   int read_result = fread(&buffer, sizeof(char),1,fp); //le status do header
   if (read_result != 1 ){
         d_printf("erro no fread\n");
         return false;  
   }

   read_result = fwrite(&lista_todos->cabeca->offset, sizeof(long int),1,fp); //escreve offset do primeiro elemento da lista no campo topo do header
   if (read_result != 1){
      d_printf("erro no fwrite\n");
      return false;  
   }

   No* no_atual = lista_todos->cabeca;
   long int offset_atual, offset_prox;

   while(no_atual){
      offset_atual = no_atual->offset; //pega o offset do no atual
      int seek_result = fseek(fp,offset_atual,SEEK_SET); //vai pra esse offset
      b_errno_check(seek_result);

      if (!no_atual->prox){ //prox nó é null
         offset_prox = -1;
      }else {
         offset_prox = no_atual->prox->offset;
      }

      char removido = REMOVED_REG;
      
      int result_fread = fwrite(&removido,sizeof(char),1,fp); 
      if (result_fread != 1){
            d_printf("falha no fwrite");
            return false;
      }

      int tam_registro;
      int result_read = fread(&tam_registro,sizeof(int),1,fp); //le o campo de tamanho do registro
      if (result_read != 1){
         d_printf("falha no fread");
         return false;
      }

      int result_fwrite = fwrite(&offset_prox,sizeof(long int),1,fp); //escreve o prox removido no registro
      if (result_fwrite != 1){
         d_printf("falha no fwrite");
         return false;
      }

      no_atual = no_atual->prox;
   }

   fseek(fp,0,SEEK_SET);
   return true;
}

//realiza a remoção com index, escreve no arquivo que o registro está como removido e atualiza headers e listas dos removidos com esse valor.
//Da fseek pro começo
bool __rm_remocao_logica_index(FILE*fp,Header* header, ListaEncad* ja_removidos, ListaEncad* novos_removidos_final, const long int offset, const int id){
   assert_3ptrs(fp,header,ja_removidos);
   assert(novos_removidos_final);
   
   int resultado_fseek = fseek(fp,offset,SEEK_SET); //vai pro offset do registro 
   b_errno_check(resultado_fseek);
   char removido = REMOVED_REG;
   b_file_io_check(fwrite(&removido, sizeof(char),1,fp),1); //escreve que ele é removido

   int tam_registro;
   b_file_io_check(fread(&tam_registro,sizeof(int),1,fp),1); //le o tamanho do registro
         
   int num_removidos_antes = ja_removidos->num_elem;
   li_encad_add_ordem(ja_removidos,offset,tam_registro,id); //adiciona o novo registro na lista de removidos
   int num_removidos_depois = ja_removidos->num_elem; //faz a diferença pra não contar a mais registros duplicados entre a lista de ja removidos e novos removidos
  
   if (num_removidos_depois - num_removidos_antes == 1){
      header->nroRegArq -= 1;//atualiza o número de registros arquivados e removidos
      header->nroRegRem += 1;   
   }

   li_encad_add_ordem(novos_removidos_final,offset,tam_registro,id);
   resultado_fseek = fseek(fp,0,SEEK_SET); //volta pro começo do arquivo, para realizar a busca da proxima linha
   b_errno_check(resultado_fseek);

   return true;

}

//lógica da remoção com busca sequencial, atualiza as listas de registros em memória e a struct header, essas structs
//serão utilizadas para escrever o arquivo final atualizado
void __rm_remocao_logica_sequen(Header* header, ListaEncad* ja_removidos, ListaEncad* novos_removidos_final, ListaEncad* removidos_da_linha){
     assert_3ptrs(header,ja_removidos,novos_removidos_final);
     assert(removidos_da_linha); 

      int num_removidos_antes = ja_removidos->num_elem;
      li_encad_juntar_listas_inplace(ja_removidos,removidos_da_linha); //junta listas de ja removidos e novos removidos da chamada
      li_encad_juntar_listas_inplace(novos_removidos_final,removidos_da_linha); //junta listas de todos os novos removidos na funcionalidade e novos removidos da chamada (linha)
        
      int num_removidos_depois = ja_removidos->num_elem; //faz a diferença pra não contar a mais registros duplicados entre a lista de ja removidos e novos removidos
      int novos_removidos = num_removidos_depois - num_removidos_antes; 
      
      header->nroRegArq -= novos_removidos;//atualiza o número de registros arquivados e removidos
      header->nroRegRem += novos_removidos;
}

//deleta os registros removidos da struct index em memória
bool __rm_remove_do_index(ListaEncad* novos_removidos_final, Index* index){
   assert_2ptrs(novos_removidos_final,index);

   bool resultado = true;
   No* no_atual = novos_removidos_final->cabeca;
   while (no_atual){ //remove todos os registros do index com o ID dos removidos na memória
      resultado = resultado && ind_index_mem_remove_id(index,no_atual->id);
      no_atual = no_atual->prox;
   }
   return resultado;
}

//Dado um nome de arquivo de dados e seu arquivo de index, realiza a remoção lógica dos registros que correspondem aos campos de busca
bool rm_remove_registros(const char* nome_arquivo_dados, const char*  nome_arquivo_index, const int numero_buscas){
   assert_2ptrs(nome_arquivo_dados,nome_arquivo_index);
   
   Index* index = ind_cria_carrega_index(nome_arquivo_dados, nome_arquivo_index); //tenta criar e carregar o index em memória
   if (!index){ //falha em abrir o index
      return false; //não precisa printar msg de erro que a função do index já faz isso
   }

   Header* header = rgt_header_criar(); //struct de header do arquivo de dados
   FILE* fp = rgt_header_abre_modifica(nome_arquivo_dados,header); //abre o arquivo de dados para modificação, escreve ele com inconsistente
   if (!fp){ //falha em abrir o arquivo de dados
      ind_index_mem_destroi(&index); //libera memória do index
      return false;
   }

   bool novo_removido = false;   //flag de que pelo menos um registro foi removido
   ListaEncad* ja_removidos = NULL; //essa lista vai ser criada como NULL e será inicializada de acordo com o tipo de remoção (sequencial x index)
   ListaEncad* novos_removidos_final = li_encad_criar(); //arvore para guardar todos os novos removidos ao chamar essa funcionalidade

   for (int i = 0; i < numero_buscas; i++){ //loop por cada linha da chamada da funcionalidade

      ListaEncad* removidos_linha_atual = li_encad_criar(); //registros removidos nessa chamada da funcionalidade  (linha no terminal da remoção)
      
      Registro* reg_busca = query_processa_linha_comando(-1); //chama a função para pegar a linha e realizar realizar a busca de registros pra remover
      long int offset_index = -1;
      if (reg_busca->id != FIX_CAMP_NULL_FLAG) //busca tem ID
         offset_index = ind_busca_offset_por_id(index,reg_busca->id);  //tenta achar o offset no index
      bool busca_index_flag = offset_index != -1; //se achar, vamos remover pelo index

      if (busca_index_flag){ //busca é feita com ID, e tem o offset no index
         novo_removido = true; 
         if (!ja_removidos) //lista de removidos do arquivo não existe, é NULL
             ja_removidos = query_lista_removidos_arquivo(fp); //procura pelo arquivo pela lista de removidos para criar ela pela primeira vez

         __rm_remocao_logica_index(fp,header,ja_removidos,novos_removidos_final,offset_index,reg_busca->id);//realiza a remoção no arquivo com o index e atualiza as listas e o header com essa operação       
      } else { //busca não é feita por ID, é sequencial
         if (!ja_removidos) //lista null,vamos criar uma lista vazia
            ja_removidos = li_encad_criar();

         query_busca_arquivo_e_removidos(reg_busca,fp,removidos_linha_atual,ja_removidos); //percorre o arquivo e enche as listas de registro a serem removidos na chamada atual e de reg ja removidos anteriormente
         if (removidos_linha_atual->num_elem > 0){ //temos pelo menos um novo arquivo removido
              novo_removido = true;
         }
         __rm_remocao_logica_sequen(header,ja_removidos,novos_removidos_final,removidos_linha_atual);  //logica de remoção sequencial, apenas atualiza as listas encadeadas em memória para dps escrever no
         //arquivo no final
      }

      if (ja_removidos->cabeca) //lista de já removidos tem pelo menos 1 item
         header->topo = ja_removidos->cabeca->offset; //atualiza o topo do header

      li_encad_destruir(&removidos_linha_atual); 
      rgt_registro_destruir(&reg_busca); //free na memoria do registro criado pra busca
   }

   bool arquivos_atualizados;
   if (novo_removido){ //tivemos pelo menos um registro removido em todas as chamadas das linhas
      arquivos_atualizados = __rm_escreve_li_encad_novos_removidos(fp,ja_removidos); //tenta escrever a lista atualizada de removidos no arquivo de dados
      __rm_remove_do_index(novos_removidos_final,index); //tira os registros removidos do indice
   }

   rgt_header_fecha_modifica(&fp,header); //escreve o header atualizado, coloca o arquivo como consistente e fecha ele
   rgt_header_destruir(&header); //libera memória do header
   li_encad_destruir(&ja_removidos); //destroi listas
   li_encad_destruir(&novos_removidos_final);

   //escreve index atualizado em disco
   ind_atualiza_index(nome_arquivo_index, index);
   ind_index_mem_destroi(&index); //libera memória do index em RAM

   return  arquivos_atualizados;
}

