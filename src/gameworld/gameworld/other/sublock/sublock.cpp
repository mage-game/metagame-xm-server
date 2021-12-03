#include "sublock.hpp"
#include <memory.h>

#include "world.h"
#include "scene/scenemanager.h"

#include "obj/character/role.h"
#include "protocal/msgsublock.hpp"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "engineadapter.h"

static const int MAX_ERROR_TIMES_PER_DAY = 20;			// 每天允许尝试密码的次数
static const int PASSWORD_MIN_LENGTH = 4;				// 密码长度
static const int PASSWORD_MAX_LENGTH = 6;

UNSTD_STATIC_CHECK(PASSWORD_MIN_LENGTH <= PASSWORD_MAX_LENGTH);

SubLock::SubLock()
	: m_role(NULL), 
	m_has_lock(false),
	m_has_pw(false), 
	m_today_error_times(0)
{
	memset(m_password_md5, 0, sizeof(m_password_md5));
}

SubLock::~SubLock()
{

}

void SubLock::Init(Role *role, const SublockString password_str, char has_lock, char today_error_times)
{
	m_role = role;

	UNSTD_STATIC_CHECK(sizeof(SublockMD5) <= sizeof(SublockString));

	static SublockMD5 password_md5;
	memcpy(password_md5, password_str, sizeof(password_md5));

	m_today_error_times = today_error_times;

	this->InitPwHelper(password_md5);

	if (m_has_pw) m_has_lock = (0 != has_lock);
}

void SubLock::GetInitParam(SublockString password_str, char *has_lock, char *today_error_times)
{
	if (NULL == password_str || NULL == has_lock || NULL == today_error_times) return;

	UNSTD_STATIC_CHECK(sizeof(SublockMD5) < sizeof(SublockString));

	memcpy(password_str, m_password_md5, sizeof(SublockMD5));
	password_str[sizeof(SublockString) - 1] = '\0';

	*has_lock = m_has_lock ? 1 : 0;
	*today_error_times = (char)m_today_error_times;
}

void SubLock::OnDayChange(int old_dayid, int now_dayid)
{
	if (old_dayid != now_dayid) 
	{
		m_today_error_times = 0;

		this->SendSubLockInfo();
	}
}

bool SubLock::CheckHasLock(bool need_notice)
{
	if (World::GetInstWorld()->GetSceneManager()->IsForbidSublock()) return false;
	
	if (m_has_lock)
	{
		if (need_notice) m_role->NoticeNum(errornum::EN_SUBLOCK_IS_LOCKED);
		return true;
	}

	return false;
}

void SubLock::SendSubLockInfo()
{
	static Protocol::SCSubLockInfo si;

	si.has_pw = (short)m_has_pw;
	si.today_error_times = m_today_error_times;

	// EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&si, sizeof(si));
}

bool SubLock::DeletePw(SublockMD5 password_md5)
{
	if (!m_has_pw || !this->CheckMd5Illegal(password_md5) || !this->CheckPwIsEqual(password_md5, SUBLOCK_OPERATE_TYPE_DELETE_PW)) return false;
	
	m_has_lock = false;
	m_has_pw = false;
	memset(m_password_md5, 0, sizeof(m_password_md5));

	this->SendOperateSucc(SUBLOCK_OPERATE_TYPE_DELETE_PW);

	return true;
}

bool SubLock::CreatePw(SublockMD5 password_md5)
{
	if (m_has_pw || !this->CheckMd5Illegal(password_md5)) return false;

	this->InitPwHelper(password_md5);

	m_has_lock = false;					// 默认设置完不锁

	this->SendOperateSucc(SUBLOCK_OPERATE_TYPE_CREATE_PW);

	return true;
}

bool SubLock::Lock()
{
	if (!m_has_pw || m_has_lock) return false;

	m_has_lock = true;

	this->SendOperateSucc(SUBLOCK_OPERATE_TYPE_LOCK);

	return true;
}

bool SubLock::Unlock(SublockMD5 password_md5)
{
	if (!m_has_pw || !m_has_lock || !this->CheckMd5Illegal(password_md5) || !this->CheckPwIsEqual(password_md5, SUBLOCK_OPERATE_TYPE_UNLOCK)) return false;
	
	m_has_lock = false;

	this->SendOperateSucc(SUBLOCK_OPERATE_TYPE_UNLOCK);

	return true;
}

void SubLock::InitPwHelper(const SublockMD5 password_md5)
{
	memcpy(m_password_md5, password_md5, sizeof(m_password_md5));

	m_has_pw = (m_password_md5[0] != 0);
}

bool SubLock::CheckMd5Illegal(SublockMD5 md5)
{
	UNSTD_STATIC_CHECK(PASSWORD_MAX_LENGTH < (int)sizeof(SublockMD5));

	for (int i = 0; i <= PASSWORD_MAX_LENGTH; i++)
	{
		if (0 == md5[i] && i >= PASSWORD_MIN_LENGTH)
		{
			return true;
		}

		if (md5[i] < '0' || md5[i] > '9') break;
	}

	m_role->NoticeNum(errornum::EN_SUBLOCK_PASSWORD_ILLEGAL);
	return false;
}

bool SubLock::CheckPwIsEqual(SublockMD5 password_md5, short operate_type)
{
	if (m_today_error_times >= MAX_ERROR_TIMES_PER_DAY)
	{
		m_role->NoticeNum(errornum::EN_SUBLOCK_ERROR_TIMES_LIMIT);
		return false;
	}

	if (memcmp(m_password_md5, password_md5, sizeof(m_password_md5)) != 0)
	{
		++m_today_error_times;
		this->SendOperateFail(operate_type);
		return false;
	}

	m_today_error_times = 0;

	return true;
}

void SubLock::SendOperateSucc(int operate_type)
{
	static Protocol::SCSubLockOperateSucc operate_succ_ack;
	operate_succ_ack.operate_type = operate_type;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&operate_succ_ack, sizeof(operate_succ_ack));
}

void SubLock::SendOperateFail(short operate_type)
{
	static Protocol::SCSubLockOperateFail operate_fail_ack;
	operate_fail_ack.operate_type = operate_type;
	operate_fail_ack.today_error_times = m_today_error_times;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&operate_fail_ack, sizeof(operate_fail_ack));
}

