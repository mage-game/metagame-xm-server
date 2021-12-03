SpecialTianxiange = SpecialTianxiange or BaseClass(SpecialLogic)

function SpecialTianxiange:__init()
	self:InitDataset()
end

function SpecialTianxiange:__delete()
	
end

function SpecialTianxiange:InitDataset()
	self.dataset.is_finish = false
	self.dataset.is_pass = false
	self.dataset.owner_roleid = 0
	self.dataset.owner_objid = CONSTANT.INVALID_OBJ_ID
	self.dataset.monsterid = 0
	self.dataset.level = 0
end

function SpecialTianxiange:Update(interval, now_second)
	if self.dataset.is_finish or 0 == self.dataset.owner_roleid then
		return
	end

	self:CheckSceneMonsterStatus(interval, now_second)
end

function SpecialTianxiange:OnReqNextLevel()
	if not self.dataset.is_finish or not self.dataset.is_pass then
		return
	end

	if not self:IsAllMonsterDead() then
		return
	end

	local role = self:GetOwnerRole()
	if nil == role then
		return
	end

	local pass_level = role:GetCommonData():GetTianxiangeLevel()
	local level_cfg = TianxiangeFbCfg.Instance:GetLevelCfg(pass_level + 1)
	if nil == level_cfg then
		return
	end

	if not SceneManager.Instance:ResetScene(self:GetScene(), role) then
		return
	end

	SceneManager.Instance:CreateMonster(level_cfg.boss_id, self:GetScene(), level_cfg.boss_x, level_cfg.boss_y)
	self.dataset.is_finish = false
	self.dataset.is_pass = false
	self.dataset.monsterid = level_cfg.boss_id
	self.dataset.level = level_cfg.level

	self:SyncSceneInfo()
end

function SpecialTianxiange:OnRoleEnterScene(c_role)
	if self.dataset.is_finish then
		self:DelayKickOutRole(c_role)
		return
	end

	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if nil == role then
		return
	end

	if 0 == self.dataset.owner_roleid then
		local pass_level = role:GetCommonData():GetTianxiangeLevel()
		local level_cfg = TianxiangeFbCfg.Instance:GetLevelCfg(pass_level + 1)

		if nil ~= level_cfg then
			self.dataset.monsterid = level_cfg.boss_id
			SceneManager.Instance:CreateMonster(level_cfg.boss_id, self:GetScene(), level_cfg.boss_x, level_cfg.boss_y)
			self.dataset.level = level_cfg.level
		end
	end

	self.dataset.owner_roleid = role:GetRoleId()
	self.dataset.owner_objid = role:GetObjId()
	self:SyncSceneInfo()
end

function SpecialTianxiange:SpecialCanMountOn()
	return false
end

function SpecialTianxiange:SpecialCanGuWu()
	return true
end

function SpecialTianxiange:SpecialRelive(c_role, realive_type)
	if self.dataset.is_finish then 
		return false
	end
	
	if REALIVE_TYPE.BACK_HOME == realive_type then
		self:OnFinish(false)
		self:DelayKickOutRole(c_role)
	end

	return false
end

function SpecialTianxiange:OnTimeout()
	if not self.dataset.is_finish then
		self:OnFinish(false)
	end

	return self.dataset.is_pass
end

function SpecialTianxiange:OnAllMonsterDead()
	self:OnFinish(true)
end

function SpecialTianxiange:OnFinish(is_pass)
	if self.dataset.is_finish then
		return
	end

	self.dataset.is_finish = true
	self.dataset.is_pass = is_pass

	local role = self:GetOwnerRole()
	if nil == role then
		return
	end

	if self.dataset.is_pass then
		local pass_level = role:GetCommonData():GetTianxiangeLevel()
		local level_cfg = TianxiangeFbCfg.Instance:GetLevelCfgByMonsterId(self.dataset.monsterid)
		
		if nil ~= level_cfg and level_cfg.level > role:GetCommonData():GetTianxiangeLevel() then
			role:Recover()
			role:GetTianxiangeFb():OnFinish(level_cfg.level)
		end
	end

	self:SyncSceneInfo()
end

function SpecialTianxiange:GetOwnerRole()
	local c_role = SceneManager.Instance:GetCRole(self:GetScene(), self.dataset.owner_objid)
	local role = RoleManager.Instance:GetRoleByCRole(c_role)

	if nil == role or role:GetRoleId() ~= self.dataset.owner_roleid then
		return nil
	end

	return role
end

function SpecialTianxiange:SyncSceneInfo()
	local role = self:GetOwnerRole()
	if nil == role then
		return
	end

	local protocol = ProtocolPool.Instance:GetProtocol(SCTianxiangeSceneInfo)
	protocol.level = self.dataset.level
	protocol.time_out_stamp = SceneManager.Instance:GetSceneTimeOutStamp(self:GetScene())
	protocol.is_finish = self.dataset.is_finish and 1 or 0
	protocol.is_pass = self.dataset.is_pass and 1 or 0
	protocol.pass_time_s = self:GetPassTime()

	role:SendProtocol(protocol)
end
