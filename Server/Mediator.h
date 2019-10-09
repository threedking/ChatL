//---------------------------------------------------------------------------
#ifndef MediatorH
#define MediatorH
#include "Messages.h"
#include <string>
#include <set>
//---------------------------------------------------------------------------
class Colleague;
class MediatorSMLSeCl;
class ConcreteMediatorSMLSeCl;
class ConcreteColleagueSeCl;
class ConcreteColleagueSML;
//---------------------------------------------------------------------------
class MediatorSMLSeCl{
public:
	virtual void Send(const std::string& Message1,const std::string& Message2,Colleague *colleague) const = 0;
};
//---------------------------------------------------------------------------
class Colleague{
protected:
	MediatorSMLSeCl* mediator_=NULL;
public:
	void SetMediatorSMLSeCl(MediatorSMLSeCl* mediator);
};
//---------------------------------------------------------------------------
class ConcreteColleagueSeCl:public Colleague{
public:
	ConcreteColleagueSeCl()=default;

	void SendToMediatorSMLSeCl(const std::string& Message1,const std::string& Message2);
	virtual void NotifySMLSeCl(const std::string& Message1,const std::string& Message2){}
};
//---------------------------------------------------------------------------
class ConcreteColleagueSML:public Colleague{
public:
	ConcreteColleagueSML()=default;

	void SendToMediatorSMLSeCl(const std::string& Message1,const std::string& Message2);
	virtual void NotifySMLSeCl(const std::string& Message1,const std::string& Message2){}
};
//---------------------------------------------------------------------------
class ConcreteMediatorSMLSeCl:public MediatorSMLSeCl
{
protected:
	ConcreteColleagueSeCl* m_ColleagueSeCl;
	ConcreteColleagueSML* m_ColleagueSML;
public:
	void SetColleagueSeCl(ConcreteColleagueSeCl* c);
	void SetColleagueSML(ConcreteColleagueSML* c);
	virtual void Send(const std::string& Message1,const std::string& Message2, Colleague* colleague) const;
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class MediatorViewSeCl;
class ColleagueViewSeCl;
class ConcreteMediatorViewSeCl;
class ConcreteColleagueStoV;
class ConcreteColleagueVtoS;
class ConcreteColleagueCtoV;
class ConcreteColleagueVtoC;

class MediatorViewSeCl{
public:
	virtual void Send(const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const = 0;
	virtual void Send(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const = 0;
	virtual void Send(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const = 0;
	virtual void Send(int Command, const std::string& Message,ColleagueViewSeCl* colleague) const = 0;
	virtual void Send(int Command,ColleagueViewSeCl* colleague) const = 0;
	virtual void Send(const std::string& Info,ColleagueViewSeCl* colleague) const = 0;
/*
Серв, Кл->Вьюшки
	Клиент
		Отрисовать окно входа в чат(std string ServerIP, std::string информация)
		Отрисовать окно чата(set<сообщения> Data, имя сервера, std::string информация)
	Сервер
		Отрисовать главное окно(set<сообщения> Data, имя сервера, std::string информация)
Вьюшки->Серв, Кл
	Клиент
		Вход в чат(1,std::string Name)
		Задать IP сервера(2,std::string IP)
		Отправить сообщение(3,std::string Text)
		Выход из чата(4,std::string Text)
	Сервер
		Запуск сервера(1)
		Остановка сервера(2)
		Закрытие программы(3)
*/
};
//---------------------------------------------------------------------------
class ColleagueViewSeCl{
protected:
	MediatorViewSeCl* mediator_=NULL;
public:
	void SetMediatorViewSeCl(MediatorViewSeCl* mediator);
};
//---------------------------------------------------------------------------
class ConcreteColleagueStoV: virtual public ColleagueViewSeCl{
public:
	ConcreteColleagueStoV()=default;

	void SendToMediatorViewSeCl(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2);
	void SendToMediatorViewSeCl(const std::string& Info);
	virtual void NotifyViewSeCl(int Command){}
};
//---------------------------------------------------------------------------
class ConcreteColleagueVtoS: virtual public ColleagueViewSeCl{
public:
	ConcreteColleagueVtoS()=default;

	void SendToMediatorViewSeCl(int Command);
	virtual void NotifyViewSeCl(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2){}
	virtual void NotifyViewSeCl(const std::string& Info){}
};
//---------------------------------------------------------------------------
class ConcreteColleagueCtoV: virtual public ColleagueViewSeCl{
public:
	ConcreteColleagueCtoV()=default;

	void SendToMediatorViewSeCl(const std::string& Message1,const std::string& Message2);
	void SendToMediatorViewSeCl(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2);
	void SendToMediatorViewSeCl(const std::string& Info);
	virtual void NotifyViewSeCl(int Command,const std::string& Message){}
};
//---------------------------------------------------------------------------
class ConcreteColleagueVtoC: virtual public ColleagueViewSeCl{
public:
	ConcreteColleagueVtoC()=default;

	void SendToMediatorViewSeCl(int Command,const std::string& Message);
	virtual void NotifyViewSeCl(const std::string& Message1,const std::string& Message2){}
	virtual void NotifyViewSeCl(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2){}
	virtual void NotifyViewSeCl(const std::string& Info){}
};
//---------------------------------------------------------------------------
class ConcreteMediatorViewSeCl:public MediatorViewSeCl
{
protected:
	ConcreteColleagueStoV* m_ColleagueStoV;
	ConcreteColleagueVtoS* m_ColleagueVtoS;
	ConcreteColleagueCtoV* m_ColleagueCtoV;
	ConcreteColleagueVtoC* m_ColleagueVtoC;
public:
	void SetColleagueStoV(ConcreteColleagueStoV* c);
	void SetColleagueVtoS(ConcreteColleagueVtoS* c);
	void SetColleagueCtoV(ConcreteColleagueCtoV* c);
	void SetColleagueVtoC(ConcreteColleagueVtoC* c);

	virtual void Send(const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const override;
	virtual void Send(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const override;
	virtual void Send(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const override;
	virtual void Send(int Command, const std::string& Message,ColleagueViewSeCl* colleague) const override;
	virtual void Send(int Command,ColleagueViewSeCl* colleague) const override;
	virtual void Send(const std::string& Info,ColleagueViewSeCl* colleague) const override;
};
//---------------------------------------------------------------------------
#endif