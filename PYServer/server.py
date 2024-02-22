import socket
import threading
import select

class TCPServer:
    def __init__(self, host='0.0.0.0', port=33789):
        self.host = host
        self.port = port
        self.clients = []
        self.server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.server_socket.bind((self.host, self.port))
        self.server_socket.listen()

    def broadcast(self, message, source_client):
        for client in self.clients:
            if client != source_client:
                try:
                    client.send(message)
                except Exception as e:
                    print(f"Error sending message to a client: {e}")
                    client.close()
                    self.clients.remove(client)

    def handle_client(self, client_socket):
        while True:
            try:
                raw_message = client_socket.recv(1024)
                if raw_message:
                    print(f"Received raw data: {raw_message}")
                    self.broadcast(raw_message, client_socket)
            except Exception as e:
                print(f"Error with a client: {e}")
                client_socket.close()
                self.clients.remove(client_socket)
                break

    def start(self):
        print(f"Server running on {self.host}:{self.port}")
        while True:
            try:
                client_socket, client_address = self.server_socket.accept()
                self.clients.append(client_socket)
                print(f"Accepted new connection from {client_address}")
                client_thread = threading.Thread(target=self.handle_client, args=(client_socket,))
                client_thread.start()
            except Exception as e:
                print(f"Error accepting new connection: {e}")

if __name__ == "__main__":
    server = TCPServer()
    server.start()
