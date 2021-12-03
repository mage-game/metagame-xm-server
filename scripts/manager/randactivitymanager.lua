RandActivityManager = RandActivityManager or BaseClass(BaseHotUpdate)
function RandActivityManager:__init()
	if RandActivityManager.Instance then
		ErrorLog("[RandActivityManager] Attemp to create a singleton twice !")
	end
	
	RandActivityManager.Instance = self

	self.fb_handle_t = {}
	self:BindEnterFBHandle()
end

function RandActivityManager:__delete()
	RandActivityManager.Instance = nil
end

