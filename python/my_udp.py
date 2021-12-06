import socket
import sys
import errno

class MyUDP:
    class Mode:
        SEND = 0
        RECV_BLOCKING = 1
        RECV_NON_BLOCKING = 2

    def __init__(self, address: str, port: int, mode: Mode):
        self.address = address
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        if mode == MyUDP.Mode.RECV_BLOCKING or mode == MyUDP.Mode.RECV_NON_BLOCKING :
            self.sock.bind((self.address, self.port))
        if mode == MyUDP.Mode.RECV_NON_BLOCKING :
            self.sock.setblocking(False)

    def send(self, buf):
        self.sock.sendto(buf, (self.address, self.port))

    def recv(self) -> bytes:
        try:
            data, addr = self.sock.recvfrom(1024)
            print(type(data))
            return data
        except socket.error as e:
            err = e.args[0]
            if err == errno.EAGAIN or err == errno.EWOULDBLOCK:
                print("[MyUDP:recv] No data available")
                return None
            else:
                print("[MyUDP:recv] Error occured")
                print(e)
                sys.exit(1)

if __name__ == "__main__":
    udp_send = MyUDP("127.0.0.1", 5678, MyUDP.Mode.SEND)
    while(True):
        data = "message from python"
        data = data.encode("utf-8")
        udp_send.send(data)

    # udp_recv = MyUDP("127.0.0.1", 1234, MyUDP.Mode.RECV_BLOCKING)
    # while(True):
    #     data = udp_recv.recv()
    #     print(data)

