#pragma once
#include "../SocketServer/Message.h"

class Client
{
	int id;

	static void MessageHandler();
	static void GetUserList();
	static void printMenu();
	static int EnterInt(int downBorder = 0, int upBorder = 1000);
	static void ProcessMessages(boolean& exit);
	static boolean MenuHandler();
	void Initialize();
public:
	void Start();

};

