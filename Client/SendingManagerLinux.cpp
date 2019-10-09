//---------------------------------------------------------------------------

#pragma hdrstop

#include "SendingManagerLinux.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
SendingManagerL::~SendingManagerL(){
this->TurnOff();
}
//---------------------------------------------------------------------------
unsigned short int SendingManagerL::Send(std::string IPSendTo,std::string Message){
const char* SendingMessage = Message.c_str();
int sock;
struct sockaddr_in addr;

sock = socket(AF_INET, SOCK_STREAM, 0);
if(sock < 0){
	return 1;
}

addr.sin_family = AF_INET;
addr.sin_port = htons(6106);

addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);/*
if (inet_aton(IPSendTo.c_str(),&addr.sin_addr)) == 0) {
	return 2;
}                                               */

if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0){
	return 3;
}

int len=sizeof(SendingMessage);
int n,total = 0;
while(total < len){
	n = send(sock, SendingMessage+total, len-total, 0);
	if(n == -1) { break; }
	total += n;
}

close(sock);
return 0;
}
//---------------------------------------------------------------------------
void SendingManagerL::ReadingLoop(){
if(this->IsOn){
	int sock, listener;
	struct sockaddr_in addr;
	char buf[1024];
	int bytes_read;
	std::string Data="";

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0){
		this->IsOn=false;
		return;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6106);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
		this->IsOn=false;
		return;
	}

	listen(listener, 1);

	while(this->IsOn){
		sock = accept(listener, NULL, NULL);
		if(sock < 0){
			this->IsOn=false;
			return;
		}
		while(true){
			bytes_read = recv(sock, buf, 1024, 0);
			if(bytes_read <= 0) break;
			Data+=buf;
		}
		if(Data.size()>0){
			//char Address[16];
			char Address[INET_ADDRSTRLEN];				
			struct sockaddr_in *addr_peer;
			socklen_t addrlen = sizeof(*addr_peer);

			int err=getpeername(sock, (struct sockaddr *)addr_peer, &addrlen);;
			inet_ntop(AF_INET, &(addr_peer->sin_addr), Address, INET_ADDRSTRLEN);

			this->SendToMediatorSMLSeCl(Address,Data);
		}
		close(sock);
	}
}
this->IsOn=false;
}
//---------------------------------------------------------------------------
bool SendingManagerL::TurnOn(){
if(!this->IsOn){
	this->Thread=std::thread(&SendingManagerL::ReadingLoop,this);
	/*this->Thread=std::thread([this](){
		this->ReadingLoop();
	});*/
	this->Thread.detach();
	this->IsOn=true;
}
return true;
}
//---------------------------------------------------------------------------
void SendingManagerL::TurnOff(){
this->IsOn=false;
/*if(this->Thread.joinable()){
	this->Thread.join();
}*/
}
//---------------------------------------------------------------------------
bool SendingManagerL::GetActive(){
return this->IsOn;
}
//---------------------------------------------------------------------------
