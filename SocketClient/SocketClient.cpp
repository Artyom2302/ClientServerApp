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
	while (cin.fail() && value >= downBorder && value <= upBorder) {
		cin.clear();
		cin.ignore('\n');
		cout << "Enter again:" << endl;
		cin >> value;
	};
	return value;
}

void MenuHandler() {
	cout << "Enter client Id address(-1 to stop chat):"<<endl;
	int adr = EnterInt(-1);
	if (adr == -1) {
		Message::send(MR_BROKER, MT_EXIT);
		cout << "Exit";
		Sleep(2000);
		exit(0);
	}
	cout << endl << "Enter msg: " << endl;
	string str;
	cin >> str;
	Message::send(adr == 0 ? MR_ALL : adr + 100, MT_DATA, str);
}

void ProcessMessages()
{
	while (true)
	{
		Message m = Message::send(MR_BROKER, MT_GETDATA);
		switch (m.header.type)
		{
		case MT_DATA:
			cout <<endl<<"New message: "<< m.data << endl;
			MenuHandler();
			break;
		case MT_NOT_FOUND:
			cout << "Client not found\n";
			break;
		case MT_CONFIRM:{
			break;
		}
		case MT_NODATA: {
			Sleep(500);
			break;
		}
		case MT_EXIT:{
			cout << "exit"<<endl;
			Message::send(MR_BROKER, MT_EXIT);
			exit(0);
			break;
		}
		default:
			Sleep(500);
			break;
		}
	}
}




void Client()
{
	AfxSocketInit();
	thread t(ProcessMessages);
	t.detach();

	Message m = Message::send(MR_BROKER, MT_INIT);

	while (true)
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
