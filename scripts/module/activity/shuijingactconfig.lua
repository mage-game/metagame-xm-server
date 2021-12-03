ShuijingActCfg = ShuijingActCfg or BaseClass(BaseHotUpdate)

function ShuijingActCfg:__init(path)
	if ShuijingActCfg.Instance then
		ErrorLog("[ShuijingActCfg] Attemp to create a singleton twice !")
	end
	ShuijingActCfg.Instance = self

	self.other_cfg = {}
	self.realive_point_list_cfg = {}
	self.gather_cfg = {}
	self.flush_point_cfg = {}
end

function ShuijingActCfg:__delete()
	ShuijingActCfg.Instance = nil
end

function ShuijingActCfg:GetPath()
	return "configauto/activityshuijing_auto"
end

function ShuijingActCfg:OnLoad(cfg)
	local ret = self:CheckAllCfg(cfg)
	
	self.other_cfg = cfg.other
	self.realive_point_list_cfg = cfg.realive_point_list
	self.gather_cfg = cfg.gather
	self.flush_point_cfg = cfg.flush_point

	return ret
end

function ShuijingActCfg:CheckAllCfg(cfg)
	local ret, log = self:CheckOtherCfg(cfg.other)
	if not ret then
		ErrorLog("[ShuijingActCfg:CheckOtherCfg] " .. log)
		return false
	end

	local ret, log = self:CheckRelivePointCfg(cfg.realive_point_list)
	if not ret then
		ErrorLog("[ShuijingActCfg:CheckRelivePointCfg] " .. log)
		return false
	end

	local ret, log = self:CheckGatherCfg(cfg.gather)
	if not ret then
		ErrorLog("[ShuijingActCfg:CheckGatherCfg] " .. log)
		return false
	end

	local ret, log = self:CheckFlushPointCfg(cfg.flush_point)
	if not ret then
		ErrorLog("[ShuijingActCfg:CheckFlushPointCfg] " .. log)
		return false
	end

	return true
end

function ShuijingActCfg:CheckOtherCfg(cfg)	
	if nil == cfg then
		return false, "not found other"
	end

	ClearPolicy()
	AddPolicy(1, 'scene_id', {CHKPOLICY.SCENE})
	AddPolicy(1, 'relive_time', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'gather_max_times', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg)
end

function ShuijingActCfg:CheckRelivePointCfg(cfg)	
	if nil == cfg then
		return false, "not found realive_point_list"
	end

	ClearPolicy()
	AddPolicy(1, 'pos_x', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_y', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg)
end

function ShuijingActCfg:CheckGatherCfg(cfg)	
	if nil == cfg then
		return false, "not found gather"
	end

	ClearPolicy()

	AddPolicy(1, 'gather_type', {CHKPOLICY.FIRST_VAL, 0}, {CHKPOLICY.LAST_VAL, SpecialShuijing.SHUIJING_ZHIZUN_TYPE}, {CHKPOLICY.INC_1})
	AddPolicy(1, 'gather_id', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'gather_time', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'flush_time', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'nv_wa_shi', {CHKPOLICY.GTQ, 0})
	AddPolicy(1, 'bind_coin', {CHKPOLICY.GTQ, 0})
	AddPolicy(1, 'bind_gold', {CHKPOLICY.GTQ, 0})

	return CheckPolicy(cfg)
end

function ShuijingActCfg:CheckFlushPointCfg(cfg)	
	if nil == cfg then
		return false, "not found flush_point"
	end

	ClearPolicy()

	AddPolicy(1, 'gather_id', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_x', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_y', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg)
end

function ShuijingActCfg:GetGatherCfg(gather_type)
	return self.gather_cfg[gather_type + 1]
end

function ShuijingActCfg:GetGatherCfgByGatherId(gather_id)
	for i,v in ipairs(self.gather_cfg) do
		if gather_id == v.gather_id then
			return v
		end
	end

	return nil
end

function ShuijingActCfg:GetFlushPoints()
	return self.flush_point_cfg
end

function ShuijingActCfg:GetOtherCfg()
	return self.other_cfg[1]
end

function ShuijingActCfg:GetRelivePoint()
	local pos = self.realive_point_list_cfg[1]
	if nil ~= pos then
		return pos.pos_x, pos.pos_y
	end

	return 0, 0
end