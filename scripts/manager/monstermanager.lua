MonsterManager = MonsterManager or BaseClass(BaseHotUpdate)
function MonsterManager:__init()
	if MonsterManager.Instance then
		ErrorLog("[MonsterManager] Attemp to create a singleton twice !")
	end
	
	MonsterManager.Instance = self
end

function MonsterManager:__delete()
	MonsterManager.Instance = nil
end

function MonsterManager:IsMonster(monster_id)
	return C_LuaToCFun:IsMonster(monster_id)
end

