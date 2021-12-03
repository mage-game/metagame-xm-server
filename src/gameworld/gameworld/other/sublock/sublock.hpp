#ifndef __SUBLOCK_HPP__
#define __SUBLOCK_HPP__

#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"

class Role;

class SubLock
{
public:
	SubLock();
	~SubLock();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, const SublockString password_str, char is_unlocked, char today_error_times);
	void GetInitParam(SublockString password_str, char *has_lock, char *today_error_times);

	void OnDayChange(int old_dayid, int now_dayid);
	void OnRoleLogin() { if (m_has_pw) m_has_lock = true; }

	bool CheckHasLock(bool need_notice = true);
	void SendSubLockInfo();

	bool DeletePw(SublockMD5 password_md5);
	bool CreatePw(SublockMD5 password_md5);
	bool Lock();
	bool Unlock(SublockMD5 password_md5);

private:
	void InitPwHelper(const SublockMD5 password_md5);

	bool CheckMd5Illegal(SublockMD5 md5);
	bool CheckPwIsEqual(SublockMD5 md5, short operate_type);

	void SendOperateSucc(int operate_type);
	void SendOperateFail(short operate_type);

	Role			*m_role;

	bool			m_has_lock;					// 是否被锁定
	bool			m_has_pw;
	SublockMD5	    m_password_md5;
	short			m_today_error_times;		// 今天错误的次数
};

#endif

