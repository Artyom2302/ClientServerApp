import json
import os, sys, re, codecs, binascii, cgi, cgitb, datetime, pickle
import http.cookies
import time

import msg
from msg import *

cgitb.enable()
sys.stdout = codecs.getwriter('utf-8')(sys.stdout.detach())


class Messenger:
    def Request(self, type):
        return msg.Message.Request(msg.MR_BROKER, int(self.q.getvalue('id')), type, "")

    def Send(self, To, Data=""):
        return msg.Message.SendMessage(int(To), int(self.q.getvalue('id')), MT_DATA, Data)

    def __init__(self, q):
        self.q = q
        self.id = 0
        self.exit = False


    def MsgSend(self):
        self.Send(self.q.getvalue('idTo'), self.q.getvalue('message'))
        print("Content-type: text/plain\n")
        print()

    def MsgGet(self):
        m = self.Request(MT_GETDATA)
        if m.Header.Type != MT_NODATA:
            print("Content-type: text/plain\n")
            print(str(m.Header.Type)+" "+str(m.Header.From) + " " + m.Data)
        if m.Header.Type == MT_LOAD_MESSAGES:
            print(json.dumps(m.data))
        return m
    def initialize(self):
        m = self.Request(msg.MT_INIT)
        self.id = m.Header.To
        print("Content-type: text/plain\n")
        print(self.id)
    def getUserList(self):
        m = self.Request(msg.MT_GET_USERS)
        print("Content-type: text/plain\n")
        print(m.Data)
    def getMessageList(self):
        m = self.Request(msg.MT_LOAD_MESSAGES)
        print("Content-type: text/plain\n")
        print(m.Data)
    def getMessagesByAPI(self, id):
        m = self.Request(msg.MT_LOAD_MESSAGES)
        print("Content-type: text/plain\n")

        #while True:
           # m = self.MsgGet()
           # if m.Header.Type == MT_LOAD_MESSAGES:

           #     break
           # else:
           #     time.sleep(2)

def mainAPI(cmd, method,m):
    if method == "GET":
        data = os.environ['QUERY_STRING']
        m.getMessagesByAPI(int(data.split("=")[1]))

def main():
    q = cgi.FieldStorage()
    m = Messenger(q)
    cmd = os.environ['PATH_INFO']
    if 'api' in cmd:
        mainAPI(cmd, os.environ['REQUEST_METHOD'], m)
        exit()

    MENU = {
        'send': m.MsgSend,
        'get': m.MsgGet,
        'init': m.initialize,
        'getUserList': m.getUserList,
        'load': m.getMessageList
    }

    try:
        MENU[q.getvalue('type')]()
    except Exception as e:
        pass


main()
