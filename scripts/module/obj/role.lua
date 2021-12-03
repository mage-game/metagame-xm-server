Role = Role or BaseClass(BaseHotUpdate)

function Role:__init(c_role)
	self.c_role = c_role
end

function Role:__delete()
	
end

function Role:DestoryC()
	self.c_role = nil
end

function Role:CreateObjects()
	self:AddObject("commondata", RoleCommonData.New(self))
	self:AddObject("lingyu", Lingyu.New(self))
	self:AddObject("tianxiangefb", TianxiangeFb.New(self))
	self:AddObject("challengefb", ChallengeFb.New(self))
end

function Role:GetCRole()
	return self.c_role
end

function Role:GetRoleId()
	return self.c_role:GetUID()
end

function Role:GetObjId()
	return self.c_role:GetId()
end

function Role:GetName()
	return self.c_role:GetName()
end

function Role:GetCamp()
	return self.c_role:GetCamp()
end

function Role:GetLevel()
	return self.c_role:GetLevel()
end

function Role:GetVipLevel()
	return self.c_role:GetVip():GetVipLevel()
end

function Role:GetDeadTime()
	return self.c_role:GetDeadTime()
end

function Role:DayCountCheck(day_count_id, max_count)
	return self.c_role:GetDayCounter():DayCountCheck(day_count_id, max_count)
end

function Role:DayCountIncrease(day_count_id)
	self.c_role:GetDayCounter():DayCountIncrease(day_count_id)
end

function Role:GetDayCount(day_count_id)
	return self.c_role:GetDayCounter():GetDayCount(day_count_id)
end

function Role:ConsumeItemList(c_consume_list, reason_str)
	return C_LuaToCFun:ConsumeStuffs(self.c_role, c_consume_list, reason_str)
end

-- @isbind true or false
function Role:PutItem(itemid, num, isbind, reason_type)
	local c_itemcfgdata = CFactory:CItemConfigData(itemid, num, isbind)
	self.c_role:GetKnapsack():PutOrMail(c_itemcfgdata, reason_type)
end

function Role:PutDropList(c_drop_list, reason_type)
	return C_LuaToCFun:PutDropList(self.c_role, c_drop_list, reason_type)
end

function Role:CheckEmptyGridNoLessThan(count)
	return self.c_role:GetKnapsack():CheckEmptyGridNoLessThan(count)
end

function Role:UseGold(consume_gold, type_str, is_consume_sys)
	return self.c_role:GetKnapsack():GetMoney():UseGold(consume_gold, type_str, is_consume_sys or true)
end

function Role:UseGoldBind(consume_gold_bind, type_str)
	return self.c_role:GetKnapsack():GetMoney():UseGoldBind(consume_gold_bind, type_str)
end

function Role:UseAllGold(consume_all_gold, type_str)
	return self.c_role:GetKnapsack():GetMoney():UseAllGold(consume_all_gold, type_str)
end

function Role:UseCoin(consume_coin, type_str)
	return self.c_role:GetKnapsack():GetMoney():UseCoin(consume_coin, type_str)
end

function Role:UseCoinBind(consume_coin_bind, type_str)
	return self.c_role:GetKnapsack():GetMoney():UseCoinBind(consume_coin_bind, type_str)
end

function Role:UseAllCoin(consume_all_coin, type_str)
	return self.c_role:GetKnapsack():GetMoney():UseAllCoin(consume_all_coin, type_str)
end

function Role:CoinMoreThan(consume_coin)
	return self.c_role:GetKnapsack():GetMoney():CoinMoreThan(consume_coin)
end

function Role:CoinBindMoreThan(consume_coin_bind)
	return self.c_role:GetKnapsack():GetMoney():CoinBindMoreThan(consume_coin_bind)
end

function Role:AllCoinMoreThan(consume_coin)
	return self.c_role:GetKnapsack():GetMoney():AllCoinMoreThan(consume_coin)
end

function Role:AddGold(gold, type_str, is_chongzhi)
	return self.c_role:GetKnapsack():GetMoney():AddGold(gold, type_str, is_chongzhi or false)
end

function Role:AddGoldBind(gold_bind, type_str)
	return self.c_role:GetKnapsack():GetMoney():AddGoldBind(gold_bind, type_str)
end

function Role:AddCoin(coin, type_str)
	return self.c_role:GetKnapsack():GetMoney():AddCoin(coin, type_str)
end

function Role:AddCoinBind(coin_bind, type_str)
	return self.c_role:GetKnapsack():GetMoney():AddCoinBind(coin_bind, type_str)
end

function Role:AddNvWaShi(nv_wa_shi, type_str)
	return self.c_role:AddNvWaShi(nv_wa_shi, type_str)
end

function Role:ReCalcAttr(recalc_reason)
	self.c_role:ReCalcAttr(recalc_reason)
end

function Role:ReCalcCap(capability_type, c_attrs_add)
	self.c_role:GetCapability():ReCalcCap(capability_type, c_attrs_add)
end

function Role:SendOperateResult(operate, result, param1, param2)
	self.c_role:SendOperateResult(operate, result, param1 or 0, param2 or 0)
end

function Role:Recover()
	self.c_role:Recover()
end

function Role:ReAlive(realive_pos_x, realive_pos_y, hp_per, mp_per)
	self.c_role:ReAlive(hp_per or 100, mp_per or 100, CFactory:CLuaPos(realive_pos_x, realive_pos_y))
end

function Role:GetCScene()
	return self.c_role:GetScene()
end

function Role:FindCEffectOtherBuff(product_id)
	return self.c_role:GetEffectList():FindOtherEffectBuff(product_id)
end

function Role:SendProtocol(protocol)
	protocol:EncodeAndSend(self.c_role:GetNetId())
end

function Role:NoticeNumStr(notice_numstr)
	local protocol = ProtocolPool.Instance:GetProtocol(SCNoticeNumStr)
	protocol.notice_numstr = notice_numstr
	self:SendProtocol(protocol)
end

function Role:WriteLog(log)
	local format_log = string.format("user %d %s %s", self:GetRoleId(), self:GetName(), log)
	C_LuaToCFun:WriteLog(format_log)
end

function Role:OnRoleLogout()
	-- body
end

function Role:OnResetData(old_dayid, now_dayid)
	for i,v in ipairs(self.object_queue) do
		if v.OnResetData then
			Log("Role:OnResetData", old_dayid, now_dayid)
			v:OnResetData(old_dayid, now_dayid)
		end
	end
end

function Role:OnDieInStaticScene(killer_id)
end

function Role:GetCommonData()
	return self:GetObject("commondata")
end

function Role:GetLingyu()
	return self:GetObject("lingyu")
end

function Role:GetTianxiangeFb()
	return self:GetObject("tianxiangefb")
end

function Role:GetChallengeFb()
	return self:GetObject("challengefb")
end