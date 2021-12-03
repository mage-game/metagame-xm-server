SpecialLogicManager = SpecialLogicManager or BaseClass(BaseHotUpdate)
function SpecialLogicManager:__init()
	if SpecialLogicManager.Instance then
		ErrorLog("[SpecialLogicManager] Attemp to create a singleton twice !")
	end
	
	SpecialLogicManager.Instance = self
	self.specialclass_t = {}
	self:RegisterSpecialLogic()
end

function SpecialLogicManager:__delete()
	SpecialLogicManager.Instance = nil
end

function SpecialLogicManager:IsLuaSpecialSceneType(scene_type)
	return nil ~= self:GetSpecialLogicClass(scene_type)
end

function SpecialLogicManager:GetSpecialLogicClass(scene_type)
	return self.specialclass_t[scene_type]
end

function SpecialLogicManager:GetLogicByScenekey(scene_id, scene_key)
	return self:GetObject(scene_id .. "_" .. scene_key)
end

function SpecialLogicManager:OnCreateSpecialLogic(c_speciallogic)
	if nil == c_speciallogic then
		return
	end

	local scene_id = c_speciallogic:GetSceneId()
	local scene_key = c_speciallogic:GetSceneKey()
	local logic_class = self:GetSpecialLogicClass(c_speciallogic:GetSceneType())
	
	if nil == scene_id or nil == scene_key or nil == logic_class then
		return
	end

	if nil == self:GetLogicByScenekey(scene_id, scene_key) then
		local key = scene_id .. "_" .. scene_key
		Log("createspeciallogic", key)
		self:CreateDynamicObject(key, logic_class, c_speciallogic)
	end
end

function SpecialLogicManager:OnDestorySpecialLogic(scene_id, scene_key)
	local key = scene_id .. "_" .. scene_key
	Log("destroyspeciallogic", key)
	self:DeleteObject(key)
end

function SpecialLogicManager:Update(scene_id, scene_key, interval, now_second)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:Update(interval, now_second)
	end
end

function SpecialLogicManager:OnAddObj(scene_id, scene_key, c_obj)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnAddObj(c_obj)
	end
end

function SpecialLogicManager:OnRemoveObj(scene_id, scene_key, c_obj)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnRemoveObj(c_obj)
	end
end

function SpecialLogicManager:OnAttack(scene_id, scene_key, c_character, enemy_id, delta_hp)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnAttack(c_character, enemy_id, delta_hp)
	end
end

function SpecialLogicManager:OnHurtOther(scene_id, scene_key, c_character, c_target)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnHurtOther(c_character, c_target)
	end
end

function SpecialLogicManager:OnCharacterDie(scene_id, scene_key, c_character, killer_id)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnCharacterDie(c_character, killer_id)
	end
end

function SpecialLogicManager:OnRoleEnterScene(scene_id, scene_key, c_role)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnRoleEnterScene(c_role)
	end
end

function SpecialLogicManager:OnRoleLeaveScene(scene_id, scene_key, c_role, is_logout)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnRoleLeaveScene(c_role, is_logout)
	end
end

function SpecialLogicManager:OnGather(scene_id, scene_key, c_role, gather_id, obj_id, x, y)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnGather(c_role, gather_id, obj_id, x, y)
	end
end

function SpecialLogicManager:OnStartGather(scene_id, scene_key, c_role, gather_id, obj_id, x, y)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnStartGather(c_role, gather_id, obj_id, x, y)
	end
end

function SpecialLogicManager:OnAddExp(scene_id, scene_key, c_role, add_exp, reason)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnAddExp(c_role, add_exp, reason)
	end
end

function SpecialLogicManager:PerformSkill(scene_id, scene_key, c_role, skill_index, x, y, target_id)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:PerformSkill(c_role, skill_index, x, y, target_id)
	end
end

function SpecialLogicManager:CanDestroy(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:CanDestroy()
	end

	return true
end

function SpecialLogicManager:SpecialRelive(scene_id, scene_key, c_role, realive_type)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:SpecialRelive(c_role, realive_type)
	end
end

function SpecialLogicManager:SpecialIsEnemy(scene_id, scene_key, c_role, c_obj)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:SpecialIsEnemy(c_role, c_obj)
	end

	return false
end

function SpecialLogicManager:SpecialIsFriend(scene_id, scene_key, c_role, c_obj, check_alive)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:SpecialIsFriend(c_role, c_obj, check_alive)
	end

	return true
end

function SpecialLogicManager:SpecialRecalcInjure(scene_id, scene_key, c_attacker, c_injurer, injure)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:SpecialRecalcInjure(c_attacker, c_injurer, injure)
	end

	return injure
end

function SpecialLogicManager:OnLeaveFB(scene_id, scene_key, c_role)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnLeaveFB(c_role)
	end
end

function SpecialLogicManager:SpecialCanGuWu(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:SpecialCanGuWu()
	end

	return false
end

function SpecialLogicManager:SpecialCanMountOn(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:SpecialCanMountOn()
	end

	return true
end

function SpecialLogicManager:OnTimeout(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		return logic:OnTimeout()
	end

	return false
end

function SpecialLogicManager:SpecialOnAllMonsterDead(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnAllMonsterDead()
	end
end

function SpecialLogicManager:SpecialReqNextLevel(scene_id, scene_key)
	local logic = self:GetLogicByScenekey(scene_id, scene_key)
	if nil ~= logic then
		logic:OnReqNextLevel()
	end
end

function SpecialLogicManager:RegisterSpecialLogic()
	self.specialclass_t = {
		[SCENE_TYPE.SHUIJING] = SpecialShuijing,			-- 水晶幻境
		[SCENE_TYPE.TIANXIANGE] = SpecialTianxiange,		-- 天仙阁
		[SCENE_TYPE.CHALLENGE] = SpecialChallenge,			-- 挑战本
	}
end