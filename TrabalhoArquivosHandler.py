import os,sys , subprocess

class TrabalhoArquivosHandler():

   LINUX_OS_IDENTIFIER = "posix" #identificadores ao rodar o comando os.name para ver se o SO é windows ou linux
   WINDOWS_OS_IDENTIFIER = "nt" 
   EXEC_NAME = "main" #nome do executável do trabalho de arquivos
   CSV_FILE_NAME = "jogador.csv"

   operating_system:str 
   run_exec_cmd:str
   pipe_in_cmd:str
   pipe_out_comand:str

   c_code_folder_path:str
   index_file_name:str
   data_file_name:str


   def __init__(self,c_code_folder_path:str = "")->None:
      
      os_name:str = os.name.lower()
      if os_name == self.LINUX_OS_IDENTIFIER:
         self.operating_system =  "linux"
      elif os_name == self.WINDOWS_OS_IDENTIFIER:
         self.operating_system = "windows"
      else:
         raise SystemError("Sistema operacional não suportado")
      
      if not c_code_folder_path:
         self.c_code_folder_path = os.path.join(os.getcwd(),"trabalho_arquivos")
      else:
         self.c_code_folder_path = c_code_folder_path

      self.__compile_c_code() #ver se o código deve ser mesmo compilado toda vez que a classe é instanciada 
      self.__get_os_specific_comands()

   def __compile_c_code(self)->None:
      """
      Importante: não enviar o trbalho com o executável, pq pode n funcionar na plataforma que o professor usar
      """
 
      make_all_cmd:str = "make all"
      compile_result = subprocess.run(make_all_cmd,cwd= self.c_code_folder_path, shell=True)

      if compile_result.returncode != 0:
         raise RuntimeError("falha na compilação")

   def __get_os_specific_comands(self)->None:
      if self.operating_system == "linux":
         self.run_exec_cmd = f"./{self.EXEC_NAME}"
      else: #so é windows
         self.run_exec_cmd = f"{self.EXEC_NAME}"

   def csv_to_binary(self, data_file_name:str, index_file_name:str)->None:
      command: str = f"{self.run_exec_cmd}"
      stdin_input: str = f"1 {self.CSV_FILE_NAME} {data_file_name}"

      result = subprocess.run(command,input=stdin_input,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)

      if result.returncode != 0:
         raise RuntimeError("falha ao criar arquivo de dados")
      
      #criar arquivo de índice

      index_cmd:str = f"4 {data_file_name} {index_file_name}"
      print(index_cmd)
      index_result:str = subprocess.run(command,input=index_cmd,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)

      if index_result.returncode != 0:
         raise RuntimeError("falha ao criar arquivo indice")
      
      self.index_file_name = index_file_name
      self.data_file_name = data_file_name
      
   def show_all_players(self,binary_file_name:str)->str:
      command: str = f"{self.run_exec_cmd}"
      stdin_input: str = f"2 {binary_file_name}"

      result = subprocess.run(command,input=stdin_input,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)

      if result.returncode != 0:
         raise RuntimeError("falha ao ler conteúdo do arquivo binario")
      
      return result.stdout

   def search_players(self,binary_file_name:str,search_dict:dict[str,str|int])->str:
      command: str = f"{self.run_exec_cmd}"
      search_info: str = f"3 {binary_file_name} 1"#vamos fazer uma busca apenas
      
      search_cmd = self.__format_player_info(
         id= search_dict.get("id",-1),
         age=search_dict.get("age",-1),
         name=search_dict.get("name",""),
         country= search_dict.get("country",""),
         club=search_dict.get("club","")
      )

      stdin_input: str = f"{search_info} \n{search_cmd}"

      result = subprocess.run(command,input=stdin_input,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)

      if result.returncode != 0:
         raise RuntimeError("falha ao ler conteúdo do arquivo binario")
      
      return result.stdout
   
   def __format_player_info(self,id:int = -1,age:int = -1,name:str = "",country:str ="" ,club:str = "")->str:
      fields = [
        f"id {id}" if id != -1 else "",
        f"idade {age}" if age != -1 else "",
        f'nomeJogador "{name.upper()}"' if name != "" else "",
        f'nacionalidade "{country.upper()}"' if country != "" else "",
        f'nomeClube "{club.upper()}"' if club != "" else "",
      ]

      valid_fields:list[str] = list(filter(lambda x: x != "", fields)) #filtra apenas os campos válidos
      fields_num:int = len(valid_fields)
      formatted_string:str = " ".join(valid_fields)
      formatted_string:str = f"{fields_num} {formatted_string}"
      return formatted_string

   def delete_players(self,binary_file_name:str,index_file_name:str ,search_dict:dict[str,str|int])->bool:
      command: str = f"{self.run_exec_cmd}"
      search_info: str = f"5 {binary_file_name} {index_file_name} 1"#vamos fazer uma remoção apenas
      
      search_cmd = self.__format_player_info(
         id= search_dict.get("id",-1),
         age=search_dict.get("age",-1),
         name=search_dict.get("name",""),
         country= search_dict.get("country",""),
         club=search_dict.get("club","")
      )

      stdin_input: str = f"{search_info} \n{search_cmd}"
      print(stdin_input)

      result = subprocess.run(command,input=stdin_input,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)

      if result.returncode != 0:
         raise RuntimeError("falha ao ler conteúdo do arquivo binario")
      
      return result.stdout


if __name__ == "__main__":
   handler = TrabalhoArquivosHandler()
   #handler.csv_to_binary("dado1.bin","index1.bin")
   #print(handler.show_all_players("dado1.bin"))
   print(handler.search_players("dado1.bin",{"country":"germany","id": 224424})) #procura antes remoção
   handler.delete_players("dado1.bin","index1.bin",{"country":"germany","id": 224424})
   print(handler.search_players("dado1.bin",{"country":"germany","id": 224424})) #procura depois