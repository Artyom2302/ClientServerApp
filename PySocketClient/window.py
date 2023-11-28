import threading
import time

from PyQt5 import QtWidgets, uic
from PyQt5.QtWidgets import QMainWindow, QMessageBox

import msg


class MainWindow(QMainWindow):
    def Request(Self, type):
        return msg.Message.Request(msg.MR_BROKER, Self.id, type, "")

    def Send(Self, To, From, Type=msg.MT_DATA, Data=""):
        return msg.Message.SendMessage(To, From, Type, Data)

    def __init__(self):
        super(MainWindow, self).__init__()
        uic.loadUi('pyCLient.ui', self)
        self.id = 0
        self.exit = False
        self.initialize()
        self.sendBtn.clicked.connect(self.SendMessage)
        t = threading.Thread(target=self.ProcessMessages)
        t.start()

    def ShowInfo(self, text):
        mesg = QMessageBox()
        mesg.setText(text)
        mesg.setIcon(QMessageBox.Warning)
        mesg.exec_()

    def close(self):
        self.exit = True

    def initialize(self):
        m = self.Request(msg.MT_INIT)
        self.id = m.Header.To
        self.GetUserList()

    def GetUserList(self):
        self.UsersList.clear()
        m = self.Request(msg.MT_GET_USERS)
        ids = m.Data.split(' ')
        self.UsersList.addItem("All users")
        for item in ids:
            if item != "":
                self.UsersList.addItem(item)

    def ProcessMessages(self):
        while not self.exit:
            m = self.Request(msg.MT_GETDATA)
            if m.Header.Type == msg.MT_DATA:
                self.chatBox.append(str(m.Header.From) + ": " + m.Data + "\n")
            elif m.Header.Type == msg.MT_NOT_FOUND:
                self.ShowInfo("Client do not exist, try to update list of users")
            elif m.Header.Type == msg.MT_ADD_USER:
                self.UsersList.addItem(m.Data)
            elif m.Header.Type == msg.MT_DELETE_USER:
                print(m.Data)
                index = self.UsersList.findText(m.Data)
                print(index)
                self.UsersList.removeItem(index)
            else:
                time.sleep(1)
        print("exit")
        # def DeleteUser(self,id):
        #     for i in range(0,self.UsersList.count()):
        #         if id == self.UsersList.

    def SendMessage(self):
        if self.messageBox.toPlainText() != "":
            if self.UsersList.currentText() == "All users":
                self.Send(msg.MR_ALL, self.id, msg.MT_DATA, self.messageBox.toPlainText())
            else:
                self.Send(int(self.UsersList.currentText()), self.id, msg.MT_DATA, self.messageBox.toPlainText())
        else:
            self.ShowInfo("Enter message")

    def closeEvent(self, event):
        self.Send(msg.MR_BROKER, self.id, msg.MT_EXIT)
        self.exit = True


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
