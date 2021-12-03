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

	static const int WAIT_SCENE_ENTER_POS_MAX = 16;							// 一个等待场景进入点最大数量

	struct CrossActivityWaitScene
	{
		CrossActivityWaitScene() : scene_id(0), enter_pos_count(0)
		{
			memset(enter_pos_list, 0, sizeof(enter_pos_list));
			memset(rand_dis_list, 0, sizeof(rand_dis_list));
		}

		int scene_id;														// 场景ID
		int enter_pos_count;												// 进入点数量
		Posi enter_pos_list[WAIT_SCENE_ENTER_POS_MAX];						// 进入点坐标
		int rand_dis_list[WAIT_SCENE_ENTER_POS_MAX];						// 进入点随机范围
	};

	struct CrossActivityWaitSceneCfg
	{
		CrossActivityWaitSceneCfg() : wait_scene_count(0) {}

		bool Invalid() const { return wait_scene_count <= 0; }

		int wait_scene_count;												// 等待场景数量
		CrossActivityWaitScene wait_scene_list[WAIT_SCENE_MAX];				// 等待场景列表
	};

	CrossActivityWaitSceneCfg m_wait_scene_cfg_list[CROSS_ACTIVITY_TYPE_MAX];	// 跨服玩法等待场景配置列表

	std::set<int> m_wait_scene_id_set;											// 等待场景集合
};

#endif	// __CROSS_COMMON_CONFIG_H__

