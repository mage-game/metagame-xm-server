LingyuCfg = LingyuCfg or BaseClass(BaseHotUpdate)

function LingyuCfg:__init(path)
	if LingyuCfg.Instance then
		ErrorLog("[LingyuCfg] Attemp to create a singleton twice !")
	end
	LingyuCfg.Instance = self

	self.upstar_cfg = {}
end

function LingyuCfg:__delete()
	LingyuCfg.Instance = nil
end

function LingyuCfg:GetPath()
	return "configauto/lingyucfg_auto"
end

function LingyuCfg:OnLoad(cfg)
	local ret = self:CheckAllCfg(cfg)
	
	self.upstar_cfg = cfg.upstar

	return ret
end

function LingyuCfg:CheckAllCfg(cfg)
	local ret, log = self:CheckUpStarCfg(cfg.upstar)
	if not ret then
		ErrorLog("[LingyuCfg:CheckUpStarCfg] " .. log)
		return false
	end

	return true
end

function LingyuCfg:CheckUpStarCfg(cfg)	
	if nil == cfg then
		return false, "not found upstar_cfg"
	end

	ClearPolicy()
	AddPolicy(1, 'color', {CHKPOLICY.FIRST_VAL, 0}, {CHKPOLICY.LESS, Lingyu.MAX_COLOR})
	AddPolicy(2, 'level', {CHKPOLICY.INC_1}, {CHKPOLICY.FIRST_VAL, 1})
	AddPolicy(2, 'stuff_id', {CHKPOLICY.ITEM})
	AddPolicy(2, 'stuff_count', {CHKPOLICY.GTR, 0})
	AddPolicy(2, 'gongji', {CHKPOLICY.GTQ, 0})
	AddPolicy(2, 'fangyu', {CHKPOLICY.GTQ, 0})
	AddPolicy(2, 'maxhp', {CHKPOLICY.GTQ, 0})
	AddPolicy(2, 'mingzhong', {CHKPOLICY.GTQ, 0})
	AddPolicy(2, 'shanbi', {CHKPOLICY.GTQ, 0})
	AddPolicy(2, 'is_broadcast', {CHKPOLICY.GTQ, 0}, {CHKPOLICY.LEQ, 1})

	return CheckPolicy(cfg)
end

function LingyuCfg:GetUpStarCfg(color, level)
	for k,v in pairs(self.upstar_cfg) do
		if color == v.color and level == v.level then
			return v
		end
	end

	return nil
end