//---------------------------------------------------------------------------
#ifndef SendingManagerLinuxH
#define SendingManagerLinuxH
#include "Mediator.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <thread>
//---------------------------------------------------------------------------
class ISendingManager{
public:
	virtual ~ISendingManager()= default;

	virtual unsigned short int Send(std::string IPSendTo,std::string Message)=0;
	virtual bool TurnOn()=0;
	virtual void TurnOff()=0;
	virtual bool GetActive()=0;
};
//---------------------------------------------------------------------------
class SendingManagerL:public ISendingManager, public ConcreteColleagueSML{
private:
	SendingManagerL()= default;
	SendingManagerL(const SendingManagerL&)= delete;
	SendingManagerL& operator=(SendingManagerL&)= delete;

	bool IsOn=false;
	std::thread Thread;

	void ReadingLoop();
public:
	static SendingManagerL& getInstance(){
		static SendingManagerL instance;
		return instance;
	}
	~SendingManagerL() override;

	unsigned short int Send(std::string IPSendTo,std::string Message) override;
	bool TurnOn() override;
	void TurnOff() override;
	bool GetActive() override;
};
//---------------------------------------------------------------------------
#endif
