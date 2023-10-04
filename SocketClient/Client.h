#pragma once
#include "../SocketServer/Message.h"

class Client
{
	int id;
	bool exit;


	void MessageHandler();
	void GetUserList();
	static void printMenu();
	static int EnterInt(int downBorder = 0, int upBorder = 1000);
	void ProcessMessages();
	boolean MenuHandler();
	void Initialize();
	Message Request(int type);
	void Send(int to,int type = MT_DATA,const string& data="");
public:
	void Start();

};

