#ifndef __CROSS_COMMON_CONFIG_H__
#define __CROSS_COMMON_CONFIG_H__

#include <set>

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class CrossCommonConfig
{
public:
	static CrossCommonConfig & Instance();

	bool Init(const std::string &path, std::string *err);

	int GetWaitSceneCount(int cross_activity_type);

	int GetWaitSceneID(int cross_activity_type, Posi *pos, int wait_scene_index = -1);
	bool IsCrossWaitScene(int scene_id);
	
private:
	CrossCommonConfig();
	~CrossCommonConfig();

	static const int WAIT_SCENE_ENTER_POS_MAX = 16;							// һ���ȴ�����������������

	struct CrossActivityWaitScene
	{
		CrossActivityWaitScene() : scene_id(0), enter_pos_count(0)
		{
			memset(enter_pos_list, 0, sizeof(enter_pos_list));
			memset(rand_dis_list, 0, sizeof(rand_dis_list));
		}

		int scene_id;														// ����ID
		int enter_pos_count;												// ���������
		Posi enter_pos_list[WAIT_SCENE_ENTER_POS_MAX];						// ���������
		int rand_dis_list[WAIT_SCENE_ENTER_POS_MAX];						// ����������Χ
	};

	struct CrossActivityWaitSceneCfg
	{
		CrossActivityWaitSceneCfg() : wait_scene_count(0) {}

		bool Invalid() const { return wait_scene_count <= 0; }

		int wait_scene_count;												// �ȴ���������
		CrossActivityWaitScene wait_scene_list[WAIT_SCENE_MAX];				// �ȴ������б�
	};

	CrossActivityWaitSceneCfg m_wait_scene_cfg_list[CROSS_ACTIVITY_TYPE_MAX];	// ����淨�ȴ����������б�

	std::set<int> m_wait_scene_id_set;											// �ȴ���������
};

#endif	// __CROSS_COMMON_CONFIG_H__

