#pragma once

class Session
{
public:
	int id;
	string name;
	queue<Message> messages;
	time_t lastConnectionTime;

	CCriticalSection cs;
	Session(int _id, string _name)
		:id(_id), name(_name), lastConnectionTime(time(NULL))
	{

	}

	void add(Message& m)
	{
		CSingleLock lock(&cs, TRUE);
		messages.push(m);
		lastConnectionTime = time(NULL);
	}

	void send(CSocket& s)
	{
		CSingleLock lock(&cs, TRUE);
		if (messages.empty())
		{
			Message::send(s, id, MR_BROKER,MT_NODATA);
			if (abs(lastConnectionTime - time(NULL)) > 10) {
				messages.push(Message(id,MR_BROKER,MT_EXIT));
			};
		}
		else
		{
			messages.front().send(s);
			messages.pop();
		}
	}
};

