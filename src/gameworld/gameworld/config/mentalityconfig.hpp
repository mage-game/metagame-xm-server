#ifndef __MENTALITY_CONFIG_HPP__
#define __MENTALITY_CONFIG_HPP__

#include <map>

#include "servercommon/mentalitydef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "obj/character/attribute.hpp"

// ���������������Ǻ�Ԫ��
// �°棺�������鹬������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct MentalityBaseItemConfig																				// һ���ȼ��� ����Ԫ������
{
	MentalityBaseItemConfig() : rolelevel_limit(0), need_yuanli(0), cd_time(0) {}

	int rolelevel_limit;																					// ���ƽ�ɫ�ȼ�
	int need_yuanli;																						// ��ҪԪ��
	int cd_time;																							// ��ȴCD

	CharIntAttrs::AttrPair all_attrlist[MENTALITY_TYPE_MAX];												// ����Ԫ������� 
};

struct MentalityGenguItemConfig																				// ��������
{
	MentalityGenguItemConfig() : protect_level(0), succ_rate(0), need_coin(0), upgrade_itemid(0), protect_item_id(0), protect_item_num(0) {}

	int protect_level;																						// �����ȼ�
	int succ_rate;																							// �����ǳɹ�����
	int need_coin;																							// ����������ͭ��
	ItemID upgrade_itemid;																					// ����������

	ItemID protect_item_id;
	int protect_item_num;

	CharIntAttrs::AttrPair all_attrlist[MENTALITY_TYPE_MAX];												// ���и��ǵ����� ����ͬԪ������
};

struct MentalityOtherConfig 
{
	MentalityOtherConfig() : clear_cd_cost(0) {}

	int clear_cd_cost;																						// ���CD����
};

const static int MENTALITY_WUXING_MAX_LEVEL = 12;															// ������ߵȼ�

struct MentalityWuxingConfig
{
	MentalityWuxingConfig() : wuxing_index(0), wuxing_level(0), attr_val(0), need_total_mentality_level(0), uplevel_stuff_id(0),
		uplevel_stuff_num(0), succ_rate(0), reduce_rate(0) {}

	int wuxing_index;
	int wuxing_level;
	int attr_val;
	int need_total_mentality_level;
	int uplevel_stuff_id;
	int uplevel_stuff_num;
	int succ_rate;
	int reduce_rate;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MentalityConfig : public ConfigBase
{
public:
	MentalityConfig();
	~MentalityConfig();

	bool Init(const std::string &path, std::string *err);													// ��ȡ�����ļ�

	const MentalityBaseItemConfig * GetTrainMentalityBaseItem(int mentality_level);							// ��ȡԪ����������
	const MentalityGenguItemConfig * GetUpgradeMentalityGenguItem(int gengu_level);							// ��ȡ������������		

	void GetMentalityBaseAttr(int mentality_type, int mentality_level, CharIntAttrs &base_add, int prof) const;				// �����ض�Ԫ��ȼ��ļӳ�����

	void GetGenguBaseAttr(int gengu_type,int gengu_level, CharIntAttrs &base_add, int prof) const;								//�����ض��ĸ��ǵȼ��ӳ�����

	int GetMentalityCDIntervalPerGold() { return CLEAR_CD_INTERVAL_PER_GOLD; }								// ��ȡ��ԪԪ������Ԫ��������ȴʱ��
	
	bool IsGenguLevelBoardcast(int gengu_level);															// �жϸ��ǵȼ��Ƿ�㲥
	
	void GetMentalityTitleAttr(CharIntAttrs &base_add, int least_mentality_level, int prof);							// ����Ԫ��ƺżӳ�����
	void GetGenguTitleAttr(CharIntAttrs &base_add, int least_gengu_level, int prof);									// ���Ӹ��ǳƺżӳ�����

	const MentalityOtherConfig & GetOtherCfg() { return m_other_cfg; }										// ��������

	const MentalityWuxingConfig * GetWuxingCfg(int wx_index, int wx_level);									// ��������

private:
	struct MentalityTitleConfig																				// Ԫ��ƺŽ���
	{
		MentalityTitleConfig() : least_mentality_level(0) {}

		int least_mentality_level;	
		CharIntAttrs::AttrPair mentality_attrlist[MENTALITY_TYPE_MAX];
	};

	struct GenguTitleConfig																					// ���ǳƺ�
	{
		GenguTitleConfig() : least_gengu_level(0) {}

		int least_gengu_level;
		CharIntAttrs::AttrPair gengu_attrlist[MENTALITY_TYPE_MAX];
	};


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	MentalityBaseItemConfig m_mentality_base_config_list[MAX_MENTALITY_BASE_LEVEL + 1];						// 0-160��Ԫ������
	MentalityGenguItemConfig m_mentality_gengu_config_list[MAX_MENTALITY_GENGU_LEVEL + 1];					// 0-30�����������趨 

	int m_mentality_title_count;																			// Ԫ��ƺŽ����б���
	MentalityTitleConfig m_mentality_title[MAX_MENTATITY_TITLE_COUNT_MAX];									// Ԫ��ƺŽ����б�

	int m_gengu_title_count;																				// ���ǳƺŽ����б���
	GenguTitleConfig m_gengu_title[MAX_GENGU_TITLE_COUNT_MAX];												// ���ǳƺŽ����б�

	MentalityOtherConfig m_other_cfg;																		// ��������

	MentalityWuxingConfig m_wuxing_cfglist[MENTALITY_WUXING_MAX_COUNT][MENTALITY_WUXING_MAX_LEVEL + 1];		// ��������

};

#endif // __MENTALITY_CONFIG_HPP__

