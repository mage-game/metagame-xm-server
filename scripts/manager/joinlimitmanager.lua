JoinLimitManager = JoinLimitManager or BaseClass(BaseHotUpdate)

JoinLimitManager.JLMT_ACTIVITY = 1
JoinLimitManager.JLMT_TYPE_FB = 2
JoinLimitManager.JLMT_OTHER = 3

function JoinLimitManager:__init()
	if JoinLimitManager.Instance then
		ErrorLog("[JoinLimitManager] Attemp to create a singleton twice !")
	end
	
	JoinLimitManager.Instance = self
end

function JoinLimitManager:__delete()
	JoinLimitManager.Instance = nil
end

function JoinLimitManager:GetActivityLimitLevel(activity_type)
	return C_LuaToCFun:GetJoinLimitLevel(JoinLimitManager.JLMT_ACTIVITY, activity_type)
end

function JoinLimitManager:GetFbLimitLevel(fb_type)
	return C_LuaToCFun:GetJoinLimitLevel(JoinLimitManager.JLMT_TYPE_FB, fb_type)
end

function JoinLimitManager:GetOtherLimitLevel(type)
	return C_LuaToCFun:GetJoinLimitLevel(JoinLimitManager.JLMT_OTHER, type)
end