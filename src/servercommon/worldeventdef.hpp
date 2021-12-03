#ifndef __WORLD_EVENT_DEF_HPP__
#define __WORLD_EVENT_DEF_HPP__

#include "servercommon/servercommon.h"

static const int MAX_WORLD_EVENT_SCORE = 2000000000;																	// �����¼��������ֵ 20��
const static int WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT = 1;															// NPC���ڵĳ�����
const static int WORLD_EVENT_NPC_COUNT_PER_SCENE = 24;																	// ÿ���������ڵ�NPC����
const static int WORLD_EVENT_NPC_MAX_COUNT = WORLD_EVENT_NPC_LOCATE_SCENE_MAX_COUNT * WORLD_EVENT_NPC_COUNT_PER_SCENE;	// �����ڵ�NPC����

const static int WORLD_EVENT_NPC_TYPE_MAX_COUNT = 8;																	// NPC������
const static int WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE = 3;																// NPC��������
UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_TYPE_MAX_COUNT <= 16);

const static int WORLD_EVENT_POS_CONFIG_MAX_COUNT_PER_SCENE = 64;														// ÿ�������������������
const static int WORLD_EVENT_POS_CONFIG_MAX_COUNT = 64;																	// �������������

UNSTD_STATIC_CHECK(WORLD_EVENT_NPC_MAX_COUNT == WORLD_EVENT_NPC_TYPE_MAX_COUNT * WORLD_EVENT_NPC_MAX_COUNT_PER_TYPE);

const static int WORLD_EVENT_LOGIC_OPEN_MAX_TIMES_PER_DAY = 16;															// �¼��߼�һ����࿪������
const static int WORLD_EVENT_OBJ_TYPE_MAX_COUNT = 32;																	// �����¼��������������

const static int WORLD_EVENT_RANDOM_BOSS_POS_COUNT = 32;																// ���boss�����32����ˢ��
const static int WORLD_EVENT_RANDOM_BOSS_TYPE_MAX = 8;																	// ���boss�ܹ���8��
const static int WORLD_EVENT_RANDOM_FLUSH_BOSS_MAX = 16;																// ���ͬʱˢ16��boss

const static int WORLD_EVENT_PKROLE_POS_COUNT = 32;																		// PK��������32����ˢ��
const static int WORLD_EVENT_PKROLE_MAX = 16;																			// ���ͬʱˢN��PK����
const static int WORLD_EVENT_PKROLE_MAX_RANKPOS = 12;																	// ����PK��������а�����λ�� e
const static int WORLD_EVENT_PKROLE_PER_FLUSH_NUM = 3;																	// ÿ��ˢ3��

const static int WORLD_EVENT_XUKONGZHIMENG_DOOR_MAX = 12;																// ���ͬʱ��8�����֮��
const static int WORLD_EVENT_XUKONGZHIMENG_MONSTER_MAX = 16;															// ����16��
const static int WORLD_EVENT_XUKONGZHIMENG_DISAPPER_POS_MAX = 4;														// ÿ�����֮�� ��ʧ��������� Ҳ����·������

const static int WORLD_EVENT_QUANMINGTAFANG_TOWER_MAX = 4;																// ���ͬʱ��4����
const static int WORLD_EVENT_QUANMINGTAFANG_MONSTER_MAX = 16;															// ����16��
const static int WORLD_EVENT_QUANMINGTAFANG_START_POS_MAX = 4;															// ÿ���� ��4����ʼ·����

const static int WORLD_EVENT_TITLE_WORD_MAX_NUM = 8;																	// ��Ҫ���������������
UNSTD_STATIC_CHECK(WORLD_EVENT_TITLE_WORD_MAX_NUM <= 8);

const static int WORLD_EVENT_SHENMOZHIXI_PORTAL_MAX_COUNT = 20;															// �����¼���ħ֮϶����������

const static int WORLD_EVENT_TIANNVSHANHUA_MONSTER_COUNT = 4;															// ��Ůɢ�� ��������
const static int WORLD_EVENT_TIANNVSHANHUA_MOVE_POINT_MAX_COUNT = 13;													// ��Ůɢ�� �����ƶ�λ���������
const static int WORLD_EVENT_TIANNVSHANHUA_FALLING_ITEM_MAX_COUNT = 64;													// ��Ůɽ�� ������Ʒ����
const static int WORLD_EVENT_TIANNVSHANHUA_REFRESH_FALLING_ITEM_INTERVAL_S = 20;										// ��Ůɢ�� ɢ�����
const static int WORLD_EVENT_TIANNVSHANHUA_MONSTER_REST_TIME_S = 4;														// ��Ůɢ��	������Ϣʱ��

const static int WORLD_EVENT_REWARD_GRADE_COUNT = 4;																	// �����¼� �ȼ�����

enum WORLD_EVENT_LOGIC_TYPE 
{
	WORLD_EVENT_LOGIC_TYPE_INVALID = 0,						
	WORLD_EVENT_LOGIC_TYPE_STAND ,																						// ׼����ʼ
	WORLD_EVENT_LOGIC_TYPE_START,																						// �߼���ʼ
	WORLD_EVENT_LOGIC_TYPE_STOP,																						// �߼�����
	WORLD_EVENT_LOGIC_TYPE_MAX,		
};

enum WORLD_EVENT_TYPE 
{
	WORLD_EVENT_TYPE_INVALID = 0,

	WORLD_EVENT_TYPE_QUANMINTAFANG,																						// ȫ������
	WORLD_EVENT_TYPE_SHENMOZHIXI,																						// ��ħ֮϶
	WORLD_EVENT_TYPE_XUKONGZHIMEN,																						// ���֮��
	WORLD_EVENT_TYPE_KILL_RANDOM_BOSS,																					// ��ɱ���boss
	WORLD_EVENT_TYPE_FIND_NPC,																							// Ѱ��NPC
	WORLD_EVENT_TYPE_JIQINGPK,																							// ����PK
	WORLD_EVENT_TYPE_TIANNVSANHUA,																						// ��Ůɢ��

	WORLD_EVENT_TYPE_MAX,
};

enum WORLD_EVENT_OBJ_TYPE
{
	WORLD_EVENT_OBJ_INVALID = 0,

	WORLD_EVENT_OBJ_NPC,																								// NPC����
	WORLD_EVENT_OBJ_SHENMOZHIXI,																						// ��ħ֮������
	WORLD_EVENT_OBJ_XUKONGZHIMENG,																						// ʱ��֮�Ŷ���
	WORLD_EVENT_OBJ_TOWER,																								// ������	
	WORLD_EVENT_OBJ_ZHUAGUI_NPC,																						// ץ��NPC	
																														
	WORLD_EVENT_OBJ_MAX,
};

struct WorldEventParam
{
	WorldEventParam() { this->Reset(); }

	void Reset()
	{
		curr_event_type = 0;
		jiqingpk_flush_num = 0;
		reserve_int_1 = 0;
		reserve_int_2 = 0;
		memset(score_list, 0, sizeof(score_list));
	}

	int curr_event_type;																								// ��ǰʱ������
	int jiqingpk_flush_num;																								// ����PK���ǰˢ�˶��ٸ�
	int reserve_int_1;																									// ����
	int reserve_int_2;																									// ����
	int score_list[WORLD_EVENT_TYPE_MAX];																				// ʱ�����
};

#endif // __WORLD_EVENT_DEF_HPP__


