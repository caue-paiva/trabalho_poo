import os, subprocess



class TrabalhoArquivosHandler():

   """
   Para em python para fazer a interface com o trabalho de arquivo por meio de comandos no terminal criados pela função run
   do módulo subprocess. Ela é capaz de identificar qual o SO do PC e usar comandos para linux ou windows. 
   Além de rodar as funcionalidades do trabalho de arquivos, essa classe tem métodos para limpar os arquivos 
   binários.
   """

   LINUX_OS_IDENTIFIER = "posix" #identificadores ao rodar o comando os.name para ver se o SO é windows ou linux
   WINDOWS_OS_IDENTIFIER = "nt" 
   EXEC_NAME = "main" #nome do executável do trabalho de arquivos
   CSV_FILE_NAME = "FIFA17-23.csv"
   NON_EXISTENT_REGISTRY_RESPONSE = "Registro inexistente."

   operating_system:str 
   run_exec_cmd:str
   delete_files_cmd:str
   delete_all_bin_files_cmd:str

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
      print("c compilado")
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
         self.delete_files_cmd = f"rm -f " #comando para deletar um arquivo especifico
         self.delete_all_bin_files_cmd = f"rm -f *.bin" #comando para limpar todos os binários
      else: #so é windows
         self.run_exec_cmd = f"{self.EXEC_NAME}.exe"
         self.delete_files_cmd = f"del /f "
         self.delete_all_bin_files_cmd = f'powershell -Command "Remove-Item -Force -Recurse *.bin"'

   def __run_command(self,stdin:str)->str:
      """
      roda o arquivo executável e passa para ele inputs no stdin que são dados como parâmetros
      """
      run_exec_command: str = f"{self.run_exec_cmd}"

      result = subprocess.run(run_exec_command,input=stdin,capture_output=True,text=True,shell=True,cwd=self.c_code_folder_path)
      if result.returncode != 0:
         raise RuntimeError(f"Falha ao executar comando com input {stdin}. Código de retorno: {result.stdout}. Erro: {result.stderr}")
      return result.stdout
      
   def csv_to_binary(self, data_file_name:str, index_file_name:str)->None:
     
      create_data_file_cmd: str = f"1 {self.CSV_FILE_NAME} {data_file_name}"
      self.__run_command(create_data_file_cmd)
      
      #criar arquivo de índice

      index_cmd:str = f"4 {data_file_name} {index_file_name}"
      self.__run_command(index_cmd)
      
      self.index_file_name = index_file_name
      self.data_file_name = data_file_name
      
   def show_all_players(self,binary_data_file_name:str)->str:
      show_all_players_cmd: str = f"2 {binary_data_file_name}"
      return self.__run_command(show_all_players_cmd)

   def search_players(self,binary_file_name:str,player_fields:dict[str,str|int])->str:
      search_info: str = f"3 {binary_file_name} 1"#vamos fazer uma busca apenas
      search_params = self.__format_player_info(
         id= player_fields.get("id",-1),
         age=player_fields.get("age",-1),
         name=player_fields.get("name",""),
         country= player_fields.get("country",""),
         club=player_fields.get("club","")
      )

      search_cmd: str = f"{search_info} \n{search_params}"
      return self.__run_command(search_cmd)
   
   def __format_player_info(self, is_insertion:bool = False ,id:int = -1,age:int = -1,name:str = "",country:str ="" ,club:str = "")->str:
      
      if not is_insertion: #formatação para busca e remoção
         fields = [
            f"id {id}" if id != -1 else "",
            f"idade {age}" if age != -1 else "",
            f'nomeJogador "{name.upper().strip()}"' if name != "" else "",
            f'nacionalidade "{country.upper().strip()}"' if country != "" else "",
            f'nomeClube "{club.upper().strip()}"' if club != "" else "",
         ]

         valid_fields:list[str] = list(filter(lambda x: x != "", fields)) #filtra apenas os campos válidos
         fields_num:int = len(valid_fields)
         formatted_string:str = " ".join(valid_fields)
         formatted_string:str = f"{fields_num} {formatted_string}"
         return formatted_string
      else: #formatação para inserção
         if id == -1:
            raise IOError("Na inserção é obrigátorio colocar o ID do usuário")
         
         name_str = f'"{name}"' if name else 'NULO'
         country_str = f'"{country}"' if country else 'NULO'
         club_str = f'"{club}"' if club else 'NULO'

         return f'{id} {age} {name_str} {country_str} {club_str}'.upper()

   def __parse_stdout_result(self,stdout:str)->bool:
      if not stdout or stdout.isspace(): #stdout vazio, busca,remoção e printar não achou ngm
         return False
      
      if "falha" in stdout.lower():
         return False
      
      if self.NON_EXISTENT_REGISTRY_RESPONSE in stdout: #registro inexistente
         return False
      
      return True

   def delete_players(self,binary_file_name:str,index_file_name:str ,player_fields:dict[str,str|int])->bool:
      search_info: str = f"5 {binary_file_name} {index_file_name} 1"#vamos fazer uma remoção apenas
      search_cmd = self.__format_player_info(
         id= player_fields.get("id",-1),
         age=player_fields.get("age",-1),
         name=player_fields.get("name",""),
         country= player_fields.get("country",""),
         club=player_fields.get("club","")
      )

      delete_cmd: str = f"{search_info} \n{search_cmd}"
      stdout_result = self.__run_command(delete_cmd)
      return self.__parse_stdout_result(stdout_result)

   def insert_player(self,binary_file_name:str,index_file_name:str ,player_fields:dict[str,str|int])->bool:
      if player_fields.get("id") is None:
         print("atualização do jogador deve conter o ID do mesmo, por seu uma operação sobre um único jogador")
         return False
      
      insert_cmd_header: str = f"6 {binary_file_name} {index_file_name} 1"
      insert_fields = self.__format_player_info(
         is_insertion= True,
         id= player_fields.get("id",-1),
         age=player_fields.get("age",-1),
         name=player_fields.get("name",""),
         country= player_fields.get("country",""),
         club=player_fields.get("club","")
      )

      insert_cmd:str = f"{insert_cmd_header} \n{insert_fields}"
      stdout: str = self.__run_command(insert_cmd)
      return self.__parse_stdout_result(stdout)

   def update_player(self,binary_file_name:str,index_file_name:str ,player_fields:dict[str,str|int])->bool:
      if player_fields.get("id") is None:
         print("atualização do jogador deve conter o ID do mesmo, por seu uma operação sobre um único jogador")
         return False
      
      #deleta jogador, vamos deletar apenas pelo ID
      deletion_result:bool = self.delete_players(binary_file_name,index_file_name,{"id":player_fields["id"]})
      if not deletion_result:
         print("falha na atualização de jogador: Remoção de um já existente falhou. Verifique se o jogador realmente existe")
         return False
      
      #insere ele com novos dados
      insertion_result: bool = self.insert_player(binary_file_name,index_file_name,player_fields)
      if not insertion_result:
         print("falha na atualização de jogador: Inserção dos novos dados falhou")
         return False
      
      return True      

   def delete_binary_files(self,binary_file_name:str = "",index_file_name:str="")->bool:
      """
      Deleta o arquivo binário de dados e de índice passados como parâmetro, caso nenhum parâmetro seja passado,
      deleta todos os arquivos binários do diretório
      """
      if not binary_file_name and not index_file_name: #vai deletar todos os arquivos binários no folder
         delete_files_cmd:str = self.delete_all_bin_files_cmd
      else:
         delete_files_cmd: str =f"{self.delete_files_cmd} {binary_file_name} {index_file_name}" #deleta apenas alguns arquivos binarios
      
      #roda comando para deletar arquivos binários
      result = subprocess.run(delete_files_cmd,text=True,shell=True,cwd=self.c_code_folder_path)
      if result.returncode != 0:
         return False
      return True
