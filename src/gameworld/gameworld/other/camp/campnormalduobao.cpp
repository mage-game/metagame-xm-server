#include "gamelog.h"
#include "servercommon/servercommon.h"
#include "item/knapsack.h"
#include "engineadapter.h"
#include "obj/character/role.h"
#include "servercommon/errornum.h"
#include "config/logicconfigmanager.hpp"
#include "engineadapter.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "servercommon/errornum.h"
#include <set>
#include "item/itempool.h"
#include "other/event/eventhandler.hpp"
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include "scene/scene.h"
#include "campconfig.hpp"

#include "world.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/servercommon.h"
#include "campnormalduobao.h"
#include "other/rolestatus/rolestatusmanager.hpp"

CampNormalDuobao::CampNormalDuobao()
{

}

CampNormalDuobao::~CampNormalDuobao() 
{
 
}

CampNormalDuobao & CampNormalDuobao::Instance()
{
	static CampNormalDuobao cnd;
	return cnd;
}

void CampNormalDuobao::OnEnter(Role *role, int sceneid)
{
	if (NULL == role)
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanEnterFB(true, true))
	{
		return;
	}

	if (role->GetCamp() <= CAMP_TYPE_INVALID || role->GetCamp() >= CAMP_TYPE_MAX)
	{
		role->NoticeNum(errornum::EN_CAMPNORMALDUOBAO_LIMIT_CAMP);
		return;
	}

	const CampNormalDuoBaoLayerCfg *layer_cfg = LOGIC_CONFIG->GetCampCfg().GetNormalDuoBaoLayerCfg(sceneid);
	if (NULL == layer_cfg)
	{
		return;
	}

	if (layer_cfg->sceneid != sceneid || layer_cfg->camp != role->GetCamp())
	{
		return;
	}

	World::GetInstWorld()->GetSceneManager()->GoTo(role, layer_cfg->sceneid, 0, layer_cfg->enterpos);
}

void CampNormalDuobao::OnExist(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!Scene::IsCampNormalDuoBaoScene(role->GetScene()->GetSceneID()))
	{
		return;
	}

	Posi target_pos(216, 188);
	int target_scene_id = 103;
	World::GetInstWorld()->GetSceneManager()->GoTo(role, target_scene_id, 0, target_pos);	
}