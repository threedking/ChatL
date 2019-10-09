//---------------------------------------------------------------------------
#ifndef ViewsManagerH
#define ViewsManagerH
#include "Mediator.h"
#include "Messages.h"
#include "Users.h"
#include <ncurses.h>
#include <set>
#include <string>
#include <mutex>
#include <thread>
//---------------------------------------------------------------------------
class ViewsManager:public ConcreteColleagueVtoC, public ConcreteColleagueVtoS{
private:
	ViewsManager()=default;
	ViewsManager(const ViewsManager&)=delete;
	ViewsManager& operator=(ViewsManager&)= delete;

	std::mutex Mutex;
	std::thread Thread;

	bool IsOn=false;

	enum enumActiveView{
		SeMain,
		ClLogin,
		ClChat,
		ClLoginWr,
		ClSetIP,
        ClChatWr
	};
	enumActiveView ActiveView;

	void NotifyViewSeCl(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2)override;
	void NotifyViewSeCl(const std::string& Message1,const std::string& Message2)override;
	void NotifyViewSeCl(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2)override;

	void NotifyViewSeCl(const std::string& Info)override;

	void ViewSeMainScr(std::set<MessageSe> Data,std::string ServerName,std::string Info);
	void ViewClLoginScr(std::string ServerIP,std::string Info);
	void ViewClChatScr(std::set<Message> Data,std::string ServerName,std::string Info);
	std::string ViewClLoginWritingModeScr();
	std::string ViewClSetIPScr();
	std::string ViewClChatWritingModeScr();

public:
	static ViewsManager& getInstance(){
		static ViewsManager instance;
		return instance;
	}

	void MainWork(bool SorC);
	bool SetActiveView(enumActiveView newActiveView);//Каждый вид сам пытается установить себя активным и отрабатывает, если получил true
};
//---------------------------------------------------------------------------
#endif
