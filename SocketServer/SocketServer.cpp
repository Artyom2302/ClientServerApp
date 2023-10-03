// SocketServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "framework.h"
#include "SocketServer.h"
#include "Message.h"
#include "Session.h"
#include "Server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void LaunchClient()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	CreateProcess(NULL, (LPSTR)"SocketClient.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}



void ProcessClient(SOCKET hSock,Server& server)
{
	CSingleLock lock(&server.cs, TRUE);
	CSocket s;
	s.Attach(hSock);
	Message m;
	int code = m.receive(s);
	switch (code)
	{
	case MT_INIT:
	{
		cout << "Client#" << ++server.maxID-100 << " connect server" << endl;
		auto session = make_shared<Session>(server.maxID, m.data);
		server.sessions[session->id] = session;
		Message::send(s, session->id, MR_BROKER, MT_INIT);
		
		break;
	}
	case MT_EXIT:
	{
		server.sessions.erase(m.header.from);
		cout << "Client#"<<m.header.from-100<<" disconnect server"<<endl;
		break;
	}
	case MT_GETDATA:
	{
		auto iSession = server.sessions.find(m.header.from);
		if (iSession != server.sessions.end())
		{
			iSession->second->send(s);
		}
		break;
	}
	case MT_GET_USERS: {
		auto iSession = server.sessions.find(m.header.from);
		cout << "Client#" << m.header.from - 100 << " request a list of users" << endl;
		string str = "All connected clients(id numbers): ";
		for (auto const& [key, val] : server.sessions) {
			if (key !=m.header.from)
			str.append("#" + to_string(key - 100) + " ");
		}
		Message mes = Message(m.header.from, MR_BROKER,MT_GET_USERS, str.c_str());
		iSession->second->add(mes);
		break;
	}
	default:
	{
		auto iSessionFrom = server.sessions.find(m.header.from);
		if (iSessionFrom != server.sessions.end())
		{
			auto iSessionTo = server.sessions.find(m.header.to);
			if (iSessionTo != server.sessions.end())
			{
				iSessionTo->second->add(m);
				cout << "User #" << m.header.from-100<<" send to User #"<<m.header.to-100<< " message("<<m.data<<") "<<endl;
			}
			else if (m.header.to == MR_ALL)
			{
				for (auto& [id, session] : server.sessions)
				{
					if (id != m.header.from)
						session->add(m);
				}
				cout << "User #" << m.header.from-100 << " send " << m.header.to-100<< " message("<<m.data<<") to all " << endl;
			}
			else
			{
				iSessionFrom->second->add(Message(m.header.from, m.header.to, MT_NOT_FOUND));
				cout << "User #" << m.header.from << "send message for non-existent user "<<endl;
			}
		}
		
		break;
	}
	}
}

void ServerStart()
{
	AfxSocketInit();

	CSocket ServerSocket;
	ServerSocket.Create(12345);
	Server server;
	for (int i = 0; i < 1; ++i)
	{
		LaunchClient();
	}

	while (true)
	{
		if (!ServerSocket.Listen())
			break;
		CSocket s;
		ServerSocket.Accept(s);
		thread t(ProcessClient, s.Detach(),ref(server));
		t.detach();
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
			ServerStart();
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
