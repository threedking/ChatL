#include "Mediator.h"
#include "Client.h"
#include "SendingManagerLinux.h"
#include "ViewsManager.h"
//---------------------------------------------------------------------------
int main(int argc, char** argv){
SendingManagerL* SendingManagerInstance=&SendingManagerL::getInstance();
ViewsManager* ViewsManagerInstance=&ViewsManager::getInstance();
Client* ClientInstance=&Client::getInstance();

ClientInstance->SetSendingManager(&SendingManagerL::getInstance());

ConcreteMediatorSMLSeCl CMediatorSMLSeCl;
ConcreteMediatorViewSeCl CMediatorViewSeCl;

CMediatorSMLSeCl.SetColleagueSML(&SendingManagerL::getInstance());
CMediatorSMLSeCl.SetColleagueSeCl(&Client::getInstance());
CMediatorViewSeCl.SetColleagueVtoC(&ViewsManager::getInstance());
CMediatorViewSeCl.SetColleagueCtoV(&Client::getInstance());

SendingManagerInstance->SetMediatorSMLSeCl(&CMediatorSMLSeCl);
ClientInstance->SetMediatorSMLSeCl(&CMediatorSMLSeCl);
ClientInstance->SetMediatorViewSeCl(&CMediatorViewSeCl);
ViewsManagerInstance->SetMediatorViewSeCl(&CMediatorViewSeCl);

ViewsManagerInstance->MainWork(true);
}