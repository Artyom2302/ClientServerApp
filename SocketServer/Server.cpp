#include "pch.h"
#include "Server.h"
#include <unordered_set>


void Server::ProcessClient(SOCKET hSock)
{

	CSocket s;
	s.Attach(hSock);

	Message m;
	int code = m.receive(s);
	cout << m.header.from << " : " << m.header.type << endl;
	switch (code)
	{
	case MT_INIT:
	{
		int id = ++maxID;
		cout << "Client#" << id << " connect server" << endl;
		auto session = make_shared<Session>(id, m.data);
		sessions[session->id] = session;
		Message::send(s, session->id, MR_BROKER, MT_INIT);
		AddUserToList(id);
		break;
	}
	case MT_STORAGE_INIT: {
		cout << "Storage connected"<< endl;
		auto session = make_shared<Session>(MR_STORAGE, m.data);
		sessions[MR_STORAGE] = session;
		Message::send(s,MR_STORAGE, MR_BROKER, MT_INIT);
		break;
	}
	case MT_EXIT:
	{
		DeleteUserFromList(m.header.from);
		sessions.erase(m.header.from);
		cout << "Client#" << m.header.from << " disconnect server" << endl;
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
			cout << "Client#" << m.header.from << " request a list of users" << endl;
			string str = "";
			for (auto const& [key, val] : sessions) {
				if (key != m.header.from && key != MR_STORAGE)
					str.append(to_string(key )+" ");
			}
			Message(m.header.from, MR_BROKER, MT_GET_USERS, str.c_str()).send(s);	
		}
		break;
	}
	case MT_LOAD_MESSAGES: {
		if (m.header.from == MR_STORAGE) {
			auto iSession = sessions.find(m.header.to);
			iSession->second->add(m);
		}
		else {
			auto storageSession = sessions.find(MR_STORAGE);
			if (storageSession != sessions.end()) {
				m.send(s);
				storageSession->second->add(m);
			}
		};
		
		break;
	}
	default:
	{
		auto storageSession = sessions.find(MR_STORAGE);
		auto iSessionFrom = sessions.find(m.header.from);
		if (iSessionFrom != sessions.end())
		{
			auto iSessionTo = sessions.find(m.header.to);
			if (iSessionTo != sessions.end())
			{
				iSessionTo->second->add(m);
				cout << "User #" << m.header.from<< " send to User #" << m.header.to << " message(" << m.data << ") " << endl;
				if (storageSession != sessions.end() && m.header.from!= storageSession->first)
					storageSession->second->add(m);
			}
			else if (m.header.to == MR_ALL)
			{
				for (auto& [id, session] : sessions)
				{
					if (id != m.header.from)
						session->add(m);
				}
				cout << "User #" << m.header.from << " send " << " message(" << m.data << ") to all " << endl;
			}
			else
			{
				iSessionFrom->second->add(Message(m.header.from, m.header.to, MT_NOT_FOUND));
				cout << "User #" << m.header.from << "send message for non-existent user " <<m.header.to<< endl;
			}
		}
		break;
	}
	}
	
}

void Server::CheckTimeOut()
{
	while (true)
	{
		unordered_set<int> keysForDelete;
		if (sessions.size()) {
			for (auto const& [key, val] : sessions) {
				auto timeout = abs(val->lastConnectionTime - time(NULL)) ;
				if (timeout > 50) {
					keysForDelete.insert(key);
				}
			};
			for (auto  key : keysForDelete) {
				cout << "Client #" << key << " lose connection" << endl;
				DeleteUserFromList(key);
				sessions.erase(key);
			}
		}
		Sleep(500);
	}
	
}

void Server::AddUserToList(int userId)
{
	for (auto const& [key, val] : sessions) {
		if (key != userId) {
			Message m = Message(key, MR_BROKER, MT_ADD_USER, to_string(userId));
			val->add(m);
		};
	};
}

void Server::DeleteUserFromList(int userId)
{
	for (auto const& [key, val] : sessions) {
		
		if (key != userId) {
			Message m = Message(key, MR_BROKER, MT_DELETE_USER, to_string(userId));
			val->add(m);
		}	
	};
}


void Server::Start()
{
	AfxSocketInit();
	ServerSocket.Create(12345);
	thread timeout(&Server::CheckTimeOut, this);
	timeout.detach();
	while (true)
	{
		if (!ServerSocket.Listen())
			break;
		CSocket s;
		ServerSocket.Accept(s);
		thread t(&Server::ProcessClient,this, s.Detach());
		t.detach();
	}
}

