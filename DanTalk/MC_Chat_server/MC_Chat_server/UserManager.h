#pragma once

#include <unordered_map>
#include <deque>
#include <string>
#include "Common.h"
using ERROR_CODE = Common::ERROR_CODE;

class User;

class UserManager
{
public:
	UserManager();
	virtual ~UserManager();

	void Init(const int maxUserCount);

	ERROR_CODE AddUser(const int sessionIndex, const char* pszID);
	ERROR_CODE RemoveUser(const int sessionIndex);

	std::tuple<ERROR_CODE, User*> GetUser(const int sessionIndex);
	std::tuple<ERROR_CODE, User*> GetUser(const char* pszID);

private:
	User* AllocUserObjPoolIndex();
	void ReleaseUserObjPoolIndex(const int index);

	User* FindUser(const int sessionIndex);
	User* FindUser(std::string pszID);

private:
	std::vector<User> m_UserObjPool;
	std::deque<int> m_UserObjPoolIndex;

	std::unordered_map<int, User*> m_UserSessionDic;
	std::unordered_map<std::string, User*> m_UserIDDic; //char*는 key로 사용못함

};