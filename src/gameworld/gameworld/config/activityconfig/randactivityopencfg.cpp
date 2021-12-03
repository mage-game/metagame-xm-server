#include "randactivityopencfg.hpp"
#include "config/sharedconfig/sharedconfig.h"

RandActivityOpenCfg::RandActivityOpenCfg() : m_begin_day_idx(0), m_end_day_idx(0), m_rand_activity_cfg_switch_timestamp(0), m_allow_set_time_dayidx(0), m_upgrade_activity_end_day(0)
{

}

RandActivityOpenCfg::~RandActivityOpenCfg()
{

}

bool RandActivityOpenCfg::Init(const std::string &path, std::string *err)
{
	int iRet = 0;
	char err_info[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, path, "RandActivityOpenCfg", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = path + " xml root node error";
		return false;
	}

	{
		// 杂项配置
		PugiXmlNode root_element = RootElement.child("other");
		if (root_element.empty())
		{
			*err = path + " xml not other node ";
			return false;
		}

		iRet = this->InitOtherCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOtherCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 开启配置
		PugiXmlNode root_element = RootElement.child("open_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not open_cfg node ";
			return false;
		}

		iRet = this->InitOpenCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitOpenCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	{
		// 活动版本开启配置 -- 优先级高于基于开服天数开启
		PugiXmlNode root_element = RootElement.child("version_open_cfg");
		if (root_element.empty())
		{
			*err = path + " xml not version_open_cfg node ";
			return false;
		}

		iRet = this->InitVersionOpenCfg(root_element);
		if (0 != iRet)
		{
			sprintf(err_info, "%s InitVersionOpenCfg fail %d ", path.c_str(), iRet);
			*err = err_info;
			return false;
		}
	}

	return true;
}

int RandActivityOpenCfg::InitOtherCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (!dataElement.empty())
	{
// 		if (!PugiGetSubNodeValue(dataElement, "begin_day_idx", m_begin_day_idx) ||m_begin_day_idx < 0)
// 		{
// 			return -1;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "end_day_idx", m_end_day_idx) || m_end_day_idx <= m_begin_day_idx)
// 		{
// 			return -2;
// 		}
// 
 		if (!PugiGetSubNodeValue(dataElement, "allow_set_time_dayidx", m_allow_set_time_dayidx) || m_allow_set_time_dayidx < 0)
 		{
 			return -3;
 		}
// 
// 		time_t zero_timestamp = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();
// 
// 		m_rand_activity_cfg_switch_timestamp = zero_timestamp + SECOND_PER_DAY * m_end_day_idx;
	}

	return 0;
}

int RandActivityOpenCfg::InitOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -100000;
	}

	while (!dataElement.empty())
	{
		int activity_type = 0;
		if (!PugiGetSubNodeValue(dataElement, "activity_type", activity_type) || activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END)
		{
			return -1;
		}

		RandActivityOpenDetail &cfg_item = m_open_detail_list[activity_type - RAND_ACTIVITY_TYPE_BEGIN];
		
		if (cfg_item.IsValid())
		{
			return -2;
		}

		cfg_item.activity_type = activity_type;

		if (!PugiGetSubNodeValue(dataElement, "begin_day_idx", cfg_item.begin_day_idx))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "end_day_idx", cfg_item.end_day_idx))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "open_type", cfg_item.open_type) || cfg_item.open_type != RAND_ACTIVITY_OPEN_TYPE_NORMAL)
		{
			return -5;
		}
		if (!PugiGetSubNodeValue(dataElement, "send_mail_need_level", cfg_item.send_mail_need_level) || cfg_item.send_mail_need_level < 0)
		{
			return -6;
		}
		//特殊处理，开服投资活动一直持续，但客户端需求在表上填0~7
		if (RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST == activity_type)
		{
			cfg_item.end_day_idx += 9990;
		}

		time_t zero_timestamp = SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime();

		cfg_item.begin_timestamp = static_cast<unsigned int>(zero_timestamp + SECOND_PER_DAY * cfg_item.begin_day_idx);
		cfg_item.end_timestamp = static_cast<unsigned int>(zero_timestamp + SECOND_PER_DAY * cfg_item.end_day_idx);

		if (this->isUpgradeRankActivity(cfg_item.activity_type) && cfg_item.end_day_idx > m_upgrade_activity_end_day)
		{
			m_upgrade_activity_end_day = cfg_item.end_day_idx;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int RandActivityOpenCfg::InitVersionOpenCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return 0; //允许空配置
	}

	while (!dataElement.empty())
	{
		int activity_type = 0;

		if (!PugiGetSubNodeValue(dataElement, "activity_type", activity_type) || activity_type < RAND_ACTIVITY_TYPE_BEGIN || activity_type >= RAND_ACTIVITY_TYPE_END)
		{
			return -1;
		}

		RandActivityOpenDetail &cfg_item = m_open_detail_list[activity_type - RAND_ACTIVITY_TYPE_BEGIN];

		//版本活动中不能有开服活动
		if (cfg_item.IsValid())
		{
			return -2;
		}

		cfg_item.activity_type = activity_type;

		if (!PugiGetSubNodeValue(dataElement, "open_type", cfg_item.open_type) || cfg_item.open_type != RAND_ACTIVITY_OPEN_TYPE_VERSION)
		{
			return -3;
		}

		{		
			PugiXmlNode BeginTimeElement = dataElement.child("begin_date");
			if (BeginTimeElement.empty())
			{
				return -4;
			}

			time_t begin_time = 0;
			if (!ConvertTimeStringToUnixTime(BeginTimeElement.first_child().value(), &begin_time))
			{
				return -5;
			}
			cfg_item.begin_timestamp = static_cast<unsigned int>(begin_time);

			PugiXmlNode EndTimeElement = dataElement.child("end_date");
			if (EndTimeElement.empty())
			{
				return -6;
			}

			time_t end_time = 0;
			if (!ConvertTimeStringToUnixTime(EndTimeElement.first_child().value(), &end_time))
			{
				return -7;
			}
			cfg_item.end_timestamp = static_cast<unsigned int>(end_time);

		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

time_t RandActivityOpenCfg::GetRandActivitySwitchTimeStamp()
{
	return m_rand_activity_cfg_switch_timestamp;
}

const RandActivityOpenDetail * RandActivityOpenCfg::GetOpenCfg(int activity_type)
{
	if (activity_type >= RAND_ACTIVITY_TYPE_BEGIN && activity_type < RAND_ACTIVITY_TYPE_END)
	{
		int index = activity_type - RAND_ACTIVITY_TYPE_BEGIN;
		if (m_open_detail_list[index].IsValid())
		{
			return &m_open_detail_list[index];
		}
	}

	return NULL;
}

bool RandActivityOpenCfg::isUpgradeRankActivity(int ra_type) const
{
	if (ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK || ra_type == RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK
		|| ra_type == RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK || ra_type == RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK)
	{
		return true;
	}

	return false;
}
