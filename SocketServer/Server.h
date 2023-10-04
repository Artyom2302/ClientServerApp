#pragma once
#include "Message.h"
#include "Session.h"
#include "framework.h"

class Server
{
public:
	int maxID = MR_USER;
	map<int, shared_ptr<Session>> sessions;
	CCriticalSection cs;
	CSocket ServerSocket;
	void ProcessClient(SOCKET hSock);
	void Start();
	void LaunchClient()
	{
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		CreateProcess(NULL, (LPSTR)"SocketClient.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

};

