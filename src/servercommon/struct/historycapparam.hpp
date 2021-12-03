
#ifndef __HISTORY_CAP_PARAM_HPP__
#define __HISTORY_CAP_PARAM_HPP__

#pragma pack(push) 
#pragma pack(4)

enum HISTORY_CAPABILITY_TYPE
{
	HISTORY_CAPABILITY_TYPE_TAOTAL = 0,			// ��ս��
	HISTORY_CAPABILITY_TYPE_EQUIPMENT,			// װ������ս����
	HISTORY_CAPABILITY_TYPE_XIANNV,				// ��Ů����ս����
	HISTORY_CAPABILITY_TYPE_MENTALITY,			// Ԫ������ս����
	HISTORY_CAPABILITY_TYPE_MOUNT,				// ��������ս����
	HISTORY_CAPABILITY_TYPE_WING,				// ��������ս����
	HISTORY_CAPABILITY_TYPE_SHENZHUANG,			// ��װ����ս����
	HISTORY_CAPABILITY_TYPE_JINGLING,			// ��������ս����
	HISTORY_CAPABILITY_TYPE_WASH,				// ϴ������ս����
	HISTORY_CAPABILITY_TYPE_FEIXIAN,			// ��������ս����

	HISTORY_CAPABILITY_TYPE_MAX,		
};

struct HistoryCapItem
{
	HistoryCapItem() { this->Reset(); }

	void Reset() { max_cap = 0; reach_timestamp = 0; }

	int max_cap;
	unsigned int reach_timestamp;
};

struct HistoryCapParam
{
	HistoryCapParam() { this->Reset(); }

	void Reset() 
	{
		for (int i = 0; i < HISTORY_CAPABILITY_TYPE_MAX; ++ i)
		{
			history_cap_list[i].Reset();
		}
	}

	HistoryCapItem history_cap_list[HISTORY_CAPABILITY_TYPE_MAX];
};

#pragma pack(pop)

typedef char HistoryCapParamHex[sizeof(HistoryCapParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(HistoryCapParamHex) < 256);

#endif  // __HISTORY_CAP_PARAM_HPP__


