from msg import *


class Client:
    def __init__(self):
        self.id = 0
        self.exit = False
        #self.initialize()

    def request(self, type):
        return Message.Request(MR_BROKER, self.id, type, "")

    def send(self, To, From, Type=MT_DATA, Data=""):
        return Message.SendMessage(To, From, Type, Data)

    def initialize(self):
        m = self.request(MT_INIT)
        self.id = m.Header.To

    def processMessage(self):
        m = self.request(MT_GETDATA)
        if m.Header.Type != MT_NODATA:
            return m
        return None

    def close(self):
        self.Send(MR_BROKER, self.id, MT_EXIT)
        self.exit = True

