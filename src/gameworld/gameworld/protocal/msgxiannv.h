#ifndef MSG_XIANNV_H
#define MSG_XIANNV_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/struct/xiannvparam.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class SCAllXiannvInfo								// ��Ů������Ϣ2201
	{
	public:
		SCAllXiannvInfo();
		MessageHeader header;

		int active_xiannv_flag;
		unsigned char active_huanhua_flag[MAX_XIANNV_ACTIVE_SPECIAL_IMAGE_BYTE];
		short huanhua_id;
		short reserved;
		GameName xiannv_name[MAX_XIANNV_COUNT];
		int xiannv_huanhua_level[MAX_XIANNV_HUANHUA_COUNT];

		XiannvItem xn_item_list[MAX_XIANNV_COUNT];
		char pos_list[MAX_XIANNV_POS];

		int shengwu_lingye;									// ʥ����Һ
		short cur_gold_miling_times;						// ��ǰ��Ԫ�������������ȡ���������㣩
		short shengwu_chou_id;								// δ��ȡ�ĳ齱exp����ʥ��id
		short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// δ��ȡ�ĳ齱exp����
		NvshenShengwuItem all_shengwu_list[XIANNV_SHENGWU_MAX_ID + 1];

		short grid_level_list[XIANNV_SHENGWU_GONGMING_MAX_GRID_ID + 1];
		char day_free_miling_times;							// ��������������
		char day_fetch_ling_time;							// �����ȡ��Һ����

		char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];	// ��ǰ�鵽���������б�
		short reserve_sh;
		int shengwu_essence;						// ������������
	};

	class SCXiannvInfo									// ָ����Ů��Ϣ2200
	{
	public:
		SCXiannvInfo();
		MessageHeader header;

		short notify_reaon;
		short xiannv_id;
		XiannvItem xn_item;
	};

	class SCXiannvViewChange							// ��Ů����ı�㲥2203
	{
	public:
		SCXiannvViewChange();
		MessageHeader header;

		ObjID obj_id;
		short use_xiannv_id;
		short huanhua_id;
		short reserved;
		GameName xiannv_name;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class CSXiannvCall									// ������Ů��ս2250
	{
	public:
		CSXiannvCall();
		MessageHeader header;

		char pos_list[MAX_XIANNV_POS];
	};

	class CSXiannvRename								// ����������2251
	{
	public:
		CSXiannvRename();
		MessageHeader header;

		short xiannv_id;
		GameName new_name;
	};

	class CSXiannvActiveReq								// ���󼤻�2252
	{
	public:
		CSXiannvActiveReq();
		MessageHeader header;

		short xiannv_id;
		short item_index;
	};

	class CSXiannvAddZizhiReq							// ���������2253
	{
	public:
		CSXiannvAddZizhiReq();
		MessageHeader header;

		short xiannv_id;
		short auto_buy;
	};

	class CSXiannvActiveHuanhua							// ���󼤻�û�2265
	{
	public:
		CSXiannvActiveHuanhua();
		MessageHeader header;

		short xiannv_id;
		short item_index;
	};

	class CSXiannvImageReq								//��������2266
	{
	public:
		CSXiannvImageReq();
		MessageHeader header;

		short huanhua_id;
	};

	class CSXiannvHuanHuaUpLevelReq						// ����û�����2267
	{
	public:
		CSXiannvHuanHuaUpLevelReq();
		MessageHeader header;

		short huanhua_id;
		short auto_buy;
	};


	class CSXiannvChangeSkillReq						// 2268 ����ı���Ů����
	{
	public:
		CSXiannvChangeSkillReq();
		MessageHeader header;

		unsigned short xiannv_skill_id;
		unsigned short reserve;
		int xiannv_skill_level;
	};

	class CSXiannvShengwuReq							// 2270 Ů��ʥ������Э��
	{
	public:
		enum ReqType
		{
			NORMAL_CHOU_EXP = 0,						// ��ȡ����-��ͨ��ȡ��param1 �Ƿ��Զ�����param2 �Ƿ��Զ�ѡ����Ƭ��param3 �Ƿ�10����
			PERFECT_CHOU_EXP,							// ��ȡ����-������ȡ��param1 �Ƿ��Զ�����param2 �Ƿ��Զ�ѡ����Ƭ��param3 �Ƿ�10����
			FETCH_EXP,									// ��ȡ����
			UPGRADE_GRID,								// �����������ӣ�param1 ����ID
			CHOU_LING,									// ��Һ��ȡ
			FETCH_LING,									// ��Һ��ȡ��param1 �Ƿ�˫����ȡ��0 ��1 �ǣ�
			UPGRADE_EXP,								// ���ף�param1 ��ʥ��ID
		};

		CSXiannvShengwuReq();
		MessageHeader header;

		short req_type;
		short param1;
		short param2;
		short param3;
	};

	class SCXiannvShengwuChangeInfo						// 2271 Ů��ʥ��仯������Ϣ
	{
	public:
		enum NotifyType
		{
			LINGYE = 0,									// ������Ϣ��param1 ������ʹ������������, param2 ������ʹ��Ԫ�����������param3 ��������ȡ��Һ������param4 ʣ����Һ
			SHENGWU_INFO,								// ʥ����Ϣ��param1 ʥ��ID��param2 ʥ��ȼ���param3 NULL��param4 ʥ�ﾭ��ֵ
			GRID_INFO,									// ������Ϣ��param1 ����ID��param2 ���ӵȼ�
			ESSENCE,									// ʥ�ﾫ����Ϣ, param3Ϊ�仯���� param4 ʥ�ﾫ������
		};

		SCXiannvShengwuChangeInfo();
		MessageHeader header;

		short notify_type;
		short param1;
		short param2;
		short param3;
		int param4;
	};

	class SCXiannvShengwuMilingList						// 2272 Ů��ʥ�������б�
	{
	public:
		SCXiannvShengwuMilingList();
		MessageHeader header;

		char miling_list[XIANNV_SHENGWU_MILING_FONT_COUNT];
		short reserve_sh;
	};

	class SCXiannvShengwuChouExpList					// 2273 Ů��ʥ������ȡ�ľ����б�
	{
	public:
		SCXiannvShengwuChouExpList();
		MessageHeader header;

		char chou_type;									// ���䣨�龭�飩����
		char reserve_ch;
		short shengwu_chou_id;							// δ��ȡ�ĳ齱exp����ʥ��id
		short shengwu_chou_exp_list[XIANNV_SHENGWU_CHOU_EXP_COUNT];		// δ��ȡ�ĳ齱exp����
	};


	class SCXiannvShengwuChouExpResult					// 2274 Ů��ʥ��龭���������ڿͻ��˲�����Ч��
	{
	public:
		SCXiannvShengwuChouExpResult();
		MessageHeader header;

		struct FetchItem
		{
			short shengwu_id;
			short add_exp;
		};

		short is_auto_fetch;								// �Ƿ��Զ���ȡ

		short add_exp_count;								// ��ȡ�����ʥ������
		FetchItem add_exp_list[XIANNV_SHENGWU_MAX_ID + 1];	// ��ȡ�����ʥ���б�
	};
}

#pragma pack(pop)

#endif

