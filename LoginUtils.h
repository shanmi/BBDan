#ifndef __LOGIN_UTILS__
#define __LOGIN_UTILS__

#define LOGIN_FIRST "login-first"
#define LOGIN_DAYS "login-days"
#define TARGET_DAYS "target-days"

//һ�������
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