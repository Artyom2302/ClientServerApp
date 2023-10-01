// SocketClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "../SocketServer/Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



int EnterInt(int downBorder = 0, int upBorder = 1000) {
	int value;
	cin >> value;
	while (cin.fail() || value < downBorder || value > upBorder) {
		cin.clear();
		cin.ignore(32767,'\n');
		cout << "Enter again:" << endl;
		cin >> value;
	};
	return value;
}

void MessageHandler() {
	cout << "Enter client Id address(0 to send this message for all):" << endl;
	int adr = EnterInt();
	cout << endl << "Enter msg: " << endl;
	string str;
	cin >> str;
	Message::send(adr == 0 ? MR_ALL : adr + 100, MT_DATA, str);
	return;
}
void GetUserList() {
	Message m = Message::send(MR_BROKER, MT_GET_USERS);
}
boolean MenuHandler() {
	cout << "Enter: " << endl;
	cout << "1)Send Message to users" << endl;
	cout << "2)List of users" << endl;
	cout << "3)Quit" << endl;
	int choice = EnterInt(1,3);
	boolean exitApp = false;

	switch (choice)
	{
	case 1 :
		MessageHandler();
		break;
	case 2:
		GetUserList();
		break;
	case 3:
		Message::send(MR_BROKER, MT_EXIT);
		cout << "Exit app" << endl;
		Sleep(2000);
		exitApp = true;
		break;
	default:
		break;
	}
	if (exitApp)
		return true;
}



void ProcessMessages(boolean& exit)
{
	exit = false;
	while (!exit)
	{
		Message m = Message::send(MR_BROKER, MT_GETDATA);
		switch (m.header.type)
		{
		case MT_DATA:
			cout <<endl<<endl<<"New message: "<< m.data << endl;
			break;
		case MT_NOT_FOUND:
			cout <<endl<<endl<< "Client #"<<m.header.from-100<<" not found\n";
			break;
		case MT_CONFIRM:{
			break;
		}
		case MT_GET_USERS:{
			cout << m.data<<endl;
			break;
		}
		case MT_NODATA: {
			Sleep(500);
			break;
		}
		case MT_EXIT:{
			cout << "Disconnect..." << endl;
			Sleep(1000);
			cout << "Enter -1 to close app"<<endl;
			Message::send(MR_BROKER, MT_EXIT);
			exit = true;
			break;
		}
		default:
			Sleep(500); 
			break;
		}
		if (m.header.type == MT_DATA || m.header.type == MT_NOT_FOUND) {
			cout << "Enter client Id address(-1 to stop chat or 0 to send this message for all):" << endl;
		}
	}
}




void Client()
{
	boolean exitServer = false,exitClient = false;
	AfxSocketInit();
	thread t(ProcessMessages, std::ref(exitServer));
	t.detach();

	Message m = Message::send(MR_BROKER, MT_INIT);
	cout <<"Your id: " << m.clientID-100 << endl;
	while (!(exitServer || exitClient))
	{
		MenuHandler();
	}
}

CWinApp theApp;

int main()
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);

	if (hModule != nullptr)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: code your application's behavior here.
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			Client();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}

	return nRetCode;
}
