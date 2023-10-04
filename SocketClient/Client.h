#pragma once
#include "../SocketServer/Message.h"

class Client
{
	private:
	int id=0;
	bool exit=false;

	static void printMenu();
	static int EnterInt(int downBorder = 0, int upBorder = 1000);

	boolean MenuHandler();
	void MessageHandler();
	void GetUserList();
	void ProcessMessages();

	void Initialize();
	Message Request(int type);
	void Send(int to,int type = MT_DATA,const string& data="");\

public:
	void Start();

};

