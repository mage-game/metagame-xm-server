LogicConfigManager = LogicConfigManager or BaseClass(BaseHotUpdate)
function LogicConfigManager:__init()
	if LogicConfigManager.Instance then
		ErrorLog("[LogicConfigManager] Attemp to create a singleton twice !")
	end
	
	LogicConfigManager.Instance = self
end

function LogicConfigManager:__delete()
	LogicConfigManager.Instance = nil
end

function LogicConfigManager:CreateObjects()
	self:AddObject("lingyucfg", LingyuCfg.New())
	self:AddObject("tianxiangefbcfg", TianxiangeFbCfg.New())
	self:AddObject("challengefbcfg", ChallengeFbCfg.New())
	self:AddObject("shuijingactcfg", ShuijingActCfg.New())
end

function LogicConfigManager:LoadFromC()
	return self:Load(LuaCodesBak, true)
end

function LogicConfigManager:LoadFromRevert()
	return self:Load(LuaCodesBak, false)
end

function LogicConfigManager:LoadFromPreCheck()
	return self:Load(LuaCodesBak, true)
end

function LogicConfigManager:LoadFromRecreate(new_bak)
	return self:Load(new_bak, true)
end

function LogicConfigManager:Load(bak, is_reload)
	for i, v in ipairs(self.object_queue) do
		local path = v:GetPath()
		local code = nil
		
		if is_reload then
			code = __loadfile(path, bak)
		else
			code = __getbakcode(bak, path)
		end

		if nil == code then
			return false
		end

		if not v:OnLoad(code()) then
			return false
		end
	end

	Log(string.format("load config success"))

	return true
end

