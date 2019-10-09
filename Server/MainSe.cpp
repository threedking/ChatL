#include "Mediator.h"
#include "SendingManagerLinux.h"
#include "ViewsManager.h"
#include "Server.h"
//---------------------------------------------------------------------------
int main(int argc, char** argv){
SendingManagerL* SendingManagerInstance=&SendingManagerL::getInstance();
ViewsManager* ViewsManagerInstance=&ViewsManager::getInstance();
Server* ServerInstance=&Server::getInstance();

ServerInstance->SetSendingManager(&SendingManagerL::getInstance());

ConcreteMediatorSMLSeCl CMediatorSMLSeCl;
ConcreteMediatorViewSeCl CMediatorViewSeCl;

CMediatorSMLSeCl.SetColleagueSML(&SendingManagerL::getInstance());
CMediatorSMLSeCl.SetColleagueSeCl(&Server::getInstance());
CMediatorViewSeCl.SetColleagueVtoS(&ViewsManager::getInstance());
CMediatorViewSeCl.SetColleagueStoV(&Server::getInstance());

SendingManagerInstance->SetMediatorSMLSeCl(&CMediatorSMLSeCl);
ServerInstance->SetMediatorSMLSeCl(&CMediatorSMLSeCl);
ServerInstance->SetMediatorViewSeCl(&CMediatorViewSeCl);
ViewsManagerInstance->SetMediatorViewSeCl(&CMediatorViewSeCl);

ViewsManagerInstance->MainWork(false);
}