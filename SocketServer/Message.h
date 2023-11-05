#pragma once

enum MessageTypes
{
	MT_INIT,
	MT_EXIT,
	MT_GETDATA,
	MT_DATA,
	MT_NODATA,
	MT_CONFIRM,
	MT_GET_USERS,
	MT_NOT_FOUND,
	MT_ADD_USER,
	MT_DELETE_USER,
	MT_STORAGE_INIT,
	MT_LOAD_MESSAGES
};

enum MessageRecipients
{
	MR_STORAGE = -3,
	MR_BROKER = -2,
	MR_ALL = -1,
	MR_USER = 0,
};

struct MessageHeader
{
	int to;
	int from;
	int type;
	int size;
};

class Message
{
public:
	MessageHeader header = {0};
	string data;
	Message() {}
	Message(int to, int from, int type = MT_DATA, const string& data = "")
	{
		this->data = data;
		header = {to, from, type, int(data.length())};
	}
	
	void send(CSocket& s)
	{
		s.Send(&header, sizeof(MessageHeader));
		if (header.size)
		{
			s.Send(data.c_str(), (int)header.size);
		}
	}

	int receive(CSocket& s)
	{
		if (!s.Receive(&header, sizeof(MessageHeader)))
		{
			return MT_NODATA;
		}
		if (header.size)
		{
			vector <char> v(header.size);
			s.Receive(&v[0], (int)header.size);
			data = string(&v[0], header.size);
		}
		return header.type;
	}

	static void send(CSocket& s, int to, int from, int type = MT_DATA, const string& data = "");
	static Message request(int to,int from, int type = MT_DATA, const string& data = "");
	static void send(int to, int from, int type = MT_DATA, const string& data = "");
};

