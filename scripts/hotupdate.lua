HotUpdate = HotUpdate or BaseClass(BaseHotUpdate)
function HotUpdate:__init()

end

function HotUpdate:__delete()
end

function HotUpdate:CreateObjects()
	self:AddObject("DBManager", DBManager.New())
	self:AddObject("GmManager", GmManager.New())
	self:AddObject("LogicConfigManager", LogicConfigManager.New())
	self:AddObject("RoleManager", RoleManager.New())
	self:AddObject("ProtocolPool", ProtocolPool.New())
	self:AddObject("MessageHandleManagr", MessageHandleManagr.New())
	self:AddObject("SceneManager", SceneManager.New())
	self:AddObject("MonsterManager", MonsterManager.New())
	self:AddObject("FBManager", FBManager.New())
	self:AddObject("SpecialLogicManager", SpecialLogicManager.New())
	self:AddObject("WorldManager", WorldManager.New())
	self:AddObject("WordCommonData", WordCommonData.New())
	self:AddObject("ItemManager", ItemManager.New())
	self:AddObject("VipManager", VipManager.New())
	self:AddObject("MemManager", MemManager.New())
	self:AddObject("ActivityManager", ActivityManager.New())
	self:AddObject("JoinLimitManager", JoinLimitManager.New())
	self:AddObject("EventManager", EventManager.New())
end