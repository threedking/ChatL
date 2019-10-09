//---------------------------------------------------------------------------
#pragma hdrstop
#include "Server.h"
#include <set>
//---------------------------------------------------------------------------
#pragma package(smart_init)
int Server::AmountOfShowingMessages=20;
//---------------------------------------------------------------------------
void Server::SetSendingManager(ISendingManager* SendingManagerPTR){
this->SendingManagerInstance=SendingManagerPTR;
}
//---------------------------------------------------------------------------
bool Server::TurnOn(){
if(!this->IsOn){
	this->IsOn=this->SendingManagerInstance->TurnOn();
	this->ModelMessages.SetIsOn(this->IsOn);
	this->ModelUsers.SetIsOn(this->IsOn);	

	MessageSe newMessage("Chat is open",this->Name);
	this->Request_AddMessageToChat(newMessage);
}
this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->Name,"Server activated");
return this->IsOn; 
}
//---------------------------------------------------------------------------
void Server::TurnOff(){ 
if(this->IsOn){
MessageSe newMessage("Chat is closed",this->Name);
this->Request_AddMessageToChat(newMessage);
this->SendingManagerInstance->TurnOff();
this->IsOn=false;
this->ModelUsers.SetIsOn(false);
this->ModelMessages.SetIsOn(false);
}
this->SendToMediatorViewSeCl(this->ModelMessages.GetFewOfLastData(this->AmountOfShowingMessages),this->Name,"Server deactivated");
}
//---------------------------------------------------------------------------
bool Server::GetIsOn(){
return this->IsOn;
}
//---------------------------------------------------------------------------
void Server::SetName(std::string newName){
if((newName.size() > 0) && (newName.size() <= User::MAXNameLength)){
	this->Name=newName;
}
}
//---------------------------------------------------------------------------
std::string Server::GetName(){
return this->Name;
}
//---------------------------------------------------------------------------
void Server::NotifyViewSeCl(int Command){
switch (Command) {
case 1:{//Запуск сервера
	std::thread NewThread(&Server::TurnOn,this);
	NewThread.detach();
}
break;
case 2:
case 3:{//Остановка сервера
	std::thread NewThread(&Server::TurnOff,this);
	NewThread.detach();
}
break;
default:
break;
}
}
//---------------------------------------------------------------------------
void Server::CheckSendingManagerIsOn(){
if(this->SendingManagerInstance!=NULL){
	if(!this->SendingManagerInstance->GetActive()){
		this->SendingManagerInstance->TurnOn();
	}
}
}
//---------------------------------------------------------------------------
void Server::NotifySMLSeCl(std::string const& Message1, std::string const& Message2){
this->Request(Message1,Message2);
}
//---------------------------------------------------------------------------
void Server::Request(std::string const& FromIP,std::string const& Data){
if(this->IsOn){
	if(this->SendingManagerInstance != NULL){
		std::thread NewThread(&Server::WorkOnRequest,this,FromIP,Data);
		NewThread.detach();
	}
	this->CheckSendingManagerIsOn();
}
}
//---------------------------------------------------------------------------
void Server::WorkOnRequest(std::string FromIP,std::string Data){
unsigned short int RequestN=0,Result=0;
std::size_t pos1,pos2;

if(Data.size()<1){
	return;
}
pos1=Data.find("@cl!");
if(pos1!=std::string::npos){
	//RequestN=StrToInt(Data[pos1+4]);
	RequestN=std::stoi(Data.substr(pos1+4,1));
}

switch (RequestN) {
case 1:		//@cl!1			-Получение запроса имени серва
	Result=this->Request_SendServerInfo(FromIP);
break;
case 2:     //@cl!2N[]		-Получение запроса на добавление пользователя(добавление в бд)
	Data=Message::GetParsedSubString(Data,"N");
	if(Data.size()>0){
		User Sender(FromIP,Data);
		Result=this->Request_AddUserToChat(Sender);

		MessageSe newMessage("User "+Sender.GetName()+" join the chat",this->Name);
		this->Request_AddMessageToChat(newMessage);
	}
break;
case 3:     //@cl!3			-Получение запроса на отправку всех сообщений
	Result=this->Request_SendAllMessages(FromIP);
break;
case 4:     //@cl!4X[]		-Получение запроса на отправку последних X сообщений
	Data=Message::GetParsedSubString(Data,"X");
	if(Data.size()>0){
		Result=this->Request_SendLastXMessages(FromIP,std::stoi(Data));
	}
break;
case 5:{ 	//@cl!5M[]		-Получение нового сообщения(добавление в модель+рассылка всем)
	User UserS(FromIP);
	User UserF(this->ModelUsers.FindAndGetOneData(UserS));
	if(UserS.GetName() != UserF.GetName()){//Если вернул не стандартный, значит нашел
		Data=Message::GetParsedSubString(Data,"M");
		if(Data.size()>0){
			MessageSe newMessage(Data,UserF.GetName());
			Result=this->Request_AddMessageToChat(newMessage);
		}
	}
}
break;
case 6:{    //@cl!6			-Получение уведомления о выходе из чата(удалить пользователя в моделе)
	User UserS(FromIP);
	User UserF(this->ModelUsers.FindAndGetOneData(UserS));
	if(UserS.GetName() != UserF.GetName()){//Если вернул не стандартный, значит нашел
		Result=this->Request_DeleteUser(UserF);

		MessageSe newMessage("User "+UserF.GetName()+" left the chat",this->Name);
		this->Request_AddMessageToChat(newMessage);
	}
}
break;
default:
break;
}//TODO обработать Result вьюшкой
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_SendServerInfo(std::string & SendToIP){
return this->SendingManagerInstance->Send(SendToIP,"@se!1N["+this->Name+"]");
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_AddUserToChat(User& newUser){
this->ModelUsers.AddData(newUser);
return this->SendingManagerInstance->Send((newUser.GetIP()).c_str(),"@se!2");
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_SendAllMessages(std::string & SendToIP){
std::set<MessageSe> Data=this->ModelMessages.GetAllData();
return this->SendMessagesToOne(SendToIP,Data);
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_SendLastXMessages(std::string & SendToIP,int X){
std::set<MessageSe> Data=this->ModelMessages.GetFewOfLastData(X);
return this->SendMessagesToOne(SendToIP,Data);
}
//---------------------------------------------------------------------------
unsigned short int Server::SendMessagesToOne(std::string& SendToIP,std::set<MessageSe>& Data){
for(MessageSe it : Data){
	return this->SendingManagerInstance->Send(SendToIP,(this->CreateMess3(it)).c_str());
}
return 0;
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_AddMessageToChat(MessageSe& newMessage){
this->ModelMessages.AddData(newMessage);
return this->SendMessageToAll(newMessage);
}
//---------------------------------------------------------------------------
unsigned short int Server::SendMessageToAll(MessageSe& newMessage){
for(auto &it : this->ModelUsers.GetAllData()){
	return this->SendingManagerInstance->Send((it.GetIP()).c_str(),(this->CreateMess3(newMessage)).c_str());
}
return 0;
}
//---------------------------------------------------------------------------
std::string Server::CreateMess3(MessageSe& newMessage){//@se!3M[]N[]T[]I[]
return ("@se!3M["+newMessage.GetText()+
"]N["+newMessage.GetSenderName()+
"]T["+std::to_string(newMessage.GetTime())+
"]I["+std::to_string(newMessage.GetID())+
"]");
}
//---------------------------------------------------------------------------
unsigned short int Server::Request_DeleteUser(User& newUser){
this->ModelUsers.DeleteOne(newUser);
return 0;
}
//---------------------------------------------------------------------------
/*
От клиента
@cl!1				-Получение запроса имени серва
@cl!2N[]			-Получение запроса на добавление пользователя(добавление в бд)
@cl!3				-Получение запроса на отправку всех сообщений
@cl!4X[]			-Получение запроса на отправку последних X сообщений
@cl!5M[]			-Получение нового сообщения(добавление в модель+рассылка всем)
@cl!6				-Получение уведомления о выходе из чата(удалить пользователя в моделе)

От сервера
@se!1N[]			-Отправка имени серва конкретному пользователю
@se!2				-Отправка подтверждения об успешном добавлении(в любом случае. банов нет) конкретному пользователю
@se!3M[]N[]T[]I[]	-Отправка сообщения пользователю(текст, имя отправителя, время, id)
@se!4				-Отправка уведомления о закрытии сервера

Самое длинное сообщение @se!3M[]N[]T[]I[] - 17+data
data - M[150]+N[15]+T[10]+I[10]=185
Всего - 185+17 = 202
*/
