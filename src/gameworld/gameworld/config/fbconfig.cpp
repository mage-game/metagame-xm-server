#include "fbconfig.hpp"
#include "servercommon/configcommon.h"
#include "checkresourcecenter.hpp"

FBConfig::FBConfig()
{
}

FBConfig::~FBConfig()
{
}

bool FBConfig::Init(const std::string &configname, std::string *err)
{
	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, configname, "FBConfig", *err);

	PugiXmlNode RootElement = document.document_element();
	if (RootElement.empty())
	{
		*err = configname + ": xml node error in root";
		return false;
	}
	
	PugiXmlNode FBElement = RootElement.child("fb");
	while (!FBElement.empty())
	{
		int daycount_id = 0;
		if (!PugiGetSubNodeValue(FBElement, "daycount_id", daycount_id) || daycount_id < 0 || daycount_id >= MAX_FB_NUM)
		{
			*err = configname + ": xml node error in [fb->daycount_id]";
			return false;
		}

		FBConfigItem exist_fb_item = m_fb_list[daycount_id];

		m_fb_list[daycount_id].daycount_id = daycount_id;

		int total_count = 0;
		if (!PugiGetSubNodeValue(FBElement, "total_count", total_count) || total_count < 0)
		{
			*err = configname + ": xml node error in [fb->total_count]";
			return false;
		}

		if (!PugiGetSubNodeValue(FBElement, "reset_count", m_fb_list[daycount_id].max_reset_count) || m_fb_list[daycount_id].max_reset_count < 0 || m_fb_list[daycount_id].max_reset_count > total_count)
		{
			*err = configname + ": xml node error in [fb->reset_count]";
			return false;
		}

		m_fb_list[daycount_id].max_free_count = total_count - m_fb_list[daycount_id].max_reset_count;

		if (!exist_fb_item.Invalid() && exist_fb_item.max_free_count != m_fb_list[daycount_id].max_free_count)
		{
			*err = configname + ": xml node error in [fb->total_count], must be same value for one daycount_id!";
			return false;
		}

		if (!exist_fb_item.Invalid() && exist_fb_item.max_reset_count != m_fb_list[daycount_id].max_reset_count)
		{
			*err = configname + ": xml node error in [fb->max_reset_count], must be same value for one daycount_id!";
			return false;
		}

		int reset_cost_count = 0;

		{
			PugiXmlNode CostElement = FBElement.child("reset_cost");
			if (CostElement.empty())
			{
				*err = configname + ": xml node error in [fb->reset_cost]";
				return false;
			}

			PugiXmlNode GoldElement = CostElement.child("gold");
			while (!GoldElement.empty())
			{
				if (reset_cost_count >= m_fb_list[daycount_id].max_reset_count)
				{
					*err = configname + ": xml node error in [fb->reset_cost->gold], too many";
					return false;
				}

				if (!PugiGetNodeValue(GoldElement, m_fb_list[daycount_id].reset_gold_list[reset_cost_count]) || m_fb_list[daycount_id].reset_gold_list[reset_cost_count] < 0)
				{
					*err = configname + ": xml node error in [fb->reset_cost->gold]";
					return false;
				}

				if (!exist_fb_item.Invalid() && exist_fb_item.reset_gold_list[reset_cost_count] != m_fb_list[daycount_id].reset_gold_list[reset_cost_count])
				{
					*err = configname + ": xml node error in [fb->reset_cost->gold], must be same value for one daycount_id!";
					return false;
				}

				++ reset_cost_count;
				GoldElement = GoldElement.next_sibling();
			}

			if (0 != reset_cost_count && reset_cost_count != m_fb_list[daycount_id].max_reset_count)
			{
				*err = configname + ": xml node error in [fb->reset_cost], too few";
				return false;
			}
		}

		int reset_coin_count = 0;

		{
			PugiXmlNode reset_coin_element = FBElement.child("reset_coin");
			if (reset_coin_element.empty())
			{
				*err = configname + ": xml node error in [fb->reset_coin]";
				return false;
			}

			PugiXmlNode coin_element = reset_coin_element.child("coin");
			while (!coin_element.empty())
			{
				if (reset_coin_count >= m_fb_list[daycount_id].max_reset_count)
				{
					*err = configname + ": xml node error in [fb->reset_coin->coin], too many";
					return false;
				}

				if (!PugiGetNodeValue(coin_element, m_fb_list[daycount_id].reset_coin_list[reset_coin_count]) || m_fb_list[daycount_id].reset_coin_list[reset_coin_count] < 0)
				{
					*err = configname + ": xml node error in [fb->reset_coin->coin]";
					return false;
				}

				if (0 == m_fb_list[daycount_id].reset_coin_list[reset_coin_count] && 0 == m_fb_list[daycount_id].reset_gold_list[reset_cost_count])
				{
					*err = configname + ": xml error reset_cost and reset_coin can't all to 0";
					return false;
				}

				if (!exist_fb_item.Invalid() && exist_fb_item.reset_coin_list[reset_coin_count] != m_fb_list[daycount_id].reset_coin_list[reset_coin_count])
				{
					*err = configname + ": xml node error in [fb->reset_coin->coin], must be same value for one daycount_id!";
					return false;
				}

				++ reset_coin_count;
				coin_element = coin_element.next_sibling();
			}

			if (0 != reset_coin_count && reset_coin_count != m_fb_list[daycount_id].max_reset_count)
			{
				*err = configname + ": xml node error in [fb->reset_coin], too few";
				return false;
			}
		}

		if (reset_cost_count != m_fb_list[daycount_id].max_reset_count && reset_coin_count != m_fb_list[daycount_id].max_reset_count)
		{
			*err = configname + ": xml error reset_cost or reset_coin too few";
			return false;
		}

		FBElement = FBElement.next_sibling("fb");
	}
	
	return true;
}

int FBConfig::GetFBDayCount(int daycount_id, int role_level, int *max_reset_count) const
{
	if (daycount_id < 0 || daycount_id >= MAX_FB_NUM || m_fb_list[daycount_id].Invalid()) return 0;

	if (NULL != max_reset_count) *max_reset_count = m_fb_list[daycount_id].max_reset_count;
	
	return m_fb_list[daycount_id].max_free_count;
}

bool FBConfig::GetFBDayCountResetCost(int daycount_id, int role_level, int reset_num, int *need_gold, int *need_coin) const
{
	if (NULL == need_gold || NULL == need_coin || 
		daycount_id < 0 || daycount_id >= MAX_FB_NUM || m_fb_list[daycount_id].Invalid() ||
		reset_num < 0 || reset_num >= m_fb_list[daycount_id].max_reset_count)
	{
		return false;
	}

	*need_gold = m_fb_list[daycount_id].reset_gold_list[reset_num];
	*need_coin = m_fb_list[daycount_id].reset_coin_list[reset_num];

	return true;
}

int FBConfig::GetFBDayCountIDBySceneId(int scene_id)
{
	SceneDayCounterIdMapIt it = m_scene_daycount_map.find(scene_id);
	if (it == m_scene_daycount_map.end()) return -1;
	
	return it->second;
}

bool FBConfig::IsDaycountIDExist(int daycount_id)
{
	if (daycount_id < 0 || daycount_id >= MAX_FB_NUM || m_fb_list[daycount_id].Invalid()) return false;

	return true;
}

