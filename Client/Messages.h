//---------------------------------------------------------------------------
#ifndef MessagesH
#define MessagesH

#include <ctime>
#include <algorithm>
#include <string>
//---------------------------------------------------------------------------
class Message{
protected:
	unsigned int ID=0;//Ќазначить только извне. 0 - не назначено

	std::string Text="";
	std::time_t Time;
	std::string SenderName="@Anonym";

	void SetText(std::string newText);
public:
	static const int MAXTextLength;
	static bool CheckSingleLetter(unsigned short Key);
	static std::string GetParsedSubString(const std::string &Str,std::string Key);

	Message();
	Message(std::string newText, std::string newSenderName);
	Message(std::string newText, std::string newSenderName, unsigned int newID);
	Message(std::string newText, std::string newSenderName, unsigned int newID,std::time_t newTime);

	std::string GetText()const;
	std::time_t GetTime()const;
	std::string GetSenderName()const;
	int GetID()const;

	bool operator<(const Message& Other)const;
	bool operator==(const Message& Other)const;
};
//---------------------------------------------------------------------------
class MessageSe: public Message{
protected:
	static unsigned int Count;
public:
	MessageSe();
	MessageSe(std::string newText, std::string newSenderName);
};
//---------------------------------------------------------------------------
#endif
