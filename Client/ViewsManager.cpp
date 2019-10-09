//---------------------------------------------------------------------------

#pragma hdrstop

#include "ViewsManager.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void ViewsManager::NotifyViewSeCl(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2){
//Серверное прил. Отрисовать главное окно(set<сообщения> Data, имя сервера, std::string информация)
std::thread NewThread(&ViewsManager::ViewSeMainScr,this,Data,Message1,Message2);
NewThread.detach();
}
//---------------------------------------------------------------------------
void ViewsManager::NotifyViewSeCl(const std::string& Message1,const std::string& Message2){
//Клиентское прил. Отрисовать окно входа в чат(std string ServerIP, std::string информация)
std::thread NewThread(&ViewsManager::ViewClLoginScr,this,Message1,Message2);
NewThread.detach();
}
//---------------------------------------------------------------------------
void ViewsManager::NotifyViewSeCl(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2){
//Клиентское прил. Отрисовать окно чата(set<сообщения> Data, имя сервера, std::string информация)
std::thread NewThread(&ViewsManager::ViewClChatScr,this,Data,Message1,Message2);
NewThread.detach();
}
//---------------------------------------------------------------------------
void ViewsManager::NotifyViewSeCl(const std::string& Info){
std::lock_guard<std::mutex> lock(this->Mutex);
addstr(("\nNew Info: "+Info+"\n").c_str());
refresh();
}
//---------------------------------------------------------------------------
void ViewsManager::ViewSeMainScr(std::set<MessageSe> Data,std::string ServerName,std::string Info){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::SeMain)){
	std::time_t Time;
	char TimeBuff[32];

	clear();
	addstr(("Server: "+ServerName+"\n").c_str());
	addstr(("Info: "+Info+"\n").c_str());
	if(Data.size()>0){
		addstr("Messages:\n\n");
		for(auto &it:Data){
			Time=it.GetTime();
			std::strftime(TimeBuff, 32, "%H:%M:%S %d.%m.%Y \n", std::localtime(&Time));
			addstr((it.GetSenderName()+"\n").c_str());
			addstr(TimeBuff);
			addstr(("\n"+it.GetText()+"\n").c_str());
			addstr("-----------------------------------------------\n");
		}
	}else{
		addstr("No messages yet\n");
	}
	addstr("\nPress ENTER to turn ON server \n");
	addstr("\nPress SPACE to turn OFF server \n");
	addstr("Press ESC to close program\n");
	refresh();
}
}
//---------------------------------------------------------------------------
void ViewsManager::ViewClLoginScr(std::string ServerIP,std::string Info){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::ClLogin)){
clear();
addstr(("Server IP: "+ServerIP+"\n").c_str());
addstr(("Info: "+Info+"\n").c_str());
addstr("\nPress ENTER to write you name and join the chat\n");
addstr("Press ESC to close program\n");
addstr("Press BACKSPACE to change server IP\n");
refresh();
}
}
//---------------------------------------------------------------------------
void ViewsManager::ViewClChatScr(std::set<Message> Data,std::string ServerName,std::string Info){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::ClChat)){
	std::time_t Time;
	char TimeBuff[32];

	clear();
	addstr(("Server Name: "+ServerName+"\n").c_str());
	addstr(("Info: "+Info+"\n").c_str());
	if(Data.size()>0){
		addstr("Messages:\n");
		for(auto &it:Data){
			Time=it.GetTime();
			std::strftime(TimeBuff, 32, "%H:%M:%S %d.%m.%Y \n", std::localtime(&Time));
			addstr((it.GetSenderName()+"\n").c_str());
			addstr(TimeBuff);
			addstr(("\n"+it.GetText()+"\n").c_str());
			addstr("-----------------------------------------------\n");
		}
	}else{
		addstr("No messages yet\n");
	}
	addstr("\nPress ENTER to turn ON server \n");
	addstr("\nPress SPACE to turn OFF server \n");
	addstr("Press ESC to close program\n");
	refresh();
}
}
//---------------------------------------------------------------------------
std::string ViewsManager::ViewClLoginWritingModeScr(){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::ClLoginWr)){
	char Str[User::MAXNameLength];
	printw(("Write you name. Max length "+std::to_string(User::MAXNameLength)+" Allowed: 0-9, a-z, A-Z, signs(except @,[,])\n").c_str());
	printw("Name: ");
	curs_set(1);
	refresh();
	echo();
	getstr(Str);
	noecho();
	curs_set(0);
	return std::string(Str);
}
return "";
}
//---------------------------------------------------------------------------
std::string ViewsManager::ViewClSetIPScr(){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::ClSetIP)){
	char Str[16];
	printw("IPv4: ");
	curs_set(1);
	refresh();
	echo();
	getstr(Str);
	noecho();
	curs_set(0);
	return std::string(Str);
}
return "";
}
//---------------------------------------------------------------------------
std::string ViewsManager::ViewClChatWritingModeScr(){
std::lock_guard<std::mutex> lock(this->Mutex);
if(SetActiveView(enumActiveView::ClChatWr)){
	char Str[Message::MAXTextLength];
	printw(("Write message. Max length "+std::to_string(Message::MAXTextLength)+" Allowed: 0-9, a-z, A-Z, signs(except [,])\n").c_str());
	printw("Message:");
	curs_set(1);
	refresh();
	echo();
	getstr(Str);
	noecho();
	curs_set(0);
	return std::string(Str);
}
return "";
}
//---------------------------------------------------------------------------
bool ViewsManager::SetActiveView(enumActiveView newActiveView){
if(this->ActiveView==newActiveView){
	return true;
}
switch (this->ActiveView) {
case enumActiveView::SeMain:
break;
case enumActiveView::ClLogin:
	if((newActiveView==enumActiveView::ClChat) || (newActiveView==enumActiveView::ClSetIP)){
		this->ActiveView=newActiveView;
		return true;
	}
break;
case enumActiveView::ClSetIP:
	if(newActiveView==enumActiveView::ClLogin){
		this->ActiveView=newActiveView;
		return true;
	}
break;
case enumActiveView::ClChat:
	if((newActiveView==enumActiveView::ClLogin) || (newActiveView==enumActiveView::ClChatWr)){
		this->ActiveView=newActiveView;
		return true;
	}
break;
case enumActiveView::ClChatWr:
	if(newActiveView==enumActiveView::ClChat){
		this->ActiveView=newActiveView;
		return true;
	}
break;
default:
break;
}
return false;
}
//---------------------------------------------------------------------------
void ViewsManager::MainWork(bool SorC){//0-Server 1-Client

initscr();
raw();
noecho();
curs_set(0);
keypad(stdscr,TRUE);

int Ch;

if(SorC){
	this->ActiveView=enumActiveView::ClLogin;
	this->ViewClLoginScr("127.0.0.1","Hello world");
}else{
	this->ActiveView=enumActiveView::SeMain;
	this->ViewSeMainScr(std::set<MessageSe>(),"@Unknown","Hello world");
}

this->IsOn=true;
while(this->IsOn){

	Ch=getch();

	switch (this->ActiveView) {
	case enumActiveView::SeMain:
		if(Ch==10){
			this->ConcreteColleagueVtoS::SendToMediatorViewSeCl(1);
		}else if(Ch==' '){
			this->ConcreteColleagueVtoS::SendToMediatorViewSeCl(2);
		}else if(Ch==27){
			this->ConcreteColleagueVtoS::SendToMediatorViewSeCl(3);
			this->IsOn=false;
		}/*else{
			addstr(std::to_string(Ch).c_str());
		}*/
	break;
	case enumActiveView::ClLogin:
		if(Ch==10){
			std::string Name = this->ViewClLoginWritingModeScr();
			this->ConcreteColleagueVtoC::SendToMediatorViewSeCl(1,Name);
		}else if(Ch==KEY_BACKSPACE ){//Ввести IP сервера
			std::string IP = this->ViewClSetIPScr();
			this->ConcreteColleagueVtoC::SendToMediatorViewSeCl(2,IP);
		}else if(Ch==27){
			this->IsOn=false;
		}

	break;
	case enumActiveView::ClChat:
		if(Ch==10){//Ввести сообщение
			std::string MessageText = this->ViewClChatWritingModeScr();
			this->ConcreteColleagueVtoC::SendToMediatorViewSeCl(3,MessageText);
		}else if(Ch==27){//Выйти из чата
			this->ConcreteColleagueVtoC::SendToMediatorViewSeCl(4,"Bye");
		}
	break;
	default:
	break;
	}
	refresh();
}
endwin();
}
//---------------------------------------------------------------------------
