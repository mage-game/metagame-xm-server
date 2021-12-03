#ifndef __MAGICAL_PRECIOUS_CONFIG_HPP__
#define __MAGICAL_PRECIOUS_CONFIG_HPP__

#include "servercommon/fbdef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "gamedef.h"
#include "servercommon/configbase.h"
#include "servercommon/magicalpreciousdef.hpp"
#include "servercommon/pugixml/pugixml_adapter.hpp"


static const int MAGICAL_PRECIOUS_MAX_CHAPTER = MAGICAL_PRECIOUS_CHAPTER_COUNT;
static const int MAGICAL_PRECIOUS_MAX_REWARD_INDEX = MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT;
//static const int MAGICAL_PRECIOUS_MAX_REWARD_INDEX = MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT;

enum CHAPTER_CONDITION_TYPE
{
	CONDITION_TYPE_INVALID = 0,

	CONDITION_TYPE_TOTAL_GONGJI,					// �������ԴﵽX
	CONDITION_TYPE_TOTAL_FANGYU,					// �������ԴﵽX
	CONDITION_TYPE_TOTAL_MAXHP,						// �������ԴﵽX
	CONDITION_TYPE_TOTAL_SHANBI,					// �������ԴﵽX
	CONDITION_TYPE_TOTAL_MINGZHONG,					// �������ԴﵽX
	CONDITION_TYPE_TOTAL_BAOJI,						// �������ԴﵽX
	CONDITION_TYPE_TOTAL_JIANREN,					// �������ԴﵽX

	CONDITION_TYPE_MOUNT_GRADE = 11,				// ��������ﵽX
	CONDITION_TYPE_WING_GRADE,						// ��������ﵽX
	CONDITION_TYPE_HALO_GRADE,						// �⻷�����ﵽX
	CONDITION_TYPE_SHENGONG_GRADE,					// ��Ů�⻷�����ﵽX
	CONDITION_TYPE_SHENYI_GRADE,					// ��Ů��������ﵽX
	CONDITION_TYPE_FIGHT_MOUNT_GRADE,				// ս����������ﵽX
	CONDITION_TYPE_SHIZHUANG_GRADE,					// ʱװ�����ﵽX
	CONDITION_TYPE_SHENBING_GRADE,					// ��������ﵽX
	CONDITION_TYPE_FABAO_GRADE,						// ���������ﵽX
	CONDITION_TYPE_FOOTPRINT_GRADE,					// �㼣�����ﵽX
	CONDITION_TYPE_JINEJIE_LEVEL = 21,				// ��������ﵽX
	CONDITION_TYPE_MOUNT_STAR,						// �����Ǽ��ﵽX
	CONDITION_TYPE_WING_STAR,						// �����Ǽ��ﵽX
	CONDITION_TYPE_HALO_STAR,						// �⻷�Ǽ��ﵽX
	CONDITION_TYPE_SHENGONG_STAR,					// ��Ů�⻷�Ǽ��ﵽX
	CONDITION_TYPE_SHENYI_STAR,						// ��Ů�����Ǽ��ﵽX
	CONDITION_TYPE_FIGHT_MOUNT_STAR,				// ս�������Ǽ��ﵽX

	CONDITION_TYPE_ALL_CAPACITY = 31,				// �ۺ�ս���ﵽX
	CONDITION_TYPE_RUNESYSTER_CAPACITY,				// ����ϵͳս���ﵽX
	CONDITION_TYPE_SHENGE_CAPACITY,					// ���ս���ﵽX
	CONDITION_TYPE_SHENZHOU_WEAPON_CAPACITY,		// ��������ս���ﵽX
	CONDITION_TYPE_CHINESE_ZODIAC_CAPACITY,			// ����ս���ﵽX
	CONDITION_TYPE_JINGLING_CAPACITY,				// ����ս���ﵽX
	CONDITION_TYPE_JINGLING_CHENGZHANG,				// ����ɳ��ﵽX
	CONDITION_TYPE_JINGLING_WUXING,					// �������ԴﵽX
	CONDITION_TYPE_XIANNV_SHENWU_CAP,				// ��Ů����ս���ﵽX

	CONDITION_TYPE_EQUIP_STRENGTHEN = 41,			// װ��ǿ���ܵȼ��ﵽX
	CONDITION_TYPE_EQUIP_STAR,						// װ���Ǽ��ܵȼ��ﵽX
	CONDITION_TYPE_EQUIP_STONE,						// װ����ʯ�ܵȼ��ﵽX
	CONDITION_TYPE_ROLE_LEVEL,						// ��ɫ�ȼ��ﵽX
	CONDITION_TYPE_XIANNV_ZIZHI_LEVEL,				// ��Ů���ʵȼ��ﵽX
	CONDITION_TYPE_RUNETOWER_LEVEL,					// �����������ﵽX
	CONDITION_TYPE_PATA_FB_LEVEL,					// �������������ﵽX
	CONDITION_TYPE_TEAMEQUIP_FB_LEVEL,				// ����������������ﵽX
	CONDITION_TYPE_TUITU_FB_NORMAL,					// ��ͨ��ͼ���ﵽx��x��
	CONDITION_TYPE_TUITU_FB_HARD,					// ��Ӣ��ͼ���ﵽx��x��
	CONDITION_TYPE_ARMORDEFEND_FB_LEVEL = 51,		// ���߲��ϱ��ﵽx��
	CONDITION_TYPE_TOWERDEFEND_FB_LEVEL,			// �����������ﵽx��
	CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE,		// ����ػ���ͨ��x��
	CONDITION_TYPE_CHALLENGE_RANK,					// �������ﵽǰx��
	CONDITION_TYPE_NEWEQUIP_FB_LEVEL,				// �������ﵽx��

	CONDITION_TYPE_GUILD_TASK = 61,					// ��ɹ��������ܸ���X
	CONDITION_TYPE_DAILY_TASK,						// ����ճ������ܸ���X
	CONDITION_TYPE_WABAO,							// �ڱ��ܴ���X
	CONDITION_TYPE_OPEN_GUILD_BOX,					// �������ᱦ���ܴ���X
	CONDITION_TYPE_HOTSPRING_JOIN,					// ��������Ȫ�ܴ���X
	CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT,			// �����Ȫ������ȷ�ܴ���X
	CONDITION_TYPE_MILLIONAIRE_BOX,					// �󸻺��ɼ������ܴ���X
	CONDITION_TYPE_HUSONG,							// �����ܴ���X
	CONDITION_TYPE_ENTER_EXP_FB,					// ���뾭�鸱���ܴ���X
	CONDITION_TYPE_PASS_PHASE_FB,					// ͨ�ؽ׶θ����ܴ���X
	CONDITION_TYPE_WANGLING_TASK = 71,				// �������̽�������ܴ���X
	CONDITION_TYPE_TIANJIANGCAIBAO_TASK,			// ����콵�Ʊ������ܴ���X
	CONDITION_TYPE_SHUIJING_TASK,					// ���ˮ���þ������ܴ���X
	CONDITION_TYPE_SHUIJING_GATHER,					// ˮ���þ��ɼ�ˮ���ܴ���X
	CONDITION_TYPE_GONGCHENG_ZHAN,					// �μӹ���ս�ܴ���X
	CONDITION_TYPE_KILL_MIKU_BOSS,					// ��ɱ��Ӣboss�ܴ���X
	CONDITION_TYPE_KILL_WORLD_BOSS,					// ��ɱ����boss�ܴ���X
	CONDITION_TYPE_KILL_ACTIVE_BOSS,				// ��ɱ��Ծboss�ܴ���X
	CONDITION_TYPE_KILL_DABAO_BOSS,					// ��ɱ��boss�ܴ���X
	CONDITION_TYPE_KILL_VIP_BOSS,					// ��ɱVIPboss�ܴ���X

	CONDITION_TYPE_KILL_VIP_MIKU_BOSS1 = 81,		// ��ɱVIP + ��ӢbossX�ȼ��ܴ���Y
	CONDITION_TYPE_KILL_VIP_MIKU_BOSS2,				// ��ɱVIP + ��ӢbossX�ȼ��ܴ���Y
	CONDITION_TYPE_KILL_VIP_MIKU_BOSS3,				// ��ɱVIP + ��ӢbossX�ȼ��ܴ���Y
	CONDITION_TYPE_KILL_ROLE_IN_FIGHTING,			// ��ɱ�������и�������X
	CONDITION_TYPE_MINE_TIMES,						// �ڿ�X��
	CONDITION_TYPE_FISH_NUM,						// ��������X��
	CONDITION_TYPE_SAILING_NUM,						// ����X��
	CONDITION_TYPE_ONLINE_TIME,						// ����XСʱ
	CONDITION_TYPE_KILL_THREE_TYPE_BOSS,			// ��ɱ��ԾBOSS������BOSS����ӢBOSS������X
	CONDITION_TYPE_KILL_MONSTER_NUM,				// ��ɱ����Xֻ
	CONDITION_TYPE_ACTIVE_DEGREE,					// ��Ծ�ȵ���X
	CONDITION_TYPE_MARRY_SUCC,						// �ɹ����
	CONDITION_TYPE_MARRY_SUCC_TYPE,					// �ɹ����X����
	CONDITION_TYPE_PROF_LEVEL,						// �ﵽתְXת
	CONDITION_TYPE_GONGCHENG_ZHAN_WINER,			// ���ڰ��ɳ�Ϊ����ս����

	ZHUANGBEI_CONDITION_TYPE = 100,					// װ��X�׵�X��ɫX��λװ��
	STONE_CONDITION_TYPE,							// ����һ��X�ȼ�X���Եı�ʯ
	DUANZAO_CONDITION_TYPE,							// ����X�׵�X��λ��װ
	CONDITION_TYPE_WEAR_EQUIPMENT,					// װ��X��Y��Z����ȼ�װ��
	CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT,			// װ��X��Y��ZƷ��תְװ��
	CONDITION_TYPE_WEAR_MARRY_EQUIPMENT,			// ����X��Y��ZƷ�ʽ��װ��
	CONDITION_TYPE_COLLECT_JINGLING,				// �ɳ�װ��������X��YƷ�������ɳ�
	CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG,		// ����X����������Y����װ
	CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON,			// ����X�����
	CONDITION_TYPE_XIANNV_GONGMING_LEVEL,			// ��Ů���--�����ܵȼ��ﵽX��
	CONDITION_TYPE_XIANNV_SHENGWU_LEVEL,			// ��Ů���--�����ܵȼ��ﵽX��

	CONDITION_TYPE_LIEMING_COUNT = 111,				// ��ǶX��YƷ������
	CONDITION_TYPE_SHENGE_COUNT,					// ��ǶX��YƷ�����
	CONDITION_TYPE_SHENSHOU_COUNT,					// ����X������
	CONDITION_TYPE_CHINESE_ZODIAC_LEVEL,			// ��Ф�ܵȼ��ﵽX��
	CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL,			// ʥ����ǿ���ȼ��ﵽX��
	CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT,			// ����תְX��Y��װ
	CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2,			// ����X������
	CONDITION_TYPE_ACTIVE_XIANNV,					// ����X��Y���ʵȼ�����Ů
	CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL,		// ��ʯ�ܵȼ��ﵽX��
	CONDITION_TYPE_TOTAL_SHENGWANG,					// �ۼƻ��X������

	CONDITION_TYPE_ACTIVE_LUOSHU = 121,				// �ռ�X��YƷ���鿨��
	CONDITION_TYPE_EQUIP_BAPTIZE,					// ϴ�����X��YƷ�ʼ���������
	CONDITION_TYPE_GREATESOLDIER_ACTIVE,			// ����X��YƷ�ʼ���������
	CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT,		// ��ǶX��YƷ�ʻ�ʯ
	CONDITION_TYPE_SHENSHOU_EQUIP_COUNT,			// ����X��YƷ������װ��
	CONDITION_TYPE_UPGRADE_SYS_GRADE,				// X����ϵͳ�����ﵽY
	CONDITION_TYPE_SHENQI_INLAY_COUNT,				// ����X��YƷ�������ݼ�
	CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO,			// ����X��,��Ե�⻷Y
	CONDITION_TYPE_GREATESOLDIER_UPLEVEL,			// ����X����������Ǳ�ܵ�Y��
	CONDITION_TYPE_SHENGQI_LEVEL_COUNT,				// X��ʥ��ǿ���ȼ��ﵽY��
	CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT,		// X����Ф�ȼ��ﵽY��
	CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT,		// ����X��YƷ����Ф
	CONDITION_TYPE_SEAL_INLAY_COUNT,				// ����X��YƷ������ʥӡ
	CONDITION_TYPE_COLLECT_JINGLING_SKILL,			// װ��X��YƷ�������ɳ輼��
	CONDITION_TYPE_QILINGBI_GRADE,					// ����۽����ﵽX
	CONDITION_TYPE_MASK_GRADE,						// ��߽����ﵽX
	CONDITION_TYPE_TOUSHI_GRADE,					// ͷ�ν����ﵽX
	CONDITION_TYPE_YAOSHI_GRADE,					// ���ν����ﵽX
	CONDITION_TYPE_ACTIVE_QINGYUAN_HALO,			// ����X��ɫ������Ե�⻷Y��

	MAX_CONDITION_TYPE,
};

struct MagicalPreciousOtherCfg
{
	MagicalPreciousOtherCfg(): open_level(0)
	{}

	int open_level;
};

struct MagicalPreciousChapterCfg
{
	MagicalPreciousChapterCfg():chapter_id(0), reward_index(0), condition_type(0), shenwang_reward(0), mojing_reward(0), bind_gold_reward(0), param_a(0), param_b(0), param_c(0), get_score(0) {}

	short chapter_id;
	short reward_index;
	short condition_type;
	int shenwang_reward;
	int mojing_reward;
	int bind_gold_reward;
	int param_a;
	int param_b;
	int param_c;
	int get_score;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
};

struct MagicalPreciousFinishChapterRewardCfg
{
	static const int MAX_REWARD_ITEM_NUM = 6;

	MagicalPreciousFinishChapterRewardCfg():chapter_id(0), seq(0), need_score(0), reward_item_count(0) {}

	short chapter_id;
	short seq;
	int need_score;

	int reward_item_count;
	ItemConfigData reward_item[MAX_REWARD_ITEM_NUM];
};

class MagicalPreciousCfg : public ConfigBase
{
public:
	MagicalPreciousCfg();
	~MagicalPreciousCfg();

	bool Init(const std::string &configname, std::string *err);

	const int GetMaxChapter(){return m_max_chapter_count;}
	const int GetChapterMaxRewardIndex(int chapter);
	const MagicalPreciousOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const MagicalPreciousChapterCfg * GetChapterTaskCfg(short chapter, short reward_index);
	const MagicalPreciousFinishChapterRewardCfg * GetFinishChapterRewardCfg(int chapter, int seq);
	std::vector<const MagicalPreciousChapterCfg *> GetChapterCfgByCondition(int condition_type);

private:

	int InitOtherCfg(PugiXmlNode RootElement);
	int InitChapterTaskCfg(PugiXmlNode RootElement);
	int InitFinishChapterRewardCfg(PugiXmlNode RootElement);

	MagicalPreciousOtherCfg m_other_cfg;

	int m_max_chapter_count;
	int m_chapter_task_count[MAGICAL_PRECIOUS_MAX_CHAPTER];
	MagicalPreciousChapterCfg m_chapter_cfg[MAGICAL_PRECIOUS_MAX_CHAPTER][MAGICAL_PRECIOUS_MAX_REWARD_INDEX];

	std::vector<MagicalPreciousFinishChapterRewardCfg> m_chapter_finish_cfg_vec;
};

#endif // __MAGICAL_PRECIOUS_CONFIG_HPP__

