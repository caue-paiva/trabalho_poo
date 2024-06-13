#ifndef REG_TAM_VAR_H
  #define REG_TAM_VAR_H
   #include <stdlib.h>
   #include <stdio.h>
   #include <assert.h>
   #include <stdbool.h>
   #include <string.h>
   #include <errno.h>
   #include <ctype.h>

/*
Desenvolvido por: 

Cauê Paiva Lira - NUSP: 14675416
João Pedro Alves Notari Godoy - NUSP: 14582076

*/


   //macro para dar assert em 2 e 3 ponteiros de uma vez
   #define assert_2ptrs(p1,p2) assert(p1); assert(p2);
   #define assert_3ptrs(p1,p2,p3) assert_2ptrs(p1,p2); assert(p3);

   //macro de max e min
   #define max(x, y)  ((x) > (y) ? (x) : (y))
   #define min(x, y)  ((x) > (y) ? (y) : (x))
    
   //por enquanto deixa o errno check e o null_errno_check assim, ai dps coloca a msg de erro pedida pelo trabalho

   // se uma função (fseek por exemplo) retorna -1 ou outro numero negativo, printa um aviso e tbm a msg do errno, nao retorna da funcao principal
   #define errno_check(X) do {\
        if ((X) < 0) {\
            printf("ERROR: (VAR NAME: %s , FILE: %s , LINE: %d) -- Error name: %s \n", #X ,__FILE__,__LINE__, strerror(errno));\
        }\
        } while (0)
        //strerror vem do string.h e printa a mensagem de erro associado com o número errno
    
   //errno_check com return booleano
   #define b_errno_check(X) do {\
        if ((X) < 0) {\
            printf("ERROR: (VAR NAME: %s , FILE: %s , LINE: %d) -- Error name: %s \n", #X ,__FILE__,__LINE__, strerror(errno));\
            return false;\
        }\
      } while (0)

    //macro que checa um ponteiro FILE* e se for null, printa a mensagem errno associada com o erro,não retorna nada ,precisa de errno.h e string.h
   #define null_errno_check(ptr) do {\
        void* __null_errno_ptr = (ptr);\
        if((__null_errno_ptr) == NULL){\
           printf("NULL PTR ERROR: (VAR NAME: %s , FILE: %s , LINE: %d) -- Error name: %s \n", #ptr ,__FILE__,__LINE__, strerror(errno));\
        }\
   } while(0)

   //null_errno_check com return booleano
   #define b_null_errno_check(ptr) do {\
        void* __null_errno_ptr = (ptr);\
        if((__null_errno_ptr) == NULL){\
           printf("NULL PTR ERROR: (VAR NAME: %s , FILE: %s , LINE: %d) -- Error name: %s \n", #ptr ,__FILE__,__LINE__, strerror(errno));\
           return false;\
        }\
   } while(0)

   //macro para indicar saida do programa
   #define err_exit(...) do {\
            printf("[ERROR] (FILE: %s , LINE: %d) -- ",__FILE__,__LINE__);\
            printf(__VA_ARGS__);\
            printf("\n");\
   } while(0)

   #define default_null_error_check() do {\
        void* __null_errno_ptr = (ptr);\
        if((__null_errno_ptr) == NULL){\
           printf(DEFAULT_ERROR_MSG);\
        }\
   } while(0)

   //macro para dar warning se fread ou fwrite falharam  
   #define file_io_check(func,num_elements) do {\
    int result = (func);\
    if (result != (num_elements)) {\
        warn_printf("Fread ou Fwrite falharam");\
    }\
   } while (0)


    //macro para dar warning se fread ou fwrite falharam, retorna false se a opera falhar  
   #define b_file_io_check(func,num_elements) do {\
    int result = (func);\
    if (result != (num_elements)) {\
        warn_printf("Fread ou Fwrite falharam");\
        return false;\
    }\
   } while (0)

    // debug macros
   #define DEBUG 0 //flag de debug 
   
   #if DEBUG //printf com mais info de debug
        #define d_printf(...) do {\
            printf("[DEBUG] (FILE: %s , LINE: %d) -- ",__FILE__,__LINE__);\
            printf(__VA_ARGS__);\
            printf("\n");\
        } while(0)
   #else    //nao faz nada caso DEBUG == 0
        #define d_printf(...) (void)0
   #endif  


   //macro para mensagens de warning, o argumento precisa ser uma string
   #define warn_printf(warning) printf("[WARNING] %s (FILE: %s , LINE: %d) -- \n", (warning) ,__FILE__,__LINE__)

   #define W  "wb" //defines para lidar com arquivos em binário, todas as operações são feitas em binário
   #define R  "rb"
   #define RP "r+b" 
   #define A  "ab"

   #define FILE_NAME_MAX 64 //define para o tamanho maximo de um nome de arquivo
   #define MAX_FIELD_SIZE 1024 //define para o tamanho maximo de um campo, usado para leitura e escrita dos dados
  
   
   #define DEFAULT_ERROR_MSG "Falha no processamento do arquivo.\n"

   #define FIX_CAMP_NULL_FLAG -1 //campos fixos (id, idade...) terão seu valor nulo como -1
   #define VAR_CAMP_NULL_FLAG 0//campos variaveis nulos terão seu campo de tamanho como zero



   #define REMOVED_REG '1' //defines para verificar se o registro foi removido ou não
   #define EXISTING_REG '0'


   typedef struct f_registro Registro; //typedefs na struct do registro


   struct f_registro { //struct para representar cada registro de dados com campos fixos e variáveis
      char removido; //campos de controle do registro
      int tamanhoRegistro;
      long int Prox; 
      int id; //campos fixo de dados
      int idade;
      int tamNomeJog; //campos variaveis e seus controles de tamanho
      char* nomeJogador;
      int tamNacionalidade;
      char* nacionalidade;
      int tamNomeClube;
      char* nomeClube;
   };


   //funcoes de criar e lidar com o registro

   Registro* rgt_registro_criar(const int id, const int idade, const char* nome, const char*nacionalidade, const char*nomeClube);
   void rgt_registro_print(const Registro* registro);
   void rgt_registro_destruir(Registro** registro);
   bool rgt_registro_compara(const Registro* r1, const Registro* r2);
   Registro* rgt_registro_ler_arquivo(FILE* fp, bool* removido_flag,  int* bytes_lidos);
   bool rgt_registro_escreve_arq(const Registro* regis, FILE* fp);

  

#endif