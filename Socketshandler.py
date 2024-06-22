import socket, threading , os
from ClientMessagesHandler import ClientMessagesHandler

class SocketsHandler():
    PORT = 9090
    MSG_BUFFER_SIZE = 1024
    ENCODING = "utf-8"
    LOCAL_IP:str = "127.0.0.1"

    server: socket.socket
    clients: dict

    def __init__(self) -> None:
        print(f"Server IP: {self.LOCAL_IP}")
        print(f"Server Port: {self.PORT}")

        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # cria uma socket do tipo AF_INET (ipv4) e do tipo sock_stream (TCP)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # reusa a socket se ela já estiver aberta
        self.server.bind((self.LOCAL_IP, self.PORT))
        self.server.listen()

        self.clients = {}  # inicializa o dict de clientes como vazio

    def __handle_one_client(self, client: socket.socket, addr: int) -> None:
        identification: str = client.recv(self.MSG_BUFFER_SIZE).decode(self.ENCODING)
        client_id: int = int(identification)
        print(f"id do client: {client_id}")
        client_handler: ClientMessagesHandler = ClientMessagesHandler(client_id)

        while True:
            try:
                message: str = client.recv(self.MSG_BUFFER_SIZE).decode(self.ENCODING)
                action_result: str = client_handler.run_functionality(message)
                action_result2 = action_result.replace("\n"," | ").replace("Busca 1","").replace("|","",1)
                action_result2 = action_result2 + '\n'

                encoded = action_result2.encode("utf-8")
                client.send(encoded)

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

    def clients_info(self) -> str:
        return str([str(address[1]) for address in self.clients.values()])

if __name__ == "__main__":
    my_socket = SocketsHandler()
    my_socket.receive_messages()
