from TrabalhoArquivosHandler import TrabalhoArquivosHandler



class ClientMessagesHandler:

   """
   Classe para transformar mensagens do client do socket em JAVA em comandos para a classe de handler do trabalho de arquivos
   Serve de uma ponte para a comunicação do socket e a comunicação com o trabalho de arquivos

   Essa classe é instanciada para cada cliente com seu ID único, desse modo ao ser instanciada essa classe compila o CSV em binário
   e pode limpar os binários de clientes antigos.
   """

   __ArquivosHandler: TrabalhoArquivosHandler = TrabalhoArquivosHandler() #objeto estático interno da classe, é  a interface com o trabalho de arquivo

   client_addr:int #endereço do cliente
   client_bin_data_file:str #nome do arquivo de dados e índice do cliente
   client_bin_index_file:str

   def __init__(self, client_addr:int, clean_prev_bin_files:bool) -> None:
      self.client_addr = client_addr
      self.client_bin_data_file = f"{client_addr}_dados.bin"
      self.client_bin_index_file = f"{client_addr}_index.bin"
      if clean_prev_bin_files:
         self.remove_client_data(True)
      self.__ArquivosHandler.csv_to_binary(self.client_bin_data_file,self.client_bin_index_file)

   def __split_messages(self, message:str)->dict:
      list_of_params:list[str] = message.split(",")

      return_dict:dict[str,str|int] = {}

      for param in list_of_params:
         field_val_separator_index:int = param.find(":") 
         return_dict[param[:field_val_separator_index]] = param[field_val_separator_index+1:]

      return return_dict 

   def run_functionality(self,client_message:str)->str:
      funct_params:dict = self.__split_messages(client_message) #parâmetros que o cliente mandou na mensagem, tem que ter a funcionalidade a ser
      #rodada e se necessário os parâmetros dos jogadores
      functionality:str = funct_params.pop("functionality","") #pega a funcionalidade e remove essa key do dict
      
      functionality = functionality.strip()
      match (functionality):

         case "2": #printa todos jogadores
            result = self.__ArquivosHandler.show_all_players(self.client_bin_data_file)
            if result:
               print("Comando: buscou todos os jogadores")
            #print(f"Resultado da funcionalidade 2: {result}") # Adicione este log para depurar o resultado
            return result
         
         case "3": #procura jogador(es)
            result = self.__ArquivosHandler.search_players(self.client_bin_data_file,funct_params)
            if result:
               print("Comando: pesquisou por um jogador")
            #print(f"Resultado da funcionalidade 3: {result}") # Adicione este log para depurar o resultado
            return result
         
         case "5": #deleta um jogador
            result:bool =  self.__ArquivosHandler.delete_players(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            if result:
             print("Comando: deletou um jogador")
            return "True" if result else ""
         
         case "6": #insere um jogador
            result:bool  = self.__ArquivosHandler.insert_player(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            if result:
               print("Comando: inseriu um jogador")
            return "True" if result else ""
         
         case "7": #atualiza um jogador
            result: bool = self.__ArquivosHandler.update_player(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            if result:
               print("Comando: atualizou um jogador")
            return "True" if result else ""
         
      return "" #retorno de string vazia caso o comando n seja um desses
         
   def remove_client_data(self, remove_all_binaries:bool)->bool:
      """
      Remove os arquivos binários relativos ao cliente, usado quando o cliente encerra uma conexão
      """
      if remove_all_binaries: #remove todos os binários das conexões anteriores
         return self.__ArquivosHandler.delete_binary_files()
      else: #remove apenas alguns binários específicos
         return self.__ArquivosHandler.delete_binary_files(self.client_bin_data_file,self.client_bin_index_file)
