#pragma once
#include "Message.h"
#include "Session.h"
#include "framework.h"

class Server
{
public:
	int maxID = MR_USER-1;
	map<int, shared_ptr<Session>> sessions;
	CCriticalSection cs;
	CSocket ServerSocket;
	void ProcessClient(SOCKET hSock);
	void Start();
	void LaunchClient()
	{
		/*STARTUPINFO si = { sizeof(si) };
		STARTUPINFO si1 = { sizeof(si1) };
		PROCESS_INFORMATION pi,pi1;
		CreateProcess(NULL, (LPSTR)"SocketClient.exe", NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CreateProcess(NULL, (LPSTR)"Debug/net7.0-windows/SharpSocketClient.exe", NULL, NULL, TRUE, NULL, NULL, NULL, &si1, &pi1);
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(pi1.hThread);
		CloseHandle(pi1.hProcess);*/
	}
	void CheckTimeOut();
	void AddUserToList(int userId);
	void DeleteUserFromList(int userId);

};

