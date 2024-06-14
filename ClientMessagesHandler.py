from TrabalhoArquivosHandler import TrabalhoArquivosHandler

__ArquivosHandler: TrabalhoArquivosHandler = TrabalhoArquivosHandler()

class ClientMessagesHandler:

   @staticmethod
   def _split_messages(message:str)->dict:
      list_of_params:list[str] = message.split(",")

      return_dict:dict[str,str|int] = {}

      for param in list_of_params:
         field_val_separator_index:int = param.find(":") 
         return_dict[param[:field_val_separator_index]] = param[field_val_separator_index+1:]

      return return_dict 
   
   @staticmethod
   def run_functionality(message:str, client_addr:int)->str:
      data_file_name: str = f"dado_{client_addr}.bin"
      index_file_name: str = f"index_{client_addr}.bin"

      funct_params:dict = ClientMessagesHandler._split_messages(message)
      functionality:str = funct_params.get("functionality","")
      if not functionality:
         raise IOError("messagem do socket deve conter uma funcionalidade do trabalho de arquivos")

      match (functionality):

         case "1": #criar arquivo bin
            pass
         case "2": #printa todos jogadores
            __ArquivosHandler.show_all_players()
         case "3":
            pass


   @staticmethod
   def get_player_fields(message:str)->dict:
       pass
   
   @staticmethod
   def get_binary_file_names(message:str)->tuple[str]:
       pass
   

print(ClientMessagesHandler._split_messages("funcionality:1,id:1298,age:32,country:GERMANY,club:asas,name:suhu"))