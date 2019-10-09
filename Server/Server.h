//---------------------------------------------------------------------------
#ifndef ServerH
#define ServerH

#include "Models.h"
#include "Users.h"
#include "Messages.h"
#include "Mediator.h"
#include "SendingManagerLinux.h"
#include <string>
#include <thread>
//---------------------------------------------------------------------------
class Server:public ConcreteColleagueSeCl, public ConcreteColleagueStoV{
private:
	static int AmountOfShowingMessages;

	Server()=default;
	Server(const Server&)=delete;
	Server& operator=(Server&)= delete;

	ISendingManager* SendingManagerInstance=NULL;
	bool IsOn=false;
	std::string Name="@Server6106";

	Model<User> ModelUsers;
	Model<MessageSe> ModelMessages;

	void CheckSendingManagerIsOn();
	void WorkOnRequest(std::string FromIP,std::string Data);

	unsigned short int Request_SendServerInfo(std::string& SendToIP);
	unsigned short int Request_AddUserToChat(User& newUser);
	unsigned short int Request_SendAllMessages(std::string& SendToIP);
	unsigned short int Request_SendLastXMessages(std::string& SendToIP,int X);
	unsigned short int Request_AddMessageToChat(MessageSe& newMessage);
	unsigned short int Request_DeleteUser(User& newUser);

	unsigned short int SendMessagesToOne(std::string& SendToIP,std::set<MessageSe>& Data);
	unsigned short int SendMessageToAll(MessageSe& newMessage);

	std::string CreateMess3(MessageSe& newMessage);

	void NotifySMLSeCl(std::string const& Message1, std::string const& Message2)override;
	void NotifyViewSeCl(int Command)override;

public:
	static Server& getInstance(){
		static Server instance;
		return instance;
	}

	void SetSendingManager(ISendingManager* SendingManagerPTR);
	bool TurnOn();
	void TurnOff();
	bool GetIsOn();
	void SetName(std::string newName);
	std::string GetName();
	void Request(std::string const& FromIP,std::string const& Data);
};
//---------------------------------------------------------------------------
#endif
