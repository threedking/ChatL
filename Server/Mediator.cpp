//---------------------------------------------------------------------------

#pragma hdrstop

#include "Mediator.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
void Colleague::SetMediatorSMLSeCl(MediatorSMLSeCl* mediator){
this->mediator_=mediator;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueSeCl::SendToMediatorSMLSeCl(const std::string& Message1,const std::string& Message2){
if(this->mediator_!=NULL){
	this->mediator_->Send(Message1, Message2, this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueSML::SendToMediatorSMLSeCl(const std::string& Message1,const std::string& Message2){
if(this->mediator_!=NULL){
	this->mediator_->Send(Message1, Message2, this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteMediatorSMLSeCl::SetColleagueSeCl(ConcreteColleagueSeCl* c){
this->m_ColleagueSeCl=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorSMLSeCl::SetColleagueSML(ConcreteColleagueSML* c){
this->m_ColleagueSML=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorSMLSeCl::Send(const std::string& Message1,const std::string& Message2, Colleague* colleague) const{
if (colleague==this->m_ColleagueSeCl){
	m_ColleagueSML->NotifySMLSeCl(Message1, Message2);
}else if (colleague==this->m_ColleagueSML){
	m_ColleagueSeCl->NotifySMLSeCl(Message1, Message2);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ColleagueViewSeCl::SetMediatorViewSeCl(MediatorViewSeCl* mediator){
this->mediator_=mediator;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueStoV::SendToMediatorViewSeCl(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2){
if(this->mediator_!=NULL){
	this->mediator_->Send(Data,Message1,Message2,this);
}
}
//---------------------------------------------------------------------------
void ConcreteColleagueStoV::SendToMediatorViewSeCl(const std::string& Info){
if(this->mediator_!=NULL){
	this->mediator_->Send(Info,this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueVtoS::SendToMediatorViewSeCl(int Command){
if(this->mediator_!=NULL){
	this->mediator_->Send(Command,this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueCtoV::SendToMediatorViewSeCl(const std::string& Message1,const std::string& Message2){
if(this->mediator_!=NULL){
	this->mediator_->Send(Message1,Message2,this);
}
}
//---------------------------------------------------------------------------
void ConcreteColleagueCtoV::SendToMediatorViewSeCl(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2){
if(this->mediator_!=NULL){
	this->mediator_->Send(Data,Message1,Message2,this);
}
}
//---------------------------------------------------------------------------
void  ConcreteColleagueCtoV::SendToMediatorViewSeCl(const std::string& Info){
if(this->mediator_!=NULL){
	this->mediator_->Send(Info,this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteColleagueVtoC::SendToMediatorViewSeCl(int Command,const std::string& Message){
if(this->mediator_!=NULL){
	this->mediator_->Send(Command,Message,this);
}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::SetColleagueStoV(ConcreteColleagueStoV* c){
this->m_ColleagueStoV=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::SetColleagueVtoS(ConcreteColleagueVtoS* c){
this->m_ColleagueVtoS=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::SetColleagueCtoV(ConcreteColleagueCtoV* c){
this->m_ColleagueCtoV=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::SetColleagueVtoC(ConcreteColleagueVtoC* c){
this->m_ColleagueVtoC=c;
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueCtoV){
	this->m_ColleagueVtoC->NotifyViewSeCl(Message1, Message2);
}
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(const std::set<Message>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueCtoV){
	this->m_ColleagueVtoC->NotifyViewSeCl(Data,Message1,Message2);
}
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(const std::set<MessageSe>& Data,const std::string& Message1,const std::string& Message2,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueStoV){
	this->m_ColleagueVtoS->NotifyViewSeCl(Data,Message1,Message2);
}
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(int Command, const std::string& Message,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueVtoC){
	this->m_ColleagueCtoV->NotifyViewSeCl(Command,Message);
}
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(int Command,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueVtoS){
	this->m_ColleagueStoV->NotifyViewSeCl(Command);
}
}
//---------------------------------------------------------------------------
void ConcreteMediatorViewSeCl::Send(const std::string& Info,ColleagueViewSeCl* colleague) const{
if(colleague==this->m_ColleagueStoV){
	this->m_ColleagueVtoS->NotifyViewSeCl(Info);
}else if(colleague==this->m_ColleagueCtoV){
	this->m_ColleagueVtoC->NotifyViewSeCl(Info);
}
}
//---------------------------------------------------------------------------