#ifndef __LOGIN_UTILS__
#define __LOGIN_UTILS__

#define LOGIN_FIRST "login-first"
#define LOGIN_DAYS "login-days"
#define TARGET_DAYS "target-days"

//一天的秒数
#define DAY_SECOND  86400

class LoginUtils{

public:
	static LoginUtils *getInstance();

	int getAfterLoginDays();
	void setLoginTime();
	int getTargetDays();
	void setTargetDays();
	bool checkLoginReward();
};
#endif