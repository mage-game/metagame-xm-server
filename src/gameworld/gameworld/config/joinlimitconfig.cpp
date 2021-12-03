#include "joinlimitconfig.hpp"
#include "obj/character/role.h"
#include "task/taskpool.h"
#include "other/capability/capability.hpp"
#include "engineadapter.h"
#include "config/sharedconfig/sharedconfig.h"
#include "config/joinlimitconfig.hpp"
#include "task/taskrecorder.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

JoinLimitConfig::JoinLimitConfig() 
{
	
}

JoinLimitConfig::~JoinLimitConfig()
{

}

bool JoinLimitConfig::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "JoinLimitConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + "xml root node error.";
		return false;
	}

	{
		PugiXmlNode root_element = RootElement.child("join_limit");
		if (root_element.empty())
		{
			*err = path + "xml no join_limit node.";
			return false;
		}

		iRet = this->InitJoinLImitConfig(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitJoinLImitConfig fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	{
		PugiXmlNode root_element = RootElement.child("fun_open");
		if (root_element.empty())
		{
			*err = path + "xml no fun_open node.";
			return false;
		}

		iRet = this->InitFunOpenConfig(root_element);
		if (0 != iRet)
		{
			sprintf(errinfo, "%s : InitFunOpenConfig fail %d", path.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	return true;
}

const FunOpenUnit * JoinLimitConfig::GetFunOpenCfg(int open_type)
{
	if (open_type >= 0 && open_type < FUN_OPEN_TYPE_MAX)
	{
		return &m_funopen_cfglist[open_type];
	}

	return NULL;
}

TaskID JoinLimitConfig::GetFunOpenTaskID(int open_type)
{
	if (open_type >= 0 && open_type < FUN_OPEN_TYPE_MAX)
	{
		return m_funopen_cfglist[open_type].task_id;
	}

	return INVALID_TASK_ID;
}

int JoinLimitConfig::GetFunOpenRoleLevel(int open_type)
{
	if (open_type >= 0 && open_type < FUN_OPEN_TYPE_MAX)
	{
		return m_funopen_cfglist[open_type].role_level;
	}

	return MAX_ROLE_LEVEL + 1;
}

int JoinLimitConfig::GetFunOpenParam1(int open_type)
{
	if (open_type >= 0 && open_type < FUN_OPEN_TYPE_MAX)
	{
		return m_funopen_cfglist[open_type].param1;
	}

	return 0;
}

int JoinLimitConfig::GetFunOpenParam2(int open_type)
{
	if (open_type >= 0 && open_type < FUN_OPEN_TYPE_MAX)
	{
		return m_funopen_cfglist[open_type].param2;
	}

	return 0;
}

bool JoinLimitConfig::GetJoinLimit(int main_type, int sub_type, JoinLimitUnit *limit_unit)
{
	if (NULL == limit_unit)
	{
		return false;
	}

	JoinLimitType limit_type(main_type, sub_type);
	long long key = this->JoinLimitTypeToLongLong(limit_type);

	JoinLimitMapIt it = m_limit_map.find(key);
	if (m_limit_map.end() != it)
	{
		*limit_unit = it->second;
		return true;
	}

	return false;
}

bool JoinLimitConfig::GetActivityJoinLimit(int activity_type, JoinLimitUnit *limit_unit)
{
	 bool ret = this->GetJoinLimit(JLMT_ACTIVITY, activity_type, limit_unit);
	 return ret;
}

bool JoinLimitConfig::GetFBJoinLimit(int fb_type, JoinLimitUnit *limit_unit)
{
	bool ret = this->GetJoinLimit(JLMT_TYPE_FB, fb_type, limit_unit);
	return ret;
}

bool JoinLimitConfig::GetOtherJoinLimit(int sub_type, JoinLimitUnit *limit_unit)
{
	bool ret = this->GetJoinLimit(JLMT_OTHER, sub_type, limit_unit);
	return ret;
}

bool JoinLimitConfig::CanJoinActivity(int activity_type, Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	JoinLimitUnit join_limit;
	bool has_limit = this->GetActivityJoinLimit(activity_type, &join_limit);

	if (has_limit)
	{
		return this->CheckCanJoin(join_limit, role);
	}

	return true;
}

bool JoinLimitConfig::CanJoinFB(int fb_type, Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	JoinLimitUnit join_limit;
	bool has_limit = this->GetFBJoinLimit(fb_type, &join_limit);

	if (has_limit)
	{
		return this->CheckCanJoin(join_limit, role);
	}

	return true;
}

bool JoinLimitConfig::CanJoinOther(int sub_type, Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	JoinLimitUnit join_limit;
	bool has_limit = this->GetOtherJoinLimit(sub_type, &join_limit);

	if (has_limit)
	{
		return this->CheckCanJoin(join_limit, role);
	}

	return true;
}

long long JoinLimitConfig::JoinLimitTypeToLongLong(JoinLimitType limit_type)
{
	union ParamToLongLong
	{
		struct
		{
			int m_param_1;
			int m_param_2;
		};

		long long val;
	};

	ParamToLongLong ll;

	ll.m_param_1 = limit_type.main_type;
	ll.m_param_2 = limit_type.sub_type;

	return ll.val;
}

int JoinLimitConfig::InitJoinLImitConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		JoinLimitUnit unit;
		if (!PugiGetSubNodeValue(data_element, "type", unit.join_type.main_type) || unit.join_type.main_type <= 0 || unit.join_type.main_type >= JLMT_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "sub_type", unit.join_type.sub_type) || unit.join_type.sub_type < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "level", unit.level) || unit.level < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "capability", unit.capability) || unit.capability < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "max_level", unit.max_level) || unit.max_level < 0 || unit.max_level < unit.level)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "is_notice", unit.is_notice))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "day_limit_switch", unit.day_limit_switch))
		{
			return -7;
		}

		if (unit.day_limit_switch)
		{
			int read_level_result = this->ReadListInList(data_element, "level_range_vec", unit.level_range_vec, ",", "|");
			if (read_level_result < 0)
			{
				return -10 + read_level_result;
			}
			for (size_t i = 0; i < unit.level_range_vec.size(); ++i)
			{
				if (unit.level_range_vec[i].size() != 2)
				{
					return -1000 - i;
				}
			}

			int read_open_day_result = this->ReadListInList(data_element, "open_day_vec_vec", unit.open_day_vec_vec, ",", "|");
			if (read_open_day_result < 0)
			{
				return -20 + read_open_day_result;
			}
			for (size_t i = 0; i < unit.open_day_vec_vec.size(); ++i)
			{
				for (size_t j = 0; j < unit.open_day_vec_vec[i].size(); ++j)
				{
					if (unit.open_day_vec_vec[i][j] >= DAY_PER_WEEK)
					{
						unit.open_day_vec_vec[i][j] %= DAY_PER_WEEK;		//范围限制在周0到周6，0代表周日
					}
				}
			}

			int read_open_server_day_range_result = this->ReadListInList(data_element, "open_server_day_range_vec", unit.open_server_day_range_vec, ",", "|");
			if (read_open_server_day_range_result < 0)
			{
				return -30 + read_open_server_day_range_result;
			}
			for (size_t i = 0; i < unit.open_server_day_range_vec.size(); ++i)
			{
				if (unit.open_server_day_range_vec[i].size() != 2)
				{
					return -3000 - i;
				}
			}

			if (unit.level_range_vec.size() != unit.open_day_vec_vec.size() || unit.level_range_vec.size() != unit.open_server_day_range_vec.size())
			{
				return -10000;
			}
		}
		
		long long key = this->JoinLimitTypeToLongLong(unit.join_type);
		JoinLimitMapIt it = m_limit_map.find(key);

		if (m_limit_map.end() != it)
		{
			return -30000;
		}

		m_limit_map[key] = unit;

		data_element = data_element.next_sibling();
	}

	return 0;
}

bool JoinLimitConfig::CheckCanJoin(const JoinLimitUnit &limit_unit, Role *role)
{
	if (NULL == role)
	{
		return false;
	}

	if (limit_unit.level > 0 && role->GetLevel() < limit_unit.level)
	{
		if (limit_unit.is_notice)
		{
			role->NoticeNum(errornum::EN_JOIN_LIMIT_LEVEL_NOT_ENOUGH);
		}
		return false;
	}

	if (limit_unit.level > 0 && role->GetLevel() > limit_unit.max_level)
	{
		if (limit_unit.is_notice)
		{
			role->NoticeNum(errornum::EN_JOIN_LIMIT_LEVEL_EXCEEDS_MAXIMUM);
		}
		return false;
	}

	if (limit_unit.capability > 0 && role->GetCapability()->GetTotalCapability() < limit_unit.capability)
	{
		if (limit_unit.is_notice)
		{
			role->NoticeNum(errornum::EN_JOIN_LIMIT_CAP_NOT_ENOUGH);
		}
		return false;
	}

	if (limit_unit.day_limit_switch)
	{
		const int left_border = 0;
		const int right_border = 1;
		const int border_size = 2;
		bool is_open_day = false;
		int opengame_day = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerDayIndex() + 1;
		const tm *local_tm = EngineAdapter::Instance().LocalTime();
		if (nullptr == local_tm)
		{
			return false;
		}
		if (CrossConfig::Instance().IsHiddenServer())
		{
			return false;
		}
		size_t size = limit_unit.level_range_vec.size();
		//开关内vec的size要保持一致，在读配置的时候检查了,严谨起见，这里还是判断一下，防止越界
		if (size != limit_unit.open_server_day_range_vec.size() || size != limit_unit.open_day_vec_vec.size())
		{
			return false;
		}
		for (size_t i = 0; i < size; ++i)
		{
			//判断是否在等级范围，开服天数范围
			if (limit_unit.level_range_vec[i].size() != border_size || limit_unit.open_server_day_range_vec[i].size() != border_size ||
				role->GetLevel() < limit_unit.level_range_vec[i][left_border] || role->GetLevel() > limit_unit.level_range_vec[i][right_border] ||
				opengame_day < limit_unit.open_server_day_range_vec[i][left_border] || opengame_day > limit_unit.open_server_day_range_vec[i][right_border])
			{
				continue;
			}
			for (size_t j = 0; j < limit_unit.open_day_vec_vec[i].size(); ++j)
			{
				//判断星期几开放
				if (local_tm->tm_wday == limit_unit.open_day_vec_vec[i][j])
				{
					is_open_day = true;
					break;
				}
			}
			if (is_open_day)
			{
				break;
			}
		}
		if (!is_open_day)
		{
			if (limit_unit.is_notice)
			{
				role->NoticeNum(errornum::EN_JOIN_LIMIT_OPEN_DAY_LIMIT);
			}
			return false;
		}
	}

	return true;
}


int JoinLimitConfig::InitFunOpenConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -20000;
	}

	while (!data_element.empty())
	{
		int fun_open_type = 0;
		if (!PugiGetSubNodeValue(data_element, "fun_type", fun_open_type) || fun_open_type >= FUN_OPEN_TYPE_MAX)
		{
			return -1;
		}

		FunOpenUnit &fun_open_unit = m_funopen_cfglist[fun_open_type];

		if (!PugiGetSubNodeValue(data_element, "task_id", fun_open_unit.task_id))
		{
			return -2;
		}
		if (fun_open_unit.task_id > 0)
		{
			if (NULL == TASKPOOL->GetCfgByTaskID(fun_open_unit.task_id))
			{
				return -3;
			}
		}

		if (!PugiGetSubNodeValue(data_element, "role_level", fun_open_unit.role_level) || fun_open_unit.role_level < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "param1", fun_open_unit.param1) || fun_open_unit.param1 < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", fun_open_unit.param2) || fun_open_unit.param2 < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "opengame_day", fun_open_unit.opengame_day) || fun_open_unit.opengame_day < 0)
		{
			return -7;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

