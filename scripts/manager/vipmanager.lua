VipManager = VipManager or BaseClass(BaseHotUpdate)
function VipManager:__init()
	if VipManager.Instance then
		ErrorLog("[VipManager] Attemp to create a singleton twice !")
	end
	
	VipManager.Instance = self
end

function VipManager:__delete()
	VipManager.Instance = nil
end

function VipManager:GetAuthParam(vip_level, auth_type)
	return C_LuaToCFun:GetAuthParam(vip_level, auth_type)
end