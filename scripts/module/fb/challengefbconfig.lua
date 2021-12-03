ChallengeFbCfg = ChallengeFbCfg or BaseClass(BaseHotUpdate)

function ChallengeFbCfg:__init(path)
	if ChallengeFbCfg.Instance then
		ErrorLog("[ChallengeFbCfg] Attemp to create a singleton twice !")
	end
	ChallengeFbCfg.Instance = self

	self.path = path

	self.level_cfg = {}
	self.chapter_cfg = {}
	self.other_cfg = {}
	self.buycost_cfg = {}
end

function ChallengeFbCfg:__delete()
	ChallengeFbCfg.Instance = nil
end

function ChallengeFbCfg:GetPath()
	return "configauto/challengefbcfg_auto"
end

function ChallengeFbCfg:OnLoad(cfg)
	local ret = self:CheckAllCfg(cfg)

	self.level_cfg = cfg.levelcfg
	self.chapter_cfg = cfg.chaptercfg
	self.other_cfg = cfg.other
	self.buycost_cfg = cfg.buy_cost

	return ret
end

function ChallengeFbCfg:CheckAllCfg(cfg)
	local ret, log = self:CheckLevelCfg(cfg)
	if not ret then
		ErrorLog("[ChallengeFbCfg:CheckLevelCfg] " .. log)
		return false
	end

	local ret, log = self:CheckChapterCfg(cfg)
	if not ret then
		ErrorLog("[ChallengeFbCfg:CheckChapterCfg]" .. log)
		return false
	end

	local ret, log = self:CheckOtherCfg(cfg)
	if not ret then
		ErrorLog("[ChallengeFbCfg:CheckOtherCfg]" .. log)
		return false
	end

	local ret, log = self:CehckBuyCostCfg(cfg)
	if not ret then
		ErrorLog("[ChallengeFbCfg:CehckBuyCostCfg]" .. log)
		return false
	end

	return true
end

function ChallengeFbCfg:CheckLevelCfg(cfg)	
	if nil == cfg.levelcfg then
		return false, "not found levelcfg"
	end

	ClearPolicy()
	AddPolicy(1, 'level', {CHKPOLICY.FIRST_VAL, 0}, {CHKPOLICY.LESS, ChallengeFb.MAX_LEVEL})
	AddPolicy(2, 'layer', {CHKPOLICY.INC_1}, {CHKPOLICY.FIRST_VAL, 0}, {CHKPOLICY.LAST_VAL, ChallengeFb.MAX_LAYER - 1})
	AddPolicy(2, 'scene_id', {CHKPOLICY.SCENE})
	AddPolicy(2, 'boss_id', {CHKPOLICY.MONSTER, 0})
	AddPolicy(2, 'boss_x', {CHKPOLICY.GTR, 0})
	AddPolicy(2, 'boss_y', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg.levelcfg)
end

function ChallengeFbCfg:CheckChapterCfg(cfg)
	if nil == cfg.chaptercfg then
		return false, "not found chaptercfg"
	end

	ClearPolicy()
	AddPolicy(1, 'level', {CHKPOLICY.FIRST_VAL, 0}, {CHKPOLICY.LESS, ChallengeFb.MAX_LEVEL})
	AddPolicy(1, 'pos_x', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'pos_y', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'role_level', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg.chaptercfg)
end

function ChallengeFbCfg:CheckOtherCfg(cfg)
	if nil == cfg.other then
		return false, "not found other"
	end

	ClearPolicy()
	AddPolicy(1, 'auto_item_id', {CHKPOLICY.ITEM, 0})
	AddPolicy(1, 'auto_item_num', {CHKPOLICY.GTR, 0})
	AddPolicy(1, 'reset_cost', {CHKPOLICY.GTR, 0})

	return CheckPolicy(cfg.other)
end

function ChallengeFbCfg:CehckBuyCostCfg(cfg)
	if nil == cfg.buy_cost then
		return false, "not found buy_cost"
	end

	ClearPolicy()
	AddPolicy(1, 'buy_times', {CHKPOLICY.INC, 0})
	AddPolicy(1, 'gold_cost', {CHKPOLICY.INC})

	return CheckPolicy(cfg.buy_cost)
end

function ChallengeFbCfg:GetLayerCfg(level, layer)
	for i, v in ipairs(self.level_cfg) do
		if level == v.level and layer == v.layer then
			return v
		end
	end

	return nil
end

function ChallengeFbCfg:GetChappterCfg(level)
	return self.chapter_cfg[level + 1]
end

function ChallengeFbCfg:GetLevelCfgBySceneID(scene_id)
	for k, v in pairs(self.level_cfg) do
		if scene_id == v.scene_id then
			return v
		end
	end

	return nil
end

function ChallengeFbCfg:GetBuyJoinTimes(buy_times)
	local len = #self.buycost_cfg
	for i = len, 1, -1 do
		if buy_times >= self.buycost_cfg[i].buy_times then
			return self.buycost_cfg[i].gold_cost
		end
	end

	return 0
end

function ChallengeFbCfg:GetOtherCfg()
	return self.other_cfg[1] or {}
end