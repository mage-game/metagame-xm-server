SpecialShuijing = SpecialShuijing or BaseClass(SpecialLogic)
SpecialShuijing.GATHER_MAX_NUM = 10
SpecialShuijing.GATHER_POINT_NUM = 200
SpecialShuijing.SHUIJING_ZHIZUN_TYPE = 9	-- 至尊水晶类型

function SpecialShuijing:__init()
	self:InitDataset()
end

function SpecialShuijing:__delete()
	
end

function SpecialShuijing:InitDataset()
	self.dataset.gather_next_flush_times = {}
	self.dataset.gather_objid_list = {}

	for i = 0, SpecialShuijing.GATHER_MAX_NUM - 1 do
		self.dataset.gather_next_flush_times[i] = 0
	end
	
	for i = 0, SpecialShuijing.GATHER_POINT_NUM - 1 do
		self.dataset.gather_objid_list[i] = -1
	end
end


function SpecialShuijing:Update(interval, now_second)
	SpecialLogic:Update(interval, now_second)

	self:CheckBornGather(now_second)
end

function SpecialShuijing:CheckBornGather(now_second)
	for i = 0, SpecialShuijing.GATHER_MAX_NUM - 1 do
		if now_second >= self.dataset.gather_next_flush_times[i] then
			local gather_cfg = ShuijingActCfg.Instance:GetGatherCfg(i)
			if nil ~= gather_cfg and self:CreateGathers(i) then
				-- 生长的时间计算不是以挖了以后多久才生长，而是每隔一段时间检查有没有被挖掉（策划要求）
				self.dataset.gather_next_flush_times[i] = now_second + gather_cfg.flush_time
			end
		end
	end
end

function SpecialShuijing:CreateGathers(gather_type)
	if gather_type < 0 or gather_type >= SpecialShuijing.GATHER_MAX_NUM then
		return false
	end

	local gather_cfg = ShuijingActCfg.Instance:GetGatherCfg(gather_type)
	if nil == gather_cfg then
		return false
	end

	if SpecialShuijing.SHUIJING_ZHIZUN_TYPE == gather_type and not ActivityManager.Instance:IsActivityOpen(ACTIVITY_TYPE.SHUIJING) then
		return false
	end

	local points = ShuijingActCfg.Instance:GetFlushPoints()

	for i, v in ipairs(points) do
		if -1 == self.dataset.gather_objid_list[i - 1] and v.gather_id == gather_cfg.gather_id then
			local _, objid = SceneManager.Instance:CreateGather(v.gather_id, self:GetScene(), v.pos_x, v.pos_y, gather_cfg.gather_time * 1000)
			self.dataset.gather_objid_list[i - 1] = objid
		end 
	end

	return true
end

function SpecialShuijing:OnGather(c_role, gather_id, obj_id, x, y)
	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if nil == role then
		return
	end

	for i = 0, SpecialShuijing.GATHER_POINT_NUM - 1 do
		if self.dataset.gather_objid_list[i] == obj_id then
			self.dataset.gather_objid_list[i] = -1
			break
		end
	end

	local gather_cfg = ShuijingActCfg.Instance:GetGatherCfgByGatherId(gather_id)
	if nil == gather_cfg then
		return
	end

	local other_cfg = ShuijingActCfg.Instance:GetOtherCfg()
	if role:GetDayCount(DAYCOUNT_ID.SHUIJING_GATHER) >= other_cfg.gather_max_times then
		return
	end

	role:DayCountIncrease(DAYCOUNT_ID.SHUIJING_GATHER)
	EventManager.Instance:OnShuiJingGather(role)

	local factor = 1
	if ActivityManager.Instance:IsActivityOpen(ACTIVITY_TYPE.SHUIJING) then
		factor = 2
	end

	if gather_cfg.nv_wa_shi > 0 then
		role:AddNvWaShi(gather_cfg.nv_wa_shi * factor, "ShuiJingGather")
	end

	if gather_cfg.bind_coin > 0 then
		role:AddCoinBind(gather_cfg.bind_coin * factor, "ShuiJingGather")
	end

	if gather_cfg.bind_gold > 0 then
		role:AddGoldBind(gather_cfg.bind_gold * factor, "ShuiJingGather")
	end
end

-- 返回true表示由该逻辑处理
function SpecialShuijing:SpecialRelive(c_role, realive_type)
	local role = RoleManager.Instance:GetRoleByCRole(c_role)
	if nil == role then
		return true
	end

	if REALIVE_TYPE.BACK_HOME ~= realive_type then
		return true
	end

	local other_cfg = ShuijingActCfg.Instance:GetOtherCfg()
	if role:GetDeadTime() < other_cfg.relive_time * 1000 then
		return true
	end

	local pos_x, pos_y = ShuijingActCfg.Instance:GetRelivePoint()
	role:ReAlive(pos_x, pos_y)

	return true
end

function SpecialShuijing:CanDestroy()
	return false
end

function SpecialShuijing:SpecialCanMountOn()
	return false
end