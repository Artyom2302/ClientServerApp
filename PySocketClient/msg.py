import threading
from dataclasses import dataclass
import socket, struct, time

MT_INIT = 0
MT_EXIT = 1
MT_GETDATA = 2
MT_DATA = 3
MT_NODATA = 4
MT_CONFIRM = 5
MT_GET_USERS = 6
MT_NOT_FOUND = 7
MT_ADD_USER = 8
MT_DELETE_USER = 9
MT_STORAGE_INIT = 10
MT_LOAD_MESSAGES = 11

MR_STORAGE = -3
MR_BROKER = -2
MR_ALL = -1
MR_USER = 0


@dataclass
class MsgHeader:
    To: int = 0
    From: int = 0
    Type: int = 0
    Size: int = 0

    def Send(self, s):
        s.send(struct.pack(f'iiii', self.To, self.From, self.Type, self.Size))

    def Receive(self, s):
        try:
            (self.To, self.From, self.Type, self.Size) = struct.unpack('iiii', s.recv(16))
        except:
            self.Size = 0
            self.Type = MT_NODATA


class Message:

    def __init__(self, To=0, From=0, Type=MT_DATA, Data=""):
        self.Header = MsgHeader(To, From, Type, len(Data))
        self.Data = Data

    def Send(self, s):
        self.Header.Send(s)
        if self.Header.Size > 0:
            s.send(struct.pack(f'{self.Header.Size}s', self.Data.encode('cp866')))
            print(self.Data.encode('cp866'))

    def Receive(self, s):
        self.Header.Receive(s)
        if self.Header.Size > 0:
            self.Data = struct.unpack(f'{self.Header.Size}s', s.recv(self.Header.Size))[0].decode('cp866')

    @staticmethod
    def Request(To, From, Type=MT_DATA, Data=""):
        HOST = 'localhost'
        PORT = 12345
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            m = Message(To, From, Type, Data)
            m.Send(s)
            m.Receive(s)
            return m

    @staticmethod
    def SendMessage(To, From, Type=MT_DATA, Data=""):
        HOST = 'localhost'
        PORT = 12345
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            m = Message(To, From, Type, Data)
            m.Send(s)
