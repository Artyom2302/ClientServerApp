import json
import threading
import time
import requests
from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QMainWindow, QMessageBox,QTextEdit

from msg import *


class MainWindow(QMainWindow):


    def __init__(self):
        super(MainWindow, self).__init__()
        uic.loadUi('pyCLient.ui', self)
        self.id = 0
        self.exit = False
        self.initialize()
        self.eventStop = threading.Event()
        self.sendBtn.clicked.connect(self.SendMessage)
        t = threading.Thread(target=self.ProcessMessages)
        t.start()
        self.jsonData = ""
        self.messageHistory()

    # def getUserlistCycle(self):
    #     while  not self.exit:
    #         self.GetUserList()
    #         time.sleep(5)
    def ShowInfo(self, text):
        mesg = QMessageBox()
        mesg.setText(text)
        mesg.setIcon(QMessageBox.Warning)
        mesg.exec_()

    def close(self):
        self.exit = True
    def createURL(self,type):
        return 'http://localhost:8080/cgi-bin/webClient.py?type='+str(type)+'&id='+str(self.id)
    def createSendURL(self, type,idTo):
        return self.createURL(type) + '&idTo=' + str(idTo)
    def initialize(self):
        url = self.createURL("init")
        res = requests.get(url)
        self.id = int(res.content)
        self.GetUserList()
        self.setWindowTitle(f"Client #{self.id}")

    def messageHistory(self):
        self.eventStop.set()
        try:
            url = "http://localhost:8080/cgi-bin/webClient.py/api?id="+str(self.id)
            res = requests.get(url)
            print(res.content)
            if res.status_code == 200:
                print(res.headers)
                mesList = json.loads(res.content.decode("utf-8"))
                for mes in mesList:
                     self.chatBox.append(f"{mes['from']}: {mes['message']}")
            self.eventStop.clear()
        except Exception as ex:
            print(ex)
    def GetUserList(self):
        url = self.createURL("getUserList")
        res = requests.get(url)
        ids = res.content.decode("utf-8").split(' ')
        self.UsersList.clear()
        self.UsersList.addItem("All users")
        print(ids)
        for item in ids:
            if item != "\n":
                self.UsersList.addItem(item)

    def ProcessMessages(self):
        while not self.exit:
            url = self.createURL("get")
            res = requests.get(url)
            data = res.content.decode("utf-8")
            data = data.split(" ")
            type = data[0]
            if type != '':
                mesfrom = data[1]
                if type == str(MT_DATA):
                    self.chatBox.append(mesfrom+ ": " + data[2])
                # elif type == str(MT_NOT_FOUND):
                #     self.ShowInfo("Client do not exist, try to update list of users")
                elif type == str(MT_ADD_USER):
                    self.UsersList.addItem(data[2])
                elif type == str(MT_DELETE_USER):
                    index = self.UsersList.findText(data[2])
                    self.UsersList.removeItem(index)
            else:
                time.sleep(2)




    def SendMessage(self):
        if self.messageBox.toPlainText() != "":
            # if self.UsersList.currentText() == "All users":
            #     url = self.createSendURL("send",self.messageBox.toPlainText(), MR_ALL)
            # else:
            #     self.createSendURL("send",self.messageBox.toPlainText(), int(self.UsersList.currentText()))
            url = self.createSendURL("send", MR_ALL if self.UsersList.currentText() == "All users" else self.UsersList.currentText())
            data = {'message': self.messageBox.toPlainText()}
            res = requests.post(url, data)
        else:
            self.ShowInfo("Enter message")
        self.GetUserList()

    def closeEvent(self, event):
        self.exit = True


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
