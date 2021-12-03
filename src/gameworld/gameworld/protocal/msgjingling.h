#ifndef MSG_JINGLING_H
#define MSG_JINGLING_H

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/jinglingdef.hpp"
#include "gameworld/other/jinglingadvantage/jinglingadvantage.hpp"

#pragma pack(push, 4) 

namespace Protocol
{
	enum JINGLING_OPER_TYPE
	{
		JINGLING_OPER_TAKEOFF = 0,			// ȡ��
		JINGLING_OPER_CALLBACK,				// �ٻ�
		JINGLING_OPER_FIGHTOUT,				// ��ս
		JINGLING_OPER_UPLEVEL,				// ����
		JINGLING_OPER_UPLEVELCARD,			// ��������
		JINGLING_OPER_RENAME,				// ���ľ�������
		JINGLING_OPER_UPGRADE,				// ����
		JINGLING_OPER_STRENGTH,				// ǿ��װ��
		JINGLING_OPER_USEIMAGE,				// ʹ������
		JINGLING_OPER_USEPHANTOM_IMG,		// ʹ�ûû�����
		JINGLING_OPER_PHANTOMUPLEVEL,		// �û���������
		JINGLING_OPER_UPGRADESOUL,			// ��������
		JINGLING_OPER_UPGRADE_HALO,			// ����⻷
		JINGLING_OPER_USE_HALO_IMG,			// ʹ�þ���⻷
		JINGLING_OPER_ONEKEY_RECYCL_BAG,    // һ�����վ��鱳���ľ���
		JINGLING_OPER_UPLEVEL_WUXING,		// ��������
		JINGLING_OPER_UPLEVEL_XIANZHEN,		// ��������
		JINGLING_OPER_UPLEVEL_HUNYU,		// ��������
		JINGLING_OPER_REMOVE_SKILL,			// ���� ����
		JINGLING_OPER_CHANGE_MOVE,			// ���� ��ɿ��ƶ�
		JINGLING_OPER_PUT_ON_SKILL,			// ���� ����
		JINGLING_OPER_TAKE_OFF_SKILL,		// ���� ����
		JINGLING_OPER_LEARN_SKILL,			// ���� ѧϰ
		JINGLING_OPER_REFRESH,				// ���� ˢ��
		JINGLING_OPER_GET,					// ���� ��ȡ
		JINGLING_OPER_REFRESH_ACTIVE,		// ���� ˢ�¼���
		JINGLING_OPER_OPEN_SKILL_SLOT,      // ���ܲۿ���
	};

	class SCJingLingInfo					// ������Ϣ5671
	{
	public:
		SCJingLingInfo();
		MessageHeader header;

		JingLingCardParamItem card_list[JINGLING_CARD_MAX_TYPE];
 		int shuxingdan_list[JINGLING_SHUXINGDAN_MAX_TYPE];
 		short equip_strength_list[JINGLING_EQUIP_MAX_PART];

		struct InfoType 
		{
			int	index;
			ItemDataWrapper	item_wrapper;
		};

		GameName jingling_name;
		ItemID use_jingling_id;
		short use_imageid;
		int m_active_image_flag;
 		int grade;
 		int grade_bless;

		short phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];
		unsigned char phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];
		int phantom_imageid;
		
 		short soul_level_list[JINGLING_CARD_MAX_TYPE];
 		int taozhuang_level;

 		int halo_active_image_flag;
 		short halo_level;
 		short use_halo_img;
 		int halo_bless_val;

		int xianzhen_level;
		int xianzhen_exp;
		int xianzhen_up_count;
		int hunyu_level_list[XIAN_ZHEN_HUN_YU_TYPE_MAX];
		JingLingSkillInfo skill_storage_list[JING_LING_SKILL_STORAGE_MAX];
		JingLingRefreshItem skill_refresh_item_list[JING_LING_SKILL_REFRESH_ITEM_MAX];

		int count;
		InfoType jingling_list[JINGLING_MAX_TAKEON_NUM];
	};
	
	class CSJingLingOper					// 5670�����������
	{
	public:
		CSJingLingOper();
		MessageHeader header;

		short oper_type;
		short param1;
		short param2;
		short param3;
		int param4;
		
		GameName jingling_name;
	};

	class SCJingLingViewChange				// 5672��������ı�㲥
	{
	public:
		SCJingLingViewChange();
		MessageHeader header;

		ObjID obj_id;
		ItemID jingling_id;
		short level;
		short use_imageid;
		GameName jingling_name;
		int jingling_phantom_img;
		int jingling_cur_use_halo_img;
	};

	enum JING_LING_HOME_OPER_TYPE
	{
		JING_LING_HOME_OPER_TYPE_GET_INFO = 0,		// ��ѯ��Ϣ
		JING_LING_HOME_OPER_TYPE_PUT_HOME,			// �����԰
		JING_LING_HOME_OPER_TYPE_QUICK,				// �ӿ��ٶ�
		JING_LING_HOME_OPER_TYPE_GET_REWARD,		// ��ȡ����
		JING_LING_HOME_OPER_TYPE_ROB,				// �Ӷ�
		JING_LING_HOME_OPER_TYPE_OUT,				// ȡ��
		JING_LING_HOME_OPER_TYPE_REFRESH_LIST,		// ˢ��
		JING_LING_HOME_OPER_TYPE_READ_ROB_RECORD,	// �Ķ����Ӷ��¼
	};

	class CSJingLingHomeOperReq
	{
	public:
		CSJingLingHomeOperReq();
		MessageHeader header;

		int oper_type;
		int role_id;
		short param1;
		short param2;
	};

	enum JING_LING_HOME_REASON
	{
		JING_LING_HOME_REASON_DEF = 0,
		JING_LING_HOME_REASON_PUT,
		JING_LING_HOME_REASON_QUICK,
		JING_LING_HOME_REASON_GET_REWARD,
		JING_LING_HOME_REASON_ROB_WIN,
		JING_LING_HOME_REASON_ROB_LOST,
		JING_LING_HOME_REASON_TAKE_OUT,
	};
	class SCJingLingHomeInfo
	{
	public:
		SCJingLingHomeInfo();
		MessageHeader header;

		short reason;
		short rob_times_of_me;
		int role_id;
		GameName name;
		JingLingHomeItem item_list[JINGLING_MAX_TAKEON_NUM];
	};

	class SCJingLingHomeListInfo
	{
	public:
		SCJingLingHomeListInfo();
		MessageHeader header;

		static const int LIST_MAX = 10;
		struct HomeInfo
		{
			int role_id;
			char prof;
			char sex;
			char camp;
			char vip_level;
			long long avatar_timestamp;
			GameName name;
		};

		int info_count;
		HomeInfo info_list[LIST_MAX];
	};

	class SCJingLingHomeRobRecord
	{
	public:
		SCJingLingHomeRobRecord();
		MessageHeader header;

		struct RobRecord
		{
			int role_id;
			GameName name;
			unsigned int rob_time;
		};

		unsigned int read_rob_record_time;
		int record_count;
		RobRecord rob_record_list[JING_LING_HOME_ROB_RECORD_MAX];
	};

	class SCJingLingHomeRobAck
	{
	public:
		SCJingLingHomeRobAck();
		MessageHeader header;

		struct RobItem
		{
			ItemID item_id;
			short item_num;
		};

		int role_id;
		int rob_lingjing;
		int rob_hunli;
		short is_win;
		short item_count;
		RobItem item_list[JING_LING_HOME_REWARD_ITEM_MAX];
	};

	enum JL_EXPLORE_OPER_TYPE
	{
		JL_EXPLORE_OPER_TYPE_SELECT_MODE = 0,
		JL_EXPLORE_OPER_TYPE_EXPLORE,
		JL_EXPLORE_OPER_TYPE_FETCH,
		JL_EXPLORE_OPER_TYPE_RESET,
		JL_EXPLORE_OPER_TYPE_BUY_BUFF,
	};
	class CSJinglIngExploreOperReq
	{
	public:
		CSJinglIngExploreOperReq();
		MessageHeader header;

		int oper_type;
		short param1;
		short param2;
	};

	enum JL_EXPLORE_INFO_REASON
	{
		JL_EXPLORE_INFO_REASON_DEF = 0,
		JL_EXPLORE_INFO_REASON_SELECT,
		JL_EXPLORE_INFO_REASON_CHALLENGE_SUCC,
		JL_EXPLORE_INFO_REASON_CHALLENGE_FAIL,
		JL_EXPLORE_INFO_REASON_FETCH,
		JL_EXPLORE_INFO_REASON_RESET,
		JL_EXPLORE_INFO_REASON_BUY_BUFF,
	};
	class SCJinglIngExploreInfo
	{
	public:
		SCJinglIngExploreInfo();
		MessageHeader header;

		short reason;
		short explore_mode;
		int explore_maxhp;
		int explore_hp;
		int buy_buff_count;
		JingLingExploreInfo explore_info_list[JING_LING_EXPLORE_LEVEL_COUNT];
	};

	class SCJingLingAdvantageInfo					//5606 ��������������Ϣ
	{
	public:
		SCJingLingAdvantageInfo();
		MessageHeader header;

		struct GatherPosInfo
		{
			int scene_id;
			short purple_count;
			short blue_count;
		};

		static const int SCENE_COUNT = 9;
		GatherPosInfo pos_list[SCENE_COUNT];
	};

	class SCJingLingAdvantageCount					//5607 ����������ɫ��������
	{
	public:
		SCJingLingAdvantageCount();
		MessageHeader header;

		int today_gather_blue_jingling_count;		//�����õ���ɫ��������
	};

	class SCJingLingAdvantageBossInfo				//5608 ��������bossˢ����Ϣ
	{
	public:
		SCJingLingAdvantageBossInfo();
		MessageHeader header;

		unsigned short monster_id;					//boss_id
		short reserve_sh;
		GameName role_name;
		short scene_id;
		short reserve_1;
	};

	enum JINGLING_ADCANTAGE_OPER_TYPE
	{
		JINGLING_ADCANTAGE_OPER_TYPE_BOSS = 0,        // ����boss   monster_id Ϊbossid
		JINGLING_ADCANTAGE_OPER_TYPE_EGG,             // �ɳ走λ�� monster_id Ϊ����id
	};

	class CSJingLingAdvantageBossEnter				//5609 ������������boss
	{
	public:
		CSJingLingAdvantageBossEnter();
		MessageHeader header;

		int oper_type;
		int monster_id;								//����id
	};

	class SCJingLingEggPosInfo          // 5610   ���鵰λ����Ϣ
	{
	public:
		SCJingLingEggPosInfo();
		MessageHeader header;

		int scene_id;
		int pos_x;
		int pos_y;
	};
}

#pragma pack(pop)

#endif

