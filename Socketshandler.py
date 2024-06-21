import socket, threading , os
from ClientMessagesHandler import ClientMessagesHandler

class SocketsHandler():
    PORT = 9090
    MSG_BUFFER_SIZE = 1024
    ENCODING = "utf-8"

    server: socket.socket
    local_ip: str
    clients: dict

    def __init__(self) -> None:
        self.local_ip = self.__get_local_ip()
        print(f"Server IP: {self.local_ip}")
        print(f"Server Port: {self.PORT}")

        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # cria uma socket do tipo AF_INET (ipv4) e do tipo sock_stream (TCP)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # reusa a socket se ela já estiver aberta
        self.server.bind((self.local_ip, self.PORT))
        self.server.listen()

        self.clients = {}  # inicializa o dict de clientes como vazio

    def __get_local_ip(self) -> str:
        if os.name.lower() == "posix":  # caso do linux
            hostname = socket.gethostname()
            local_ip = socket.gethostbyname(hostname)  # esse método pode não funcionar no windows
            return local_ip
        else:  # caso do windows
            ip: str = ""
            socket_temp = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # cria uma socket temp para conectar a outro ip e pegar o ip local
            try:
                socket_temp.connect(('10.254.254.254', 1))  # usa um outro IP sem possibilidade de rotear para achar o endereço de IP local
                ip = socket_temp.getsockname()[0]
            except Exception:
                ip = '127.0.0.1'  # caso padrão
            finally:
                socket_temp.close()
            return ip

    def __handle_one_client(self, client: socket.socket, addr: int) -> None:
        client_handler: ClientMessagesHandler = ClientMessagesHandler(addr)
        while True:
            try:
                message: str = client.recv(self.MSG_BUFFER_SIZE).decode(self.ENCODING)
                print(f"recebeu mensagem : {message} do addr {addr}")
                action_result: str = client_handler.run_functionality(message)
                print(f"resultado da funcionalidade pedida: {action_result}")
                client.send(action_result.encode(self.ENCODING))

            except:
                removed_addr = self.clients.pop(client)  # remove cliente
                client.close()  # fecha conexão com cliente

                print(f"endereço removido {str(removed_addr[1])}")
                remove_result: bool = client_handler.remove_client_data()  # remove os arquivos binários relacionados ao cliente
                if not remove_result:  # falha na remoção desses arquivos
                    print("falha ao remover arquivos do cliente que desligou a conexão")
                break  # sai do loop

    def receive_messages(self) -> None:
        print("server está ouvindo")
        while True:
            client, adress = self.server.accept()  # aceita conexão com um cliente
            print(f"conectou com endereço {str(adress[1])}")

            self.clients[client] = adress  # add cliente e seu endereço ao dict de clientes

            thread = threading.Thread(target=self.__handle_one_client, args=(client, adress[1]))  # cria um thread para cada cliente
            thread.start()  # executa a thread
            print(self.clients_info())

    def clients_info(self) -> str:
        return str([str(address[1]) for address in self.clients.values()])

if __name__ == "__main__":
    my_socket = SocketsHandler()
    my_socket.receive_messages()
