//---------------------------------------------------------------------------

#pragma hdrstop

#include "Client.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
int Client::AmountOfShowingMessages=20;
//---------------------------------------------------------------------------
void Client::NotifySMLSeCl(const std::string& Message1,const std::string& Message2){
this->IncomingMess(Message1,Message2);
}
//---------------------------------------------------------------------------
void Client::NotifyViewSeCl(int Command,const std::string& Message){
if(Command == 1){//���� � ���
	this->LogIn(Message);
	std::thread NewThread(&Client::JoinServer,this);
	NewThread.detach();
}else if(Command == 2){//������ IP �������
	std::thread NewThread(&Client::SetServerIP,this,Message);
	NewThread.detach();
}else if(Command == 3){//��������� ���������
	std::thread NewThread(&Client::SendMessageToServer,this,this->ServerInfo.IP,Message);
	NewThread.detach();
}else if(Command == 4){//����� �� ����
	std::thread NewThread(&Client::LeaveServer,this);
	NewThread.detach();
}
}
//---------------------------------------------------------------------------
void  Client::SetSendingManager(ISendingManager* SendingManagerPTR){
this->SendingManagerInstance=SendingManagerPTR;
}
//---------------------------------------------------------------------------
void Client::SendMessageToServer(const std::string& IPSendTo,const std::string& Message){
this->SendingManagerInstance->Send(IPSendTo,Message);
this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->ServerInfo.Name,"Message sent");
}
//---------------------------------------------------------------------------
void Client::CheckSendingManagerIsOn(){
if(this->SendingManagerInstance!=NULL){
	if(!this->SendingManagerInstance->GetActive()){
		this->SendingManagerInstance->TurnOn();
	}
}
}
//---------------------------------------------------------------------------
void Client::IncomingMess(std::string const& FromIP,std::string const& Data){
if(this->Logged){
	if(this->SendingManagerInstance != NULL){
		std::thread NewThread(&Client::WorkOnServerMess,this,FromIP,Data);
		NewThread.detach();
	}
	this->CheckSendingManagerIsOn();
}
}
//---------------------------------------------------------------------------
bool Client::LogIn(std::string newName){
if(!this->Logged){
	newName=User::CleanNameString(newName);
	if(newName.size() > 0){
		this->Name=newName;
		this->Logged=true;
		this->ModelMessages.SetIsOn(true);
		return true;
	}
}
return false;
}
//---------------------------------------------------------------------------
void Client::SetServerIP(std::string newIP){
this->ServerInfo.IP=newIP;
this->SendToMediatorViewSeCl(this->ServerInfo.IP,"Server IP changed");
}
//---------------------------------------------------------------------------
void Client::JoinServer(){
if(this->Logged && !this->Online){
	this->SendingManagerInstance->Send(this->ServerInfo.IP,this->MakeRequest_JoinServer());
	this->SendToMediatorViewSeCl(this->ServerInfo.IP,"Join request sent");
}
}
//---------------------------------------------------------------------------
void Client::LeaveServer(){
if(this->Logged && this->Online){
	this->SendingManagerInstance->Send(this->ServerInfo.IP,this->MakeRequest_LeaveServer());
	this->SendingManagerInstance->TurnOff();
	this->Online=false;
	this->Logged=false;
	this->ModelMessages.SetIsOn(false);
	this->SendToMediatorViewSeCl(this->ServerInfo.IP,"You left the chat");
}
}
//---------------------------------------------------------------------------
void Client::WorkOnServerMess(std::string FromIP,std::string Data){
unsigned short int RequestN=0;
std::size_t pos1,pos2;

if((this->ServerInfo.IP != FromIP) || (Data.size() < 1)){
	return;
}
pos1=Data.find("@se!");
if(pos1!=std::string::npos){
	RequestN=std::stoi(Data.substr(pos1+4,1));
}

switch (RequestN) {
case 1: //@se!1N[]			-��� �����
	Data=Message::GetParsedSubString(Data,"N");
	if(Data.size()>0){
		this->ServerInfo.Name=Data;
       	this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->ServerInfo.Name,"You have joined the chat");
	}
break;
case 2://@se!2				-������������� �� �������� ����������
	this->Online=true;
	this->SendingManagerInstance->Send(FromIP,this->MakeRequest_ServerName());
	this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->ServerInfo.Name,"You have joined the chat");
break;
case 3:{//@se!3M[]N[]T[]I[]	-��������� �����(�����, ��� �����������, �����, id)
	std::string MessageText,SenderName,Id,Time;
		MessageText=Message::GetParsedSubString(Data,"M");
		if(MessageText.size()>0){
			SenderName=Message::GetParsedSubString(Data,"N");
			if(Time.size()>0){
				Id=Message::GetParsedSubString(Data,"I");
				if(Id.size()>0){
					Time=Message::GetParsedSubString(Data,"T");
					if(MessageText.size()>0){
						Message NewMessage(MessageText,SenderName,std::stoi(Id),std::stol(Time));
						this->ModelMessages.AddData(NewMessage);
						this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->ServerInfo.Name,"You have joined the chat");
					}
				}
			}
		}
}
break;
case 4://@se!4				-����������� � �������� �������
	this->Online=false;
	this->ModelMessages.DeleteAllData();
break;
default:
break;
}
}
//---------------------------------------------------------------------------
bool Client::GetOnline(){
return this->Online;
}
//---------------------------------------------------------------------------
bool Client::GetLogged(){
return this->Logged;
}
//---------------------------------------------------------------------------
std::string Client::GetName(){
return this->Name;
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_ServerName(){
return "@cl!1";
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_JoinServer(){
return "@cl!2N["+this->Name+"]";
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_GetAllMess(){
return "@cl!3";
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_GetLastXMess(int X){
return "@cl!4X["+std::to_string(X)+"]";
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_SendMess(Message newMessage){
return "@cl!5M["+newMessage.GetText()+"]";
}
//---------------------------------------------------------------------------
std::string Client::MakeRequest_LeaveServer(){
return "@cl!6";
}
//---------------------------------------------------------------------------
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
