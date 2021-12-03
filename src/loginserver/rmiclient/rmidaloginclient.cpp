#include "rmidaloginclient.h"

#include "servercommon/darmi/dalogin.h"
#include "servercommon/darmi/darole.h"

#include "servercommon/servercommon.h"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();
USE_DATAACCESS_ROLE_VAR();
USE_DATAACCESS_LOGIN_VAR();

static char TLV_BUFF[4 * 1024];

bool RMILoginClient::GetProfNum(rmi::RMIBackObject *backobj)
{
	if (NULL == backobj)
	{
		return false;
	}

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::GET_PROF_NUM, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
	}

	return ret;
}

bool RMILoginClient::UserLoginAsyn(const char *pname, char can_insert, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(pname) && s.Push(can_insert);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::USER_LOGIN, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}

bool RMIGetProfNumBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1, prof1_num = 0, prof2_num = 0, prof3_num = 0, prof4_num = 0;

	if (out_param.Pop(&ret))
	{
		if (0 == ret && out_param.Pop(&prof1_num) && out_param.Pop(&prof2_num) && out_param.Pop(&prof3_num) && out_param.Pop(&prof4_num))
		{
			this->GetProfNumRet(ret, prof1_num, prof2_num, prof3_num, prof4_num);
		}
		else
		{
			this->GetProfNumRet(ret, 0, 0, 0, 0);
		}

		return true;
	}

	return false;
}
void RMIGetProfNumBackObject::GetProfNumRet(int ret, int prof1_num, int prof2_num, int prof3_num, int prof4_num)
{
	printf("GetProfNumRet:%d\n", ret);
}

void RMIGetProfNumBackObject::__free()
{
	delete this;
}

bool RMIUserLoginBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1, db_index = 0, role_1 = 0, role_2 = 0, role_3 = 0;

	if (out_param.Pop(&ret))
	{
		if (ret == 0 && out_param.Pop(&db_index) && out_param.Pop(&role_1) && out_param.Pop(&role_2) && out_param.Pop(&role_3))
		{
			UserLoginRet(ret, db_index, role_1, role_2, role_3);
		}
		else
		{
			UserLoginRet(ret, 0, 0, 0, 0);
		}

		return true;
	}

	return false;
}
void RMIUserLoginBackObject::UserLoginRet(int ret, int db_index, int role_1, int role_2, int role_3)
{
	printf("UserLoginRet:%d\n", ret);
}

void RMIUserLoginBackObject::__free()
{
	delete this;
}

bool RMILoginClient::AddRoleAsyn(const char *pname, int db_index, int role_id, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("sii", pname, db_index, role_id);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::ADD_ROLE, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMIAddRoleBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		Error();
		return false;
	}
	AddRoleRet(ret);
	return true;
}
void RMIAddRoleBackObject::AddRoleRet(int result)
{
	printf("AddRoleRet:%d\n", result);
}
void RMIAddRoleBackObject::__free()
{
	delete this;
}

bool RMILoginClient::RemoveRoleAsyn(const char *pname, int db_index, int role_id, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("sii", pname, db_index, role_id);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::REMOVE_ROLE, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMIRemoveRoleBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	RemoveRoleRet(ret);
	return true;
}
void RMIRemoveRoleBackObject::RemoveRoleRet(int result)
{
	printf("RemoveRoleRet:%d\n", result);
}
void RMIRemoveRoleBackObject::__free()
{
	delete this;
}

bool RMIPassAntiWallowBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}

	PassAntiWallowRet(ret);

	return true;
}

void RMIPassAntiWallowBackObject::PassAntiWallowRet(int ret)
{
	printf("PassAntiWallowRet:%d\n", ret);
}

void RMIPassAntiWallowBackObject::__free()
{
	delete this;
}

bool RMILoginClient::Frobid(const char *pname, unsigned int forbid_time, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(pname) && s.Push(forbid_time);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::FORBID, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}

bool RMILoginClient::FrobidOneRole(const char *pname, int role_id, unsigned int forbid_time, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(pname) && s.Push(role_id) && s.Push(forbid_time);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::FORBID_ONE_ROLE, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}

bool RMIForbidBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	int db_index = 0, role_1 = 0, role_2 = 0, role_3 = 0;
	if (!out_param.Pop(&ret) || !out_param.Pop(&db_index) || !out_param.Pop(&role_1) || !out_param.Pop(&role_2) || !out_param.Pop(&role_3))
	{
		return false;
	}
	ForbidRet(ret, db_index, role_1, role_2, role_3);

	return true;
}

void RMIForbidBackObject::ForbidRet(int result, int db_index, int role_1, int role_2, int role_3)
{
	printf("ForbidRet:%d\n", result);
}
void RMIForbidBackObject::__free()
{
	delete this;
}

bool RMIForbidOneRoleBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	int db_index = 0;
	if (!out_param.Pop(&ret) || !out_param.Pop(&db_index))
	{
		return false;
	}
	ForbidOneRoleRet(ret, db_index);

	return true;
}

void RMIForbidOneRoleBackObject::ForbidOneRoleRet(int result, int db_index)
{
	printf("ForbidOneRoleRet:%d \n", result);
}

void RMIForbidOneRoleBackObject::__free()
{
	delete this;
}

bool RMILoginClient::AddNameInfoAsyn(const char *role_name, const char *pname, rmi::RMIBackObject *backobj)
{
	if (NULL == role_name || NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(role_name) && s.Push(pname);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::ADD_NAME_INFO, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMIAddNameInfoBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	AddNameInfoRet(ret);
	return true;
}
void RMIAddNameInfoBackObject::AddNameInfoRet(int result)
{
	printf("AddNameInfoRet:%d\n", result);
}
void RMIAddNameInfoBackObject::__free()
{
	delete this;
}

bool RMILoginClient::UpdateNameInfoAsyn(const char *role_name, int db_index, int role_id, rmi::RMIBackObject *backobj)
{
	if (NULL == role_name) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Pushf("sii", role_name, db_index, role_id);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::UPDATE_NAME_INFO, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMIUpdateNameInfoBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	UpdateNameInfoRet(ret);
	return true;
}
void RMIUpdateNameInfoBackObject::UpdateNameInfoRet(int result)
{
	printf("UpdateNameInfoRet:%d\n", result);
}
void RMIUpdateNameInfoBackObject::__free()
{
	delete this;
}

bool RMILoginClient::DeleteNameInfoAsyn(int db_index, int role_id, const char *pname, rmi::RMIBackObject *backobj)
{
	if (NULL == pname) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(db_index) && s.Push(role_id) && s.Push(pname);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::DELETE_NAME_INFO, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMIDeleteNameInfoBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	DeleteNameInfoRet(ret);
	return true;
}
void RMIDeleteNameInfoBackObject::DeleteNameInfoRet(int result)
{
	printf("DeleteNameInfoRet:%d\n", result);
}
void RMIDeleteNameInfoBackObject::__free()
{
	delete this;
}
bool RMILoginClient::DeleteNameInfoByNameAsyn(const char *role_name, rmi::RMIBackObject *backobj)
{
	if (NULL == role_name) return false;

	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(role_name);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::DELETE_NAME_INFO_BY_NAME, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}

bool RMIDeleteNameInfoByNameBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	DeleteNameInfoByNameRet(ret);
	return true;
}
void RMIDeleteNameInfoByNameBackObject::__free()
{
	delete this;
}
void RMIDeleteNameInfoByNameBackObject::DeleteNameInfoByNameRet(int result)
{
	printf("DeleteNameInfoByNameRet:%d\n", result);
}

bool RMILoginClient::InitWorldStatus(rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::INIT_WORLD_STATUS, s, backobj, true, 5000);
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMILoginInitWorldStatusBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	int is_register_limit = 0;

	if (!out_param.Pop(&ret) || !out_param.Pop(&is_register_limit))
	{
		return false;
	}

	InitWorldStatusRet(ret, 0 != is_register_limit);
	return true;
}
void RMILoginInitWorldStatusBackObject::__free()
{
	delete this;
}
void RMILoginInitWorldStatusBackObject::InitWorldStatusRet(int result, bool is_register_limit)
{
	printf("RMILoginInitWorldStatusBackObject::InitWorldStatusRet:%d\n", result);
}

bool RMILoginClient::SaveWorldStatus(bool is_register_limit, rmi::RMIBackObject *backobj)
{
	TLVSerializer s;
	s.Reset(TLV_BUFF, sizeof(TLV_BUFF));

	bool ret = s.Push(is_register_limit ? 1 : 0);
	if (ret)
	{
		ret = m_session.rmi_module->Call(m_session, dataaccessrmi::LOGIN, dataaccessrmi::login::SAVE_WORLD_STATUS, s, backobj, true, 5000);
	}
	if (!ret)
	{
		delete backobj;
	}
	return ret;
}
bool RMILoginSaveWorldStatusBackObject::__response(TLVUnserializer &out_param)
{
	int ret = -1;
	if (!out_param.Pop(&ret))
	{
		return false;
	}
	SaveWorldStatusRet(ret);
	return true;
}
void RMILoginSaveWorldStatusBackObject::__free()
{
	delete this;
}
void RMILoginSaveWorldStatusBackObject::SaveWorldStatusRet(int result)
{
	printf("RMILoginSaveWorldStatusBackObject::SaveWorldStatusRet:%d\n", result);
}
