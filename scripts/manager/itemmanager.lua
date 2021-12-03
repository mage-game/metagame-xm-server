ItemManager = ItemManager or BaseClass(BaseHotUpdate)
function ItemManager:__init()
	if ItemManager.Instance then
		ErrorLog("[ItemManager] Attemp to create a singleton twice !")
	end
	
	ItemManager.Instance = self
end

function ItemManager:__delete()
	ItemManager.Instance = nil
end

function ItemManager:IsItemBase(item_id)
	return C_LuaToCFun:IsItemBase(item_id)
end

function ItemManager:IsEquipment(item_id)
	return C_LuaToCFun:IsEquipment(item_id)
end
