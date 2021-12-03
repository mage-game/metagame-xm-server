MemManager = MemManager or BaseClass(BaseHotUpdate)
function MemManager:__init()
	if MemManager.Instance then
		ErrorLog("[MemManager] Attemp to create a singleton twice !")
	end
	
	MemManager.Instance = self
	collectgarbage("setpause", 100)
	collectgarbage("setstepmul", 5000)

	self:InitDataset()
end

function MemManager:__delete()
	MemManager.Instance = nil
end


function MemManager:InitDataset()
	self.dataset.monitor = false
end

function MemManager:Update(interval, now_second)
	if self.dataset.monitor then
		Log("lua mem:", collectgarbage("count"))
	end
end

function MemManager:OpenMonitor()
	self.dataset.monitor = true
end

function MemManager:CloseMonitor()
	self.dataset.monitor = false
end

function MemManager:Collect()
	collectgarbage("collect")
	Log("lua mem:", collectgarbage("count"))
end