SpecialChallenge = SpecialChallenge or BaseClass(SpecialLogic)

function SpecialChallenge:__init()
	self:InitDataset()
end

function SpecialChallenge:__delete()
	
end

function SpecialChallenge:InitDataset()
	self.dataset.is_first = false
	self.dataset.is_finish = false
	self.dataset.is_pass = false
	self.dataset.boss_id = 0
	self.dataset.owner_roleid = 0
	self.dataset.owner_objid = 0
	self.dataset.layer = 0
	self.dataset.level = 0
end

function SpecialChallenge:Update(interval, now_second)
	if not self.dataset.is_first then
		self.dataset.is_first = true

		local level_cfg = ChallengeFbCfg.Instance:GetLevelCfgBySceneID(self:GetSceneId())
		if nil ~= level_cfg then
			SceneManager.Instance:CreateMonster(level_cfg.boss_id, self:GetScene(), level_cfg.boss_x, level_cfg.boss_y)
			self.dataset.boss_id = level_cfg.boss_id
			self.dataset.level = level_cfg.level
			self.dataset.layer = level_cfg.layer
			self:SyncSceneInfo()
		end
	end

	if self.dataset.is_finish then
		return
	end

	self:CheckSceneMonsterStatus(interval, now_second)
end

function SpecialChallenge:OnRoleEnterScene(c_role)
	if self.dataset.is_finish then
		self:DelayKickOutRole(c_role)
		return
	end

	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if nil == role then
		return
	end

	self.dataset.owner_roleid = role:GetRoleId()
	self.dataset.owner_objid = role:GetObjId()
	self:SyncSceneInfo()
end

function SpecialChallenge:OnLeaveFB(c_role)
	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if not self.dataset.is_finish and nil ~= role and role:GetRoleId() == self.dataset.owner_roleid then
		self:OnLayerFinish(false, true)
	end
end

function SpecialChallenge:OnAllMonsterDead()
	self:OnLayerFinish(true, false)
end

function SpecialChallenge:OnTimeout()
	self:OnLayerFinish(false, false)
	return false
end

function SpecialChallenge:GetOwnerRole()
	local c_role = SceneManager.Instance:GetCRole(self:GetScene(), self.dataset.owner_objid)
	local role = RoleManager.Instance:GetRoleByCRole(c_role)

	if nil == role or role:GetRoleId() ~= self.dataset.owner_roleid then
		return nil
	end

	return role
end

function SpecialChallenge:OnLayerFinish(is_pass, is_active_leave_fb)
	if self.dataset.is_finish then
		return
	end

	self.dataset.is_finish = true
	self.dataset.is_pass = is_pass

	if not is_active_leave_fb then
		self:NotifyBossHPPercent(self.dataset.boss_id)
	end

	local role = self:GetOwnerRole()
	if nil == role then
		return
	end

	role:GetChallengeFb():OnFBFinish(self.dataset.is_pass, self:GetSceneId(), is_active_leave_fb)
	self:SyncSceneInfo()
end

function SpecialChallenge:SyncSceneInfo()
	local role = self:GetOwnerRole()
	if nil == role then
		return
	end

	local protocol = ProtocolPool.Instance:GetProtocol(SCChallengeSceneInfo)
	protocol.level = self.dataset.level
	protocol.layer = self.dataset.layer
	protocol.time_out_stamp = SceneManager.Instance:GetSceneTimeOutStamp(self:GetScene())
	protocol.is_finish = self.dataset.is_finish and 1 or 0
	protocol.is_pass = self.dataset.is_pass and 1 or 0
	protocol.pass_time_s = self:GetPassTime()

	role:SendProtocol(protocol)
end
