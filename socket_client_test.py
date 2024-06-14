import socket
import threading
import time

SERVER_IP = "127.0.1.1" # Change this to your server's local IP address
SERVER_PORT = 9090
MSG_BUFFER_SIZE = 1024
ENCODING = "utf-8"

def client_task(client_id):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_IP, SERVER_PORT))
    message = f"funcionalidade:1,id:1298"
    client_socket.send(message.encode(ENCODING))
    
    response = client_socket.recv(MSG_BUFFER_SIZE).decode(ENCODING)
    print(f"Client {client_id} received: {response}")
    
    client_socket.close()

if __name__ == "__main__":
    client_threads = []
    num_clients = 5  # Number of clients to test

    for i in range(num_clients):
        thread = threading.Thread(target=client_task, args=(i,))
        client_threads.append(thread)
        thread.start()
        time.sleep(1)  # Stagger the client connections slightly

    for thread in client_threads:
        thread.join()
