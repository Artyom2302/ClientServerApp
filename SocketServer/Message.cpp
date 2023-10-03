#include "pch.h"
#include "Message.h"

string GetLastErrorString(DWORD ErrorID = 0)
{
	if (!ErrorID)
		ErrorID = GetLastError();
	if (!ErrorID)
		return string();

	LPSTR pBuff = nullptr;
	size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ErrorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pBuff, 0, NULL);
	string s(pBuff, size);
	LocalFree(pBuff);
	return s;
}


void Message::send(CSocket& s, int to, int from, int type, const string& data)
{
	Message m(to, from, type, data);
	m.send(s);
}

Message Message::send(int to,int from, int type, const string& data)
{
	CSocket s;
	s.Create();
	while (!s.Connect("127.0.0.1", 12345)) {
		cout << "Не удалось подключиться"<<endl;
	}
	
	Message m(to, from, type, data);
	m.send(s);
	m.receive(s);
	return m;
}

