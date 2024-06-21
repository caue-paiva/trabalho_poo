from TrabalhoArquivosHandler import TrabalhoArquivosHandler

__ArquivosHandler: TrabalhoArquivosHandler = TrabalhoArquivosHandler() #objeto singleton interno do módulo, é  a interface com o trabalho de arquivo


class ClientMessagesHandler:

   client_addr:int #endereço do cliente
   client_bin_data_file:str #nome do arquivo de dados e índice do cliente
   client_bin_index_file:str

   def __init__(self, client_addr:int) -> None:
      self.client_addr = client_addr
      self.client_bin_data_file = f"{client_addr}_dados.bin"
      self.client_bin_index_file = f"{client_addr}_index.bin"

   def __split_messages(message:str)->dict:
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
      
      if not functionality:
         raise IOError("messagem do socket deve conter uma funcionalidade do trabalho de arquivos")

      match (functionality):
         case "1": #criar arquivo bin
            pass
         
         case "2": #printa todos jogadores
            return __ArquivosHandler.show_all_players(self.client_bin_data_file)
         
         case "3": #procura jogador(es)
            return __ArquivosHandler.search_players(self.client_bin_data_file,funct_params)
         
         case "5": #deleta um jogador
            result:bool =  __ArquivosHandler.delete_players(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            return "True" if result else ""
         
         case "6": #insere um jogador
            result:bool  = __ArquivosHandler.insert_player(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            return "True" if result else ""
         
         case "7": #atualiza um jogador
            result: bool = __ArquivosHandler.update_player(self.client_bin_data_file,self.client_bin_index_file,funct_params)
            return "True" if result else ""
         
   def remove_client_data(self)->bool:
      """
      Remove os arquivos binários relativos ao cliente, usado quando o cliente encerra uma conexão
      """
      return __ArquivosHandler.delete_binary_files(self.client_bin_data_file,self.client_bin_index_file)

   

#print(ClientMessagesHandler.__split_messages("funcionality:1,id:1298,age:32,country:GERMANY,club:asas,name:suhu"))