-- 通知文字信息
SCNoticeNumStr = SCNoticeNumStr or BaseClass(BaseProtocolStruct)
function SCNoticeNumStr:__init()
	self.msg_type = 15000
	self.notice_numstr = ""
end

function SCNoticeNumStr:Encode(c_datastream)
	c_datastream:WriteStrN(self.notice_numstr, 256)
end
