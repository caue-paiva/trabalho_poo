#include "funct_csv_reader.h" 
#include "funct_query_file.h"
#include "funct_remocao.h"
#include "funct_insercao.h"
#include "funct_tad_index.h"
#include "funcoes_miscelaneas.h"

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/



int main(){
  
   int acao_user = -1; //variavel da ação do usuario
  
   char nome_csv_buffer[FILE_NAME_MAX]; //buffers para ler o nome de arquivos
   char nome_bin_buffer[FILE_NAME_MAX];
   char nome_index_buffer[FILE_NAME_MAX];

   scanf("%d",&acao_user);   //pega a ação do usuario
   switch(acao_user){ 

         case 1: //Caso 1 
            scanf("%s %s",nome_csv_buffer,nome_bin_buffer); //le o nome do arquivo csv e arqui binario
            bool op_sucesso = csv_salva_em_registros(nome_csv_buffer,nome_bin_buffer); //tenta ler o CSV e salvar os registros no arquivo binario
            if (op_sucesso)           
               misc_binarioNaTela(nome_bin_buffer); //se a operacao teve sucesso, printa os conteudos do novo arquivo
            else
               printf(DEFAULT_ERROR_MSG);   //senao printa msg de erro            
         break;

         case 2: //Caso 2, printar os conteudos de um arquivo binário
            scanf("%s",nome_bin_buffer); //pega o nome do arquivo binario do usuario
            rgt_printa_arquivo(nome_bin_buffer); //chama uma função de printar os conteudos de um arquivo binário
         break;

         case 3: //Caso 3, busca por registros 
            int num_queries;
            scanf("%s %d",nome_bin_buffer, &num_queries); //pega o nome do arquivo binario do usuario
           
            int c;
            while ((c = getchar()) != '\n' && c != EOF) { } //consome a newline do começo da print
            bool resultado_queries = query_realizar_pesquisa(nome_bin_buffer,num_queries); //realiza a operação de pesquisa
            if (!resultado_queries) //caso ocorra algum erro na pesquisa
               printf(DEFAULT_ERROR_MSG);
         break;

         case 4: //funcionalidade 4, cria index e usa binario na tela nele
            scanf("%s %s", nome_bin_buffer,nome_index_buffer);
            bool resultado_cria_index = ind_cria_index(nome_bin_buffer,nome_index_buffer); //tenta criar o index
            if (resultado_cria_index) //se tiver sucesso chama a função de binário na tela
               misc_binarioNaTela(nome_index_buffer);
         break;

         case 5: //funcionalidade 5, remoção com auxílio do index
            int d;
            int num_remocao;
            scanf("%s %s %d", nome_bin_buffer,nome_index_buffer,&num_remocao);
            while ((d = getchar()) != '\n' && d != EOF) { } //consome a newline do começo da print
            bool remocao_result = rm_remove_registros(nome_bin_buffer,nome_index_buffer,num_remocao); //tenta remover os registros
            if (remocao_result){ //se teve sucesso, chama binario na tela
               misc_binarioNaTela(nome_bin_buffer);
               misc_binarioNaTela(nome_index_buffer);
            }else  //senão printa msg de erro
               printf("Registro inexistente.");
         break;


         case 6: //funcionalidade 6, inserção e no final cria um index com os dados atualizados
            int num_insercoes;
            scanf("%s %s %d", nome_bin_buffer,nome_index_buffer, &num_insercoes);
            while ((d = getchar()) != '\n' && d != EOF) { } //consome a newline do começo da print
            bool insercao_result = ins_insere_registros(nome_bin_buffer,nome_index_buffer,num_insercoes); //tenta inserir registros
            
            if (insercao_result){ //index criado com sucesso
               misc_binarioNaTela(nome_bin_buffer);  //printa binario na tela
               misc_binarioNaTela(nome_index_buffer);
            }else //falha em criar index
               printf("Falha na inserção");
       
         break;

         /*
         case 39: //DEBUG: printa lista de removidos do arquivo
            scanf("%s", nome_bin_buffer);
            while ((d = getchar()) != '\n' && d != EOF) { } //consome a newline do começo da print
            FILE* fp = fopen(nome_bin_buffer,R);
            ListaEncad* lista2 = query_lista_removidos_arquivo(fp);
            li_encad_print(lista2);
         break; 
         */
        

         default: //ação não valida
         printf("Acao nao valida, tente de novo\n");
      }

}



