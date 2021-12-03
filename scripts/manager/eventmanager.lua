EventManager = EventManager or BaseClass(BaseHotUpdate)

function EventManager:__init()
	if EventManager.Instance then
		ErrorLog(" EventManager] Attemp to create a singleton twice !")
	end
	
	EventManager.Instance = self
end

function EventManager:__delete()
	EventManager.Instance = nil
end

function EventManager:GetCEvent()
	return C_EventHandler:Instance()
end

function EventManager:OnShuiJingGather(role)
	if nil == role then
		return
	end

	self:GetCEvent():OnShuiJingGather(role:GetCRole())
end