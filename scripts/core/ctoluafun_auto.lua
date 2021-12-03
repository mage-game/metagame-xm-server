function CL_ActivityOnUserLogin(role)
	ActivityManager.Instance:OnUserLogin(role)
end

function CL_SpecialRelive(scene_id, scene_key, role, realive_type)
	local ret = SpecialLogicManager.Instance:SpecialRelive(scene_id, scene_key, role, realive_type)
	return ret
end

function CL_IsDelayDeleteScene(scene_type)
	local ret = SceneManager.Instance:IsDelayDeleteScene(scene_type)
	return ret
end

function CL_ActivityIsOpen(activity_type)
	local ret = ActivityManager.Instance:IsActivityOpen(activity_type)
	return ret
end

function CL_ActivityIsClose(activity_type)
	local ret = ActivityManager.Instance:IsActivityClose(activity_type)
	return ret
end

function CL_ActivityOnUserLeaveScene(role)
	ActivityManager.Instance:OnUserLeaveScene(role)
end

function CL_OnSpecialUpdate(scene_id, scene_key, interval, now_second)
	SpecialLogicManager.Instance:Update(scene_id, scene_key, interval, now_second)
end

function CL_OnWorldGetInitParam(other_p, worlddb)
	DBManager.Instance:OnWorldGetInitParam(other_p, worlddb)
end

function CL_SpecialOnLeaveFB(scene_id, scene_key, role)
	SpecialLogicManager.Instance:OnLeaveFB(scene_id, scene_key, role)
end

function CL_SpecialOnAllMonsterDead(scene_id, scene_key)
	SpecialLogicManager.Instance:SpecialOnAllMonsterDead(scene_id, scene_key)
end

function CL_OnCreateSpecialLogic(special_logic)
	local ret = SpecialLogicManager.Instance:OnCreateSpecialLogic(special_logic)
	return ret
end

function CL_OnSpecialHurtOther(scene_id, scene_key, cha, target)
	SpecialLogicManager.Instance:OnHurtOther(scene_id, scene_key, cha, target)
end

function CL_OnAddRole(roleid, role)
	RoleManager.Instance:OnAddRole(roleid, role)
end

function CL_OnSpecialStartGather(scene_id, scene_key, role, gather_id, obj_id, x, y)
	SpecialLogicManager.Instance:OnStartGather(scene_id, scene_key, role, gather_id, obj_id, x, y)
end

function CL_OnDestorySpecialLogic(scene_id, scene_key)
	local ret = SpecialLogicManager.Instance:OnDestorySpecialLogic(scene_id, scene_key)
	return ret
end

function CL_IsLuaSpecialSceneType(scene_type)
	local ret = SpecialLogicManager.Instance:IsLuaSpecialSceneType(scene_type)
	return ret
end

function CL_OnSpecialRoleLeaveScene(scene_id, scene_key, role, is_logout)
	SpecialLogicManager.Instance:OnRoleLeaveScene(scene_id, scene_key, role, is_logout)
end

function CL_GetMaxSceneType()
	local ret = EnumAdapter:GetMaxSceneType()
	return ret
end

function CL_SpecialIsFriend(scene_id, scene_key, role, obj, check_alive)
	local ret = SpecialLogicManager.Instance:SpecialIsFriend(scene_id, scene_key, role, obj, check_alive)
	return ret
end

function CL_OnSpecialRoleEnterScene(scene_id, scene_key, role)
	SpecialLogicManager.Instance:OnRoleEnterScene(scene_id, scene_key, role)
end

function CL_SpecialCanGuWu(scene_id, scene_key)
	local ret = SpecialLogicManager.Instance:SpecialCanGuWu(scene_id, scene_key)
	return ret
end

function CL_OnSpecialAddObj(scene_id, scene_key, obj)
	SpecialLogicManager.Instance:OnAddObj(scene_id, scene_key, obj)
end

function CL_SpecialOnTimeout(scene_id, scene_key)
	local ret = SpecialLogicManager.Instance:OnTimeout(scene_id, scene_key)
	return ret
end

function CL_SpecialReqNextLevel(scene_id, scene_key)
	SpecialLogicManager.Instance:SpecialReqNextLevel(scene_id, scene_key)
end

function CL_IsLuaActivityType(activity_type)
	local ret = ActivityManager.Instance:IsLuaActivityType(activity_type)
	return ret
end

function CL_SpecialRecalcInjure(scene_id, scene_key, attacker, injurer, injure)
	local ret = SpecialLogicManager.Instance:SpecialRecalcInjure(scene_id, scene_key, attacker, injurer, injure)
	return ret
end

function CL_SpecialCanDestroy(scene_id, scene_key)
	local ret = SpecialLogicManager.Instance:CanDestroy(scene_id, scene_key)
	return ret
end

function CL_ActivityOnStatusChange(activity_type, from_status, to_status)
	ActivityManager.Instance:OnActivityStatusChange(activity_type, from_status, to_status)
end

function CL_OnDestoryRole(roleid)
	RoleManager.Instance:OnDestoryRole(roleid)
end

function CL_AdaptSCVisibleObjEnterRole(roleid, datastream)
	ProtocolAdapter:AdaptSCVisibleObjEnterRole(roleid, datastream)
end

function CL_OnSpecialPerformSkill(scene_id, scene_key, role, skill_index, x, y, target_id)
	local ret = SpecialLogicManager.Instance:PerformSkill(scene_id, scene_key, role, skill_index, x, y, target_id)
	return ret
end

function CL_OnServerStart()
	WorldManager.Instance:OnServerStart()
end

function CL_OnRoleGetInitParam(roleid, other_p, roledb)
	DBManager.Instance:OnRoleGetInitParam(roleid, other_p, roledb)
end

function CL_ActivityOnUserLogout(role)
	ActivityManager.Instance:OnUserLogout(role)
end

function CL_OnSpecialGather(scene_id, scene_key, role, gather_id, obj_id, x, y)
	SpecialLogicManager.Instance:OnGather(scene_id, scene_key, role, gather_id, obj_id, x, y)
end

function CL_ActivityOnQueryRoomStatusReq(activity_type, role)
	ActivityManager.Instance:OnQueryRoomStatusReq(activity_type, role)
end

function CL_OnRoleResetData(roleid, old_dayid, now_dayid)
	RoleManager.Instance:OnResetData(roleid, old_dayid, now_dayid)
end

function CL_ActivityUpdate(interval, now_second)
	ActivityManager.Instance:Update(interval, now_second)
end

function CL_OnUserLogout(roleid)
	RoleManager.Instance:OnRoleLogout(roleid)
end

function CL_OnSpecialAttack(scene_id, scene_key, cha, enemy_id, delta_hp)
	SpecialLogicManager.Instance:OnAttack(scene_id, scene_key, cha, enemy_id, delta_hp)
end

function CL_OnRoleSendAllInfo(roleid)
	MessageHandleManagr.Instance:OnRoleSendAllInfo(roleid)
end

function CL_OnSpecialCharacterDie(scene_id, scene_key, dead, killer_id)
	SpecialLogicManager.Instance:OnCharacterDie(scene_id, scene_key, dead, killer_id)
end

function CL_OnActivityEnterReq(activity_type, root_index, role_id)
	ActivityManager.Instance:OnActivityEnterReq(activity_type, root_index, role_id)
end

function CL_OnSpecialRemoveObj(scene_id, scene_key, obj)
	SpecialLogicManager.Instance:OnRemoveObj(scene_id, scene_key, obj)
end

function CL_OnEnterFB(roleid, fb_type, param1, param2, param3)
	local ret = FBManager.Instance:OnEnterFB(roleid, fb_type, param1, param2, param3)
	return ret
end

function CL_ActivityOnForceToNextState(activity_type)
	ActivityManager.Instance:OnForceToNextState(activity_type)
end

function CL_OnRoleDieInStaticScene(roleid, killerid)
	RoleManager.Instance:OnRoleDieInStaticScene(roleid, killerid)
end

function CL_OnWorldUpdate(interval, now_second)
	WorldManager.Instance:OnWorldUpdate(interval, now_second)
end

function CL_SpecialCanMountOn(scene_id, scene_key, role)
	local ret = SpecialLogicManager.Instance:SpecialCanMountOn(scene_id, scene_key, role)
	return ret
end

function CL_OnWorldInitParam(other_p, worlddb)
	DBManager.Instance:OnWorldInitParam(other_p, worlddb)
end

function CL_OnLoadLogicConfigList()
	local ret = LogicConfigManager.Instance:LoadFromC()
	return ret
end

function CL_OnSpecialAddExp(scene_id, scene_key, role, add_exp, reason)
	SpecialLogicManager.Instance:OnAddExp(scene_id, scene_key, role, add_exp, reason)
end

function CL_SpecialIsEnemy(scene_id, scene_key, role, obj)
	local ret = SpecialLogicManager.Instance:SpecialIsEnemy(scene_id, scene_key, role, obj)
	return ret
end

function CL_ActivityOnUserEnterScene(role)
	ActivityManager.Instance:OnUserEnterScene(role)
end

function CL_IsCanResetScene(scene_type)
	local ret = SceneManager.Instance:IsCanResetScene(scene_type)
	return ret
end

function CL_OnReCalcRoleAttr(roleid, base_add, recalc_reason, recalc_all)
	RoleManager.Instance:ReCalcRoleAttr(roleid, base_add, recalc_reason, recalc_all)
end

function CL_OnRoleInitParam(roleid, other_p, roledb)
	DBManager.Instance:OnRoleInitParam(roleid, other_p, roledb)
end

