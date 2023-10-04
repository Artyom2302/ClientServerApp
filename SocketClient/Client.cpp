#include "pch.h"
#include "Client.h"

void Client::ProcessMessages()
{
	exit = false;
	while (!exit)
	{
		Message m = Request(MT_GETDATA);
		switch (m.header.type)
		{
		case MT_DATA:
			cout << endl << endl << "New message: " << m.data << endl;
			break;
		case MT_NOT_FOUND:
			cout << endl << endl << "Client #" << m.header.from - 100 << " not found\n";
			break;
		case MT_CONFIRM: {
			break;
		}
		case MT_GET_USERS: {
			cout << endl << m.data << endl << endl;
			break;
		}
		case MT_NODATA: {
			Sleep(500);
			break;
		}
		default:
			Sleep(500);
			break;
		}
		if (m.header.type == MT_DATA || m.header.type == MT_NOT_FOUND || m.header.type == MT_GET_USERS) {
			printMenu();
		}
	}
}

void Client::Start()
{
	AfxSocketInit();
	boolean exitServer = false;
	this->Initialize();
	
	thread t(&Client::ProcessMessages,this);
	t.detach();
	cout << "Your id: " << id-100<< endl;
	while (!(exitServer))
	{
		exitServer = MenuHandler();
	}
}

boolean Client::MenuHandler()
{
		printMenu();
		int choice = EnterInt(1, 3);
		boolean exitApp = false;
		switch (choice)
		{
		case 1:
			MessageHandler();
			break;
		case 2:
			GetUserList();
			break;
		case 3:
			Message::send(MR_BROKER, MT_EXIT);
			exitApp = true;
			break;
		default:
			break;
		}
		if (exitApp)
			return true;
	
}

void Client::Initialize()
{
	Message m = Request(MT_INIT);
	this->id = m.header.to;
}

Message Client::Request(int type)
{
	return Message::request(MR_BROKER, this->id, type, "");
}

void Client::Send(int to, int type, const string& data)
{
	Message::send(to, this->id, type, data);
}


int Client::EnterInt(int downBorder, int upBorder)
{
	int value;
	cin >> value;
	while (cin.fail() || value < downBorder || value > upBorder) {
		cin.clear();
		cin.ignore(32767, '\n');
		cout << "Enter again:" << endl;
		cin >> value;
	};
	return value;
}

void Client::MessageHandler()
{
		cout << "Enter client Id address(0 to send this message for all):" << endl;
		int addr = this->id;
		while (true) {
			addr = EnterInt();
			if (addr+100 == this->id) {
				cout << "Can't send message for yourself"<<endl;
			}
			else {
				break;
			}
		}
		cout << endl << "Enter msg: " << endl;
		string str;
		cin >> str;
		Send(addr == 0 ? MR_ALL : addr + 100, MT_DATA, str);
		return;
}

void Client::GetUserList()
{
	Message m = Request(MT_GET_USERS);
	
}

void Client::printMenu()
{
		cout << "Enter: " << endl;
		cout << "1)Send Message to users" << endl;
		cout << "2)List of users" << endl;
		cout << "3)Quit" << endl;
}