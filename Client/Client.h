//---------------------------------------------------------------------------

#ifndef ClientH
#define ClientH

#include "Users.h"
#include "Models.h"
#include "Mediator.h"
#include "SendingManagerLinux.h"
#include <string>
#include <thread>
//---------------------------------------------------------------------------
class Client:public ConcreteColleagueSeCl, public ConcreteColleagueCtoV{
private:
	static int AmountOfShowingMessages;

	Client()=default;
	Client(const Client&)=delete;
	Client& operator=(Client&)= delete;

	Model<Message> ModelMessages;
	ISendingManager* SendingManagerInstance=NULL;

	bool Online=false;
	bool Logged=false;
	std::string Name="@Anonymous";

	struct typeServerInfo{
		std::string Name="";
		std::string IP="127.0.0.1";
	};
	typeServerInfo ServerInfo;

	void CheckSendingManagerIsOn();
	void WorkOnServerMess(std::string FromIP,std::string Data);

	std::string MakeRequest_ServerName();
	std::string MakeRequest_JoinServer();
	std::string MakeRequest_GetAllMess();
	std::string MakeRequest_GetLastXMess(int X);
	std::string MakeRequest_SendMess(Message newMessage);
	std::string MakeRequest_LeaveServer();

	void SendMessageToServer(const std::string& ToIP,const std::string& Message);

	void NotifySMLSeCl(const std::string& Message1,const std::string& Message2)override;
	void NotifyViewSeCl(int Command,const std::string& Message)override;
public:
	static Client& getInstance(){
		static Client instance;
		return instance;
	}
	void SetSendingManager(ISendingManager* SendingManagerPTR);

	bool LogIn(std::string newName);
	void SetServerIP(std::string newIP);
	void JoinServer();
	void LeaveServer();

	void IncomingMess(std::string const& FromIP,std::string const& Data);

	bool GetOnline();
	bool GetLogged();
	std::string GetName();
};
//---------------------------------------------------------------------------
#endif
/*
�� �������
@cl!1				-��������� ������� ����� �����
@cl!2N[]			-��������� ������� �� ���������� ������������(���������� � ��)
@cl!3				-��������� ������� �� �������� ���� ���������
@cl!4X[]			-��������� ������� �� �������� ��������� X ���������
@cl!5M[]			-��������� ������ ���������(���������� � ������+�������� ����)
@cl!6				-��������� ����������� � ������ �� ����(������� ������������ � ������)

�� �������
@se!1N[]			-�������� ����� ����� ����������� ������������
@se!2				-�������� ������������� �� �������� ����������(� ����� ������. ����� ���) ����������� ������������
@se!3M[]N[]T[]I[]	-�������� ��������� ������������(�����, ��� �����������, �����, id)
@se!4				-�������� ����������� � �������� �������

����� ������� ��������� @se!3M[]N[]T[]I[] - 17+data
data - M[150]+N[15]+T[10]+I[10]=185
����� - 185+17 = 202
*/
