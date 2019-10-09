//---------------------------------------------------------------------------
#ifndef UsersH
#define UsersH
#include <algorithm>
#include <string>
//---------------------------------------------------------------------------
class User{
private:
	std::string IP="127.0.0.1";
	std::string Name="@Anonym";

	void SetIP(std::string newIP);
	void SetName(std::string newName);
public:
	static const int MAXNameLength;

	static std::string CleanNameString(std::string newName);

	User()= default;
	User(std::string newIP, std::string newName);
	User(std::string newIP);

	std::string GetIP()const;
	std::string GetName()const;

	bool operator<(const User& Other)const;
	bool operator==(const User& Other)const;
};
//---------------------------------------------------------------------------
#endif
