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
};

