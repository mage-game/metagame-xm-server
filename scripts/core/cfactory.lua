CFactory = CFactory or {}

function CFactory:CItemConfigData(itemid, num, isbind)
	return C_ItemConfigData:new_local(itemid, isbind, num)
end

function CFactory:CMailContentParam()
	if nil ~= CFactory.MailContentParam then
		CFactory.MailContentParam:Reset()

		return CFactory.MailContentParam
	end

	CFactory.MailContentParam = C_MailContentParam:new_local()
	
	return CFactory.MailContentParam
end

function CFactory:CCharIntAttrs()
	return C_CharIntAttrs:new_local()
end

function CFactory:CConsumeStuffList()
	return C_ConsumeStuffList:new_local()
end

function CFactory:CMonsterDropList()
	return C_MonsterDropList:new_local()
end

function CFactory:CLuaPos(x, y)
	return C_LuaPos:new_local(x, y)
end

function CFactory:CActivity(activity_type)
	return C_Activity:new_local(C_ActivityManager:Instance(), activity_type)
end