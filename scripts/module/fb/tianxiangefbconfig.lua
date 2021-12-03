TianxiangeFbCfg = TianxiangeFbCfg or BaseClass(BaseHotUpdate)

function TianxiangeFbCfg:__init(path)
	if TianxiangeFbCfg.Instance then
		ErrorLog("[TianxiangeFbCfg] Attemp to create a singleton twice !")
	end
	TianxiangeFbCfg.Instance = self

	self.path = path

	self.level_cfg = {}
	self.refresh_cfg = {}
end

function TianxiangeFbCfg:__delete()
	TianxiangeFbCfg.Instance = nil
end

function TianxiangeFbCfg:GetPath()
	return "configauto/tianxiangefbcfg_auto"
end

function TianxiangeFbCfg:OnLoad(cfg)
	local ret = self:CheckAllCfg(cfg)

	self.level_cfg = cfg.level_cfg

	return ret
end

function TianxiangeFbCfg:CheckAllCfg(cfg)
	local ret, log = self:CheckLevelCfg(cfg)
	if not ret then
		ErrorLog("[TianxiangeFbCfg:CheckLevelCfg] " .. log)
		return false
	end

	return true
end

function TianxiangeFbCfg:CheckLevelCfg(cfg)	
	if nil == cfg.level_cfg then
		return false, "not found level_cfg"
	end

	ClearPolicy()
	AddPolicy(1, 'level', {CHKPOLICY.INC_1}, {CHKPOLICY.FIRST_VAL, 1})
	AddPolicy(1, 'is_broadcast', {CHKPOLICY.GTQ, 0}, {CHKPOLICY.LEQ, 1})
	AddPolicy(1, 'scene_id', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_x', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_y', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'boss_id', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'boss_x', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'boss_y', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'gongji', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'fangyu', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'maxhp', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'mingzhong', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'shanbi', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg.level_cfg)
end

function TianxiangeFbCfg:GetLevelCfg(level)
	return self.level_cfg[level]
end

function TianxiangeFbCfg:GetLevelCfgByMonsterId(monsterid)
	for k,v in pairs(self.level_cfg) do
		if monsterid == v.boss_id then
			return v
		end
	end

	return nil
end