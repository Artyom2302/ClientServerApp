import json
import pickle

from PyClient import Client
from msg import *


class Storage:
    def __init__(self):
        self.serverClient = Client()
        m = self.serverClient.request(MT_STORAGE_INIT)
        self.serverClient.id = m.Header.To

    def start(self):
        while not self.serverClient.exit:
            m = self.serverClient.processMessage()
            if m!= None:
                if m.Header.Type == MT_DATA:
                    obj = {"to": m.Header.To, "from": m.Header.From, "message": m.Data}
                    #print(obj['to'])
                    #print(type(obj))
                    self.saveMessages(obj)
                elif m.Header.Type == MT_LOAD_MESSAGES:
                    print(str(m.Header.Type) + ":" + str(m.Header.From))
                    data = self.loadMessages()
                    print(data)
                    print(type(data))
                    sendStr = "[";
                    for mes in data:
                        if mes['to'] == m.Header.From or mes['from'] == m.Header.From or mes['to'] == MR_ALL:
                            sendStr += str(mes)


                    sendStr = sendStr.replace("}{", "},{")
                    sendStr += "]"
                    print(sendStr)
                    self.serverClient.send(m.Header.From, self.serverClient.id, MT_LOAD_MESSAGES, sendStr)
            else:
                time.sleep(1)
        return 0

    def saveMessages(self, data):
        print(data)
        data1 = {}
        with open("StorageServer/messages.db", 'ab') as db:
            pickle.dump((data["to"], data["from"], data["message"]), db)

    def loadMessages(self):
        messages = []
        with open('StorageServer/messages.db', 'rb') as db:
            while True:
                try:
                    (to, from_, message) = pickle.load(db)
                    messages.append({"to": to, "from": from_, "message": message})
                except EOFError:
                    break
        return messages

if __name__ == '__main__':
    storage = Storage()
    storage.start()

