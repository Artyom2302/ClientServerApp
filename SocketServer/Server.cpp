#include "pch.h"
#include "Server.h"
#include <set>

void Server::ProcessClient(SOCKET hSock)
{

	CSocket s;
	s.Attach(hSock);

	Message m;
	int code = m.receive(s);
	switch (code)
	{
	case MT_INIT:
	{

		int id = ++maxID;
		cout << "Client#" << id  - 100 << " connect server" << endl;
		auto session = make_shared<Session>(id, m.data);
		sessions[session->id] = session;
		Message::send(s, session->id, MR_BROKER, MT_INIT);
		break;
	}
	case MT_EXIT:
	{
		sessions.erase(m.header.from);
		cout << "Client#" << m.header.from - 100 << " disconnect server" << endl;
		break;
	}
	case MT_GETDATA:
	{
		
		auto iSession = sessions.find(m.header.from);
		if (iSession != sessions.end())
		{
			iSession->second->lastConnectionTime = time(NULL);
			iSession->second->send(s);
		}
		break;
	}
	case MT_GET_USERS: {
		auto iSession = sessions.find(m.header.from); 
		if (iSession != sessions.end()) {
			cout << "Client#" << m.header.from - 100 << " request a list of users" << endl;
			string str = "All connected clients(id numbers): ";
			for (auto const& [key, val] : sessions) {
				if (key != m.header.from)
					str.append("#" + to_string(key - 100) + " ");
			}
			Message mes = Message(m.header.from, MR_BROKER, MT_GET_USERS, str.c_str());
			iSession->second->add(mes);
		}
		break;
	}
	default:
	{
		auto iSessionFrom = sessions.find(m.header.from);
		if (iSessionFrom != sessions.end())
		{
			auto iSessionTo = sessions.find(m.header.to);
			if (iSessionTo != sessions.end())
			{
				iSessionTo->second->add(m);
				cout << "User #" << m.header.from - 100 << " send to User #" << m.header.to - 100 << " message(" << m.data << ") " << endl;
			}
			else if (m.header.to == MR_ALL)
			{
				for (auto& [id, session] : sessions)
				{
					if (id != m.header.from)
						session->add(m);
				}
				cout << "User #" << m.header.from - 100 << " send " << m.header.to - 100 << " message(" << m.data << ") to all " << endl;
			}
			else
			{
				iSessionFrom->second->add(Message(m.header.from, m.header.to, MT_NOT_FOUND));
				cout << "User #" << m.header.from << "send message for non-existent user " << endl;
			}
		}
		break;
	}
	}
	
}

void Server::CheckTimeOut()
{
	set<int> keysForDelete;

		if (sessions.size()) {
			for (auto const& [key, val] : sessions) {
				if (abs(val->lastConnectionTime - time(NULL)) > 10) {
					keysForDelete.insert(key);
				}
			};
			for (auto const& key : keysForDelete) {
				sessions.erase(key);
			}
		}
}

void Server::Start()
{
	AfxSocketInit();
	ServerSocket.Create(12345);
	for (int i = 0; i < 4; ++i)
	{
		LaunchClient();
	}
	while (true)
	{
		if (!ServerSocket.Listen())
			break;
		CSocket s;
		ServerSocket.Accept(s);
		thread t(&Server::ProcessClient,this, s.Detach());
		t.detach();
		this->CheckTimeOut();
	}
}

