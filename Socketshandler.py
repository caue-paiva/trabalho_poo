import socket
from ClientMessagesHandler import ClientMessagesHandler



class SocketsHandler():

    """
    Classe em Python para trabalhar como o servidor do sistema de comunicação de Sockets, tem constantes de classe como o IP e a porta do servidor.
    Seus métodos implementam loops infinitos onde o servidor espera receber e depois enviar mensagens relacionadas à funcionalidade para os clientes
    """

    PORT = 9090
    MSG_BUFFER_SIZE = 1024
    ENCODING = "utf-8"
    LOCAL_IP: str = "127.0.0.1"

    server: socket.socket
    clients: dict
    client_id: int
    has_client_id: bool

    def __init__(self) -> None:
        print(f"Server IP: {self.LOCAL_IP}")
        print(f"Server Port: {self.PORT}")

        self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # cria uma socket do tipo AF_INET (ipv4) e do tipo sock_stream (TCP)
        self.server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)  # reusa a socket se ela já estiver aberta
        self.server.bind((self.LOCAL_IP, self.PORT))
        self.server.listen()
        self.client_id = -1

        self.clients = {}  # inicializa o dict de clientes como vazio

    def __handle_one_client(self, client: socket.socket, client_handler: ClientMessagesHandler) -> None:
        while True:
            try:
                message: str = client.recv(self.MSG_BUFFER_SIZE).decode(self.ENCODING) #recebe mensagem do cliente
                action_result: str = client_handler.run_functionality(message) #roda funcionalidade do trabalho de arquivos            
                action_result2 = action_result.replace("\n", " | ").replace("Busca 1", "").replace("|", "", 1) #parsing na string
                action_result2 = action_result2 + '\n' #coloca \n no final da str para servir como o delimitador final
                encoded = action_result2.encode("utf-8") 
                client.send(encoded) #manda mensagem para o cliente

            except Exception as e:
                removed_addr = self.clients.pop(client)  # remove cliente
                client.close()  # fecha conexão com cliente
                break  # sai do loop

    def receive_messages(self) -> None:
        print("server está ouvindo")
        client_handler: ClientMessagesHandler = None #handler do cliente ainda não existe

        while True:
            client, adress = self.server.accept()  # aceita conexão com um cliente
        
            if self.client_id == -1: #não tem id do cliente
                identification: str = client.recv(self.MSG_BUFFER_SIZE).decode(self.ENCODING) #recebe id do cliente
                self.client_id: int = int(identification) #configura id do cliente
                print("recebeu id do cliente")
                client_handler = ClientMessagesHandler(self.client_id,True) #instancia o handler do cliente
                
            self.clients[client] = adress  # add cliente e seu endereço ao dict de clientes
            self.__handle_one_client(client, client_handler)

    def clients_info(self) -> str:
        return str([str(address[1]) for address in self.clients.values()])

if __name__ == "__main__": #roda o server de socket
    my_socket = SocketsHandler()
    my_socket.receive_messages()
