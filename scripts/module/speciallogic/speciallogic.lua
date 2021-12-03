SpecialLogic = SpecialLogic or BaseClass(BaseHotUpdate)

function SpecialLogic:__init(c_speciallogic)
	self.c_speciallogic = c_speciallogic
end

function SpecialLogic:__delete()
	self.c_speciallogic = nil
end

function SpecialLogic:Update(interval, now_second)
	
end

function SpecialLogic:OnAddObj(c_obj)
	-- body
end

function SpecialLogic:OnRemoveObj(c_obj)
	-- body
end

function SpecialLogic:OnAttack(c_character, enemy_id, delta_hp)
	-- body
end

function SpecialLogic:OnHurtOther(c_character, c_target)
	-- body
end

function SpecialLogic:OnCharacterDie(c_character, killer_id)
	-- body
end

function SpecialLogic:OnRoleEnterScene(c_role)
	-- body
end

function SpecialLogic:OnRoleLeaveScene(c_role, is_logout)
	-- body
end

function SpecialLogic:OnGather(c_role, gather_id, obj_id, x, y)
	-- body
end

function SpecialLogic:OnStartGather(c_role, gather_id, obj_id, x, y)
	-- body
end

function SpecialLogic:OnAddExp(c_role, add_exp, reason)
	-- body
end

function SpecialLogic:OnLeaveFB(c_role)
	-- body
end

function SpecialLogic:OnAllMonsterDead()
	-- body
end

function SpecialLogic:OnReqNextLevel()
	-- body
end

function SpecialLogic:PerformSkill(c_role, skill_index, x, y, target_id)
	return false
end

function SpecialLogic:CanDestroy()
	return true
end

function SpecialLogic:SpecialRelive(c_role, realive_type)
	-- body
end

function SpecialLogic:SpecialRecalcInjure(c_attacker, c_injurer, injure)
	return injure
end

function SpecialLogic:OnTimeout()
	return false
end

function SpecialLogic:SpecialCanGuWu()
	return false
end

function SpecialLogic:SpecialCanMountOn()
	return true
end

function SpecialLogic:DelayKickOutRole(c_role, reason)
	self.c_speciallogic:DelayKickOutRole(c_role, reason or 0)
end

function SpecialLogic:DelayKickOutAllRole(reason)
	self.c_speciallogic:DelayKickOutAllRole(reason)
end

function SpecialLogic:AllRoleReLive()
	self.c_speciallogic:AllRoleReLive()
end

function SpecialLogic:IsAllMonsterDead()
	return self.c_speciallogic:IsAllMonsterDead()
end

function SpecialLogic:KillAllMonster()
	self.c_speciallogic:KillAllMonster()
end

function SpecialLogic:GetPassTime()
	return self.c_speciallogic:GetPassTime()
end

function SpecialLogic:GetKillAllMonster()
	return self.c_speciallogic:GetKillAllMonster()
end

function SpecialLogic:GetKillBoss()
	return self.c_speciallogic:GetKillBoss()
end

function SpecialLogic:CheckSceneMonsterStatus(interval, now_second)
	self.c_speciallogic:CheckSceneMonsterStatus(interval, now_second)
end

function SpecialLogic:GetScene()
	return self.c_speciallogic:GetScene()
end

function SpecialLogic:GetSceneId()
	local c_scene = self:GetScene()
	if nil == c_scene then
		return 0
	end
	
	return c_scene:GetSceneID()
end

function SpecialLogic:SpecialIsEnemy(c_role, c_obj)
	return self.c_speciallogic:SupserSpecialIsEnemy(c_role, c_obj)
end

function SpecialLogic:SpecialIsFriend(c_role, c_obj, check_alive)
	return self.c_speciallogic:SuperSpecialIsFriend(c_role, c_obj, check_alive or true)
end

function SpecialLogic:NotifyBossHPPercent(monsterid)
	self.c_speciallogic:NotifyBossHPPercent(monsterid)
end