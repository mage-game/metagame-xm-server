--------------------------------------------------------------
2012-6-20 10:17:07 first build
--------------------------------------------------------------

说明： 以 ☆ 标记的目录或文件是从策划文档自动生成的


\config                             配置目录
    \gameworld
        \door                       传送门配置目录
            门ID.xml                每个传送门对应一个配置文件
            
        \drop                       掉落配置目录                    ☆
            掉落ID.xml              每个掉落对应一个配置文件        ☆
            
        \globalconfig               全局配置目录
            equipconfig.xml         装备全局配置（强化、升级、宝石等）
            keyconfig.xml           关键全局配置（AOI等）
            posthouse.xml           驿站全局配置
            role.xml                角色相关全局配置（经验、成长、武魂等）
            otherconfig.xml         其它全局配置
            
        \item                       物品配置目录                    ☆
            \buff                   buff类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \compose                合成类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \equipment              装备类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \expense                消耗类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \gift                   礼包类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \medicine               回复药品类物品配置目录          ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \position               坐标类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \stuff                  材料类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
            \other                  其它类物品配置目录              ☆
                物品ID.xml          每个物品对应一个配置文件        ☆
                                                                    
        \map                        地图配置目录                    ☆
            地图ID.xml              每个地图对应一个配置文件        ☆
            
        \scene                      场景配置目录                    ☆
            场景ID.xml              每个场景对应一个配置文件        ☆
        
        \monster                    怪物配置目录                    ☆
            怪物ID.xml              每个怪物对应一个配置文件        ☆
        
        \npc                        NPC配置目录                     ☆
            NPC ID.xml              每个NPC对应一个配置文件         ☆
        
        \pet                        宠物配置目录
            宠物ID.xml              每个宠物对应一个配置文件        
        
        \script                     脚本配置目录
            脚本名.lua              每个脚本对应一个lua文件
        
        \skill                      技能配置目录
            \monsterskills          怪物技能配置目录
                CommonSkill + ID.xml        怪物普通技
                RangeCommonSkill + ID.xml   怪物群攻技
                RangeAddHPSkill + ID.xml    怪物群辅技
            
            MonsterPetSkillManager.xml      怪物技能目录文件
            YongShiCommonSkill.xml          勇士普通技
            MouShiCommonSkill.xml        	谋士普通技
            YuShiCommonSkill.xml            羽士普通技          
            RoleSkill + 技能ID.xml          每个角色技能对应一个配置文件
        
        \soul                       武魂配置目录                    ☆
            武魂ID.xml              每个武魂对应一个配置文件        ☆  
        
        \suit                       套装配置目录
            套装ID.xml              每个套装对应一个配置文件            
        
        \task                       任务配置目录                    ☆
            \trunk                  主线任务配置目录                ☆
                任务ID.xml          每个任务对应一个配置文件        ☆
            
            \branch                 支线任务配置目录                ☆
                任务ID.xml          每个任务对应一个配置文件        ☆
            
            \daily                  日常任务配置目录                ☆
                任务ID.xml          每个任务对应一个配置文件        ☆
            
        dropmanager.xml             掉落配置目录文件                ☆
        globalconfigmanager.xml     全局配置管理配置文件
        itemmanager.xml             物品配置目录文件                ☆
        mentality.xml               元神配置文件（基础、根骨）
        monstermanager.xml          怪物配置目录文件                ☆
        petmanager.xml              宠物配置目录文件
        scenemanager.xml            场景和地图配置目录文件          ☆
        scriptmanager.xml           脚本配置目录文件
        soulmanager.xml             武魂配置目录文件                ☆
        suiltmanager.xml            套装配置目录文件
        taskmanager.xml             任务配置目录文件                ☆
    
    \globalserver
        other.xml                   全局服务器其它杂项配置
        system.xml                  全局服务器配置
        
    \loginserver
        iplimit.xml                 登录IP限制
        registerconfig.xml          注册配置 （出生点、初始值等）
        
    \serverconfig
        commconfig.xml              服务端公共配置
        string.xml                  服务端的字符串资源
        
    \sql                            数据库相关SQL
        
