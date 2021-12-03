#include "servercommon/mempool/mempoolmanager.h"

// obj部分
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/character/battlefieldshenshi.hpp"
#include "obj/character/monsterhalo.hpp"
#include "obj/otherobj/fallingitem.h"
#include "obj/otherobj/scenearea.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/effectobj.h"
#include "obj/otherobj/trigger.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "obj/character/marryobj.hpp"
#include "obj/character/roleshadow.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "obj/character/roleclone.hpp"
REGISTER_MEMPOOLMGR(gamemempool, Role, 64, "Role")
REGISTER_MEMPOOLMGR(gamemempool, Monster, 64, "Monster")
REGISTER_MEMPOOLMGR(gamemempool, FallingItem, 64, "FallingItem")
REGISTER_MEMPOOLMGR(gamemempool, SceneArea, 16, "SceneArea")
REGISTER_MEMPOOLMGR(gamemempool, GatherObj, 64, "GatherObj")
REGISTER_MEMPOOLMGR(gamemempool, EffectObj, 64, "EffectObj")
REGISTER_MEMPOOLMGR(gamemempool, Trigger, 64, "Trigger")
REGISTER_MEMPOOLMGR(gamemempool, BigCoinObj, 64, "BigCoinObj")
REGISTER_MEMPOOLMGR(gamemempool, BattleFieldShenShi, 64, "BattleFieldShenShi")
REGISTER_MEMPOOLMGR(gamemempool, MonsterHalo, 64, "MonsterHalo")
REGISTER_MEMPOOLMGR(gamemempool, MarryObj, 64, "MarryObj")
REGISTER_MEMPOOLMGR(gamemempool, RoleShadow, 64, "RoleShadow")
REGISTER_MEMPOOLMGR(gamemempool, WorldEventObj, 64, "WorldEventObj")
REGISTER_MEMPOOLMGR(gamemempool, RoleClone, 64, "RoleClone")

#include "other/role_module.hpp"
REGISTER_MEMPOOLMGR(gamemempool, RoleModule, 64, "RoleModule")
#include "gameworld/role_module/roleothermgr.h"
REGISTER_MEMPOOL(gamemempool, RoleOtherMgr, 64, "RoleOtherMgr");

// scene部分
#include "scene/scene.h"
#include "scene/generater/monstergenerater.h"
#include "scene/generater/gathergenerater.h"
#include "scene/changescenedelayer.h"
#include "monster/createmonsterdelayer.h"
#include "other/fb/towerdefendteamfunc.hpp"

REGISTER_MEMPOOLMGR(gamemempool, Scene, 16, "Scene")
REGISTER_MEMPOOLMGR(gamemempool, MonsterGenerater, 64, "MonsterGenerater")
REGISTER_MEMPOOLMGR(gamemempool, GatherGenerater, 32, "GatherGenerater")
REGISTER_MEMPOOLMGR(gamemempool, ChangeSceneDelayer, 32, "ChangeSceneDelayer")
REGISTER_MEMPOOLMGR(gamemempool, CreateMonsterDelayer, 32, "CreateMonsterDelayer")
REGISTER_MEMPOOLMGR(gamemempool, RealiveDelayer, 32, "RealiveDelayer")

// aoi部分
#include "scene/aoi/obseritem.h"
REGISTER_MEMPOOLMGR(gamemempool, ObserItem, 64, "ObserItem")

// 装备部分
#include "equipment/equipment.h"
REGISTER_MEMPOOLMGR(gamemempool, EquipmentParam, 64, "EquipmentParam")

// rmi 部分
#include "rmibackobjdef.h"
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleInitBackObjectImpl, 64, "RMIRoleInitBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleSaveBackObjImplLocal, 64, "RMIRoleSaveBackObjImplLocal")
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleSaveBackObjImplRemote, 64, "RMIRoleSaveBackObjImplRemote")
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleSaveBackObjectImplRole, 64, "RMIRoleSaveBackObjectImplRole")
REGISTER_MEMPOOLMGR(gamemempool, RMIGetGoldBackObjectImpl, 64, "RMIGetGoldBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIGetGoldBackObjectImplChange, 64, "RMIGetGoldBackObjectImplChange")
REGISTER_MEMPOOLMGR(gamemempool, RMIChangeGoldBackObjectImpl, 64, "RMIChangeGoldBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIChangeGoldBackObjectImplErrorAdd, 64, "RMIChangeGoldBackObjectImplErrorAdd")
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleSaveStatusBackObjectImpl, 64, "RMIRoleSaveStatusBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIQueryNameExistBackObjectImpl, 64, "RMIQueryNameExistBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIResetNameBackObjectImpl, 64, "RMIResetNameBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIRoleQueryBackObjectImpl, 64, "RMIRoleQueryBackObjectImpl")

// NPC
#include "npc/npc.h"

REGISTER_MEMPOOLMGR(gamemempool, NPC, 16, "NPC");

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectcounter.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"
#include "effect/skilleffect/effecttalenthpstore.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effecthurtdec.hpp"
#include "effect/skilleffect/effecthurtinc.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectbomb.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmojiewudiprotect.hpp"
#include "effect/skilleffect/effectmodifyglobalcd.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectrecoveronatked.hpp"
#include "effect/skilleffect/effectgradualhurt.hpp"
#include "effect/skilleffect/effectjuhuapig.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectattrhurtrandinc.hpp"
#include "effect/skilleffect/effectbianshen.hpp"
#include "effect/skilleffect/effectmpnocost.hpp"
#include "effect/skilleffect/effectshenbing.hpp"
#include "effect/skilleffect/effectabsorbhp.hpp"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"
#include "effect/skilleffect/effectattrcontainer.hpp"
#include "effect/skilleffect/effectfire.hpp"
#include "effect/skilleffect/effectdeadlyattack.hpp"
#include "effect/skilleffect/effecthspecialshield.hpp"
#include "effect/skilleffect/effectattrstore.hpp"
#include "effect/skilleffect/effectabsorbhpbeenattack.hpp"
#include "effect/skilleffect/effectforbidrecover.hpp"

REGISTER_MEMPOOLMGR(gamemempool, EffectBuff, 16, "EffectBuff");
REGISTER_MEMPOOLMGR(gamemempool, EffectCounter, 16, "EffectCounter");
REGISTER_MEMPOOLMGR(gamemempool, EffectDoubleHurt, 16, "EffectDoubleHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectHpStore, 16, "EffectHpStore");
REGISTER_MEMPOOLMGR(gamemempool, EffectTalentHpStore, 16, "EffectTalentHpStore");
REGISTER_MEMPOOLMGR(gamemempool, EffectDecHurt, 16, "EffectDecHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectIncHurt, 16, "EffectIncHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectInterval, 16, "EffectInterval");
REGISTER_MEMPOOLMGR(gamemempool, EffectMoveSpeed, 16, "EffectMoveSpeed");
REGISTER_MEMPOOLMGR(gamemempool, EffectOnce, 16, "EffectOnce");
REGISTER_MEMPOOLMGR(gamemempool, EffectSpecialState, 16, "EffectSpecialState");
REGISTER_MEMPOOLMGR(gamemempool, EffectBomb, 16, "EffectBomb");
REGISTER_MEMPOOLMGR(gamemempool, EffectWudiProtect, 16, "EffectWudiProtect");
REGISTER_MEMPOOLMGR(gamemempool, EffectModifyGlobalCD, 16, "EffectModifyGlobalCD");
REGISTER_MEMPOOLMGR(gamemempool, EffectReboundHurt, 16, "EffectReboundHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectRecoverOnAtked, 16, "EffectRecoverOnAtked");
REGISTER_MEMPOOLMGR(gamemempool, EffectGradualHurt, 16, "EffectGradualHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectJuhuaPig, 16, "EffectJuhuaPig");
REGISTER_MEMPOOLMGR(gamemempool, EffectInvisible, 16, "EffectInvisible");
REGISTER_MEMPOOLMGR(gamemempool, EffectAttrRandIncHurt, 16, "EffectAttrRandIncHurt");
REGISTER_MEMPOOLMGR(gamemempool, EffectBianshen, 16, "EffectBianshen");
REGISTER_MEMPOOLMGR(gamemempool, EffectMPNoCost, 16, "EffectMPNoCost");
REGISTER_MEMPOOLMGR(gamemempool, EffectShenbing, 16, "EffectShenbing");
REGISTER_MEMPOOLMGR(gamemempool, EffectAbsorbHP, 16, "EffectAbsorbHP");
REGISTER_MEMPOOLMGR(gamemempool, EffectPercentBuff, 16, "EffectPercentBuff");
REGISTER_MEMPOOLMGR(gamemempool, EffectOtherBuff, 16, "EffectOtherBuff");
REGISTER_MEMPOOLMGR(gamemempool, EffectAttrContainer, 16, "EffectAttrContainer");
REGISTER_MEMPOOLMGR(gamemempool, EffectAttrStore, 16, "EffectAttrStore");
REGISTER_MEMPOOLMGR(gamemempool, EffectFire, 16, "EffectFire");
REGISTER_MEMPOOLMGR(gamemempool, EffectSpecialShield, 16, "EffectSpecialShield");
REGISTER_MEMPOOLMGR(gamemempool, EffectDeadlyAttack, 16, "EffectDeadlyAttack");
REGISTER_MEMPOOLMGR(gamemempool, EffectAbsorbHPBeenAttack, 16, "EffectAbsorbHPBeenAttack");
REGISTER_MEMPOOLMGR(gamemempool, EffectForbidRecover, 16, "EffectForbidRecover");
REGISTER_MEMPOOLMGR(gamemempool, EffectMojieWudiProtect, 16, "EffectMojieWudiProtect");

// 场景buff
#include "effect/sceneeffect/sceneeffect.hpp"
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectSpecialFaZhen, 16, "SceneEffectSpecialFaZhen");
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectHurt, 16, "SceneEffectHurt");
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectTowerDefendFaZhen, 16, "SceneEffectTowerDefendFaZhen");
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectFaZhen121, 16, "SceneEffectFaZhen121");
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectFaZhen421, 16, "SceneEffectFaZhen421");
REGISTER_MEMPOOLMGR(gamemempool, SceneEffectClient, 16, "SceneEffectClient");

// ai部分
#include "ai/monsterai/monsteraistaticinitiative.hpp"
#include "ai/monsterai/monsteraiinitiative.hpp"
#include "ai/monsterai/monsteraifool.hpp"
#include "ai/monsterai/monsteraistatic.hpp"
#include "ai/monsterai/monsteraipassive.hpp"
#include "ai/monsterai/monsteraijustrun.hpp"
#include "ai/monsterai/monsteraijustflee.hpp"
#include "ai/monsterai/monsteraiaddmianshangbuff.hpp"
#include "ai/monsterai/monsteraiaddrecoverbuff.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIStaticInitiative, 64, "MonsterAIStaticInitiative");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIInitiative, 64, "MonsterAIInitiative")
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIFool, 64, "MonsterAIFool")
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIStatic, 64, "MonsterAIStatic");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIPassive, 64, "MonsterAIPassive");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIMoveAround, 64, "MonsterAIMoveAround");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIJustRun, 64, "MonsterAIJustRun");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIJustFlee, 64, "MonsterAIJustFlee");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIAddMianshanBuff, 64, "MonsterAIAddMianshanBuff");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIAddRecoverBuff, 64, "MonsterAIAddRecoverBugg");

#include "ai/monsterai/specialai/monsterailingshanxianqi.hpp"
#include "ai/monsterai/specialai/monsteraitowerdefendteam.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAILingShanXianQi, 64, "MonsterAILingShanXianQi");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAITowerDefendTeam, 64, "MonsterAITowerDefendTeam");

#include "ai/monsterai/specialai/monsteraiguildtowerdefendattack.hpp"
#include "ai/monsterai/specialai/monsteraiguildtowerdefendmt.hpp"
#include "ai/monsterai/specialai/monsteraiguildtowerdefendshouhu.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGuildTowerDefendAttack, 64, "MonsterAIGuildTowerDefendAttack");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGuildTowerDefendMT, 64, "MonsterAIGuildTowerDefendMT");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGuildTowerDefendShouHu, 64, "MonsterAIGuildTowerDefendShouHu");

#include "ai/monsterai/specialai/monsteraibattlefield.hpp"
#include "ai/monsterai/specialai/monsteraigongchengzhan.hpp"
#include "ai/monsterai/specialai/monsteraigterritorywartower.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIBattleField, 64, "MonsterAIBattleField");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGongchengzhan, 64, "MonsterAIGongchengzhan");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAITerritoryWarTower, 64, "MonsterAITerritoryWar");

#include "ai/monsterai/specialai/monsteraicishacamp1.hpp"
#include "ai/monsterai/specialai/monsteraicishacamp2.hpp"
#include "ai/monsterai/specialai/monsteraicishacamp3.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAiCishaCamp1, 64, "MonsterAiCishaCamp1");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAiCishaCamp2, 64, "MonsterAiCishaCamp2");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAiCishaCamp3, 64, "MonsterAiCishaCamp3");

#include "ai/monsterai/specialai/monsteraixianmengzhanareaboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhanguardboss.hpp"
#include "ai/monsterai/specialai/monsteraixianmengzhandanyaoboss.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIXianMengZhanAreaBoss, 64, "MonsterAIXianMengZhanAreaBoss");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIXianMengZhanGuardBoss, 64, "MonsterAIXianMengZhanGuardBoss");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIXianMengZhanDanYaoBoss, 64, "MonsterAIXianMengZhanDanYaoBoss");

#include "ai/monsterai/specialai/monsteraiguildfbenemy.hpp"
#include "ai/monsterai/specialai/monsteraiguildfbguard.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGuildFbGuard, 64, "MonsterAIGuildFbGuard");
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGuildFbEnemy, 64, "MonsterAIGuildFbEnemy");

#include "ai/monsterai/specialai/monsteraicoin.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAICoin, 64, "MonsterAICoin");

#include "ai/monsterai/specialai/monsteraicrosspastureanimal.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAICrossPastureAnimal, 64, "MonsterAICrossPastureAnimal");

#include "ai/monsterai/specialai/monsterairunningmonster.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIRunningMonster, 64, "MonsterAIRunningMonster");

#include "ai/monsterai/specialai/monsteraibuildtower.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIBuildTower, 64, "MonsterAIBuildTower");

#include "ai/monsterai/specialai/monsteraigiftharvest.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MonsterAIGiftHarvest, 64, "MonsterAIGiftHarvest");

// 机器人AI
#include "ai/roleai/roleaifool.hpp"
#include "ai/roleai/roleaichallengefile.hpp"
#include "ai/roleai/roleaiworldevent.hpp"
#include "ai/roleai/roleairoleboss.hpp"
#include "ai/roleai/roleaicampdefend.hpp"
#include "ai/roleai/roleaielementfield.hpp"
#include "ai/roleai/roleairoleclone.hpp"
REGISTER_MEMPOOLMGR(gamemempool, RoleAIFool, 64, "RoleAIFool");
REGISTER_MEMPOOLMGR(gamemempool, RoleAIChallengefile, 64, "RoleAIChallengefile");
REGISTER_MEMPOOLMGR(gamemempool, RoleAIWorldEvent, 64, "RoleAIWorldEvent");
REGISTER_MEMPOOLMGR(gamemempool, RoleAIRoleBoss, 64, "RoleAIRoleBoss");
REGISTER_MEMPOOLMGR(gamemempool, RoleAICampDefend, 64, "RoleAICampDefend");
REGISTER_MEMPOOLMGR(gamemempool, RoleAIElementField, 64, "RoleAIElementField");
REGISTER_MEMPOOLMGR(gamemempool, RoleAIRoleClone, 64, "RoleAIRoleClone");

// chat
#include "chat/channel.h"
REGISTER_MEMPOOLMGR(gamemempool, Channel, 64, "Channel");

#include "chat/msgboarditem.h"
REGISTER_MEMPOOLMGR(gamemempool, MsgBoardItem, 64, "MsgBoardItem");

// 邮件
#include "global/mail/mailitem.hpp"
#include "global/mail/usermail.hpp"
REGISTER_MEMPOOLMGR(gamemempool, MailItem, 64, "MailItem");
REGISTER_MEMPOOLMGR(gamemempool, UserMail, 64, "UserMail");

#include "global/team/team.hpp"
REGISTER_MEMPOOLMGR(gamemempool, Team, 64, "Team");

#include "global/offlineregister/offlineregister.hpp"
REGISTER_MEMPOOLMGR(gamemempool, OfflineRegister, 64, "OfflineRegister")

#include "global/guild/impl/guildmember.hpp"
REGISTER_MEMPOOLMGR(gamemempool, GuildMember, 64, "GuildMember")

#include "global/guild/guild.hpp"
REGISTER_MEMPOOLMGR(gamemempool, Guild, 64, "Guild")

#include "global/trade/publicsaleitem.hpp"
#include "global/trade/userpublicsale.hpp"
REGISTER_MEMPOOLMGR(gamemempool, PublicSaleItem, 64, "PublicSaltItem")
REGISTER_MEMPOOLMGR(gamemempool, UserPublicSale, 64, "UserPublicSale")

#include "global/rmibackobjdef.h"

//
REGISTER_MEMPOOLMGR(gamemempool, RMISaveCampBackObjectImpl, 64, "RMISaveCampOfficialBackObjectImpl");
REGISTER_MEMPOOLMGR(gamemempool, RMIInitCampBackObjectImpl, 64, "RMIInitCampBackObjectImpl");
//
REGISTER_MEMPOOLMGR(gamemempool, RMISaveMailBackObjectImpl, 64, "RMISaveMailBackObjectImpl");
REGISTER_MEMPOOLMGR(gamemempool, RMIInitMailsBackObjectImpl, 64, "RMIInitMailsBackObjectImpl");
REGISTER_MEMPOOLMGR(gamemempool, RMIGetSystemMailsBackObjectImpl, 64, "RMIGetSystemMailsBackObjectImpl");
REGISTER_MEMPOOLMGR(gamemempool, RMIRemoveSystemMailsBackObjectImpl, 64, "RMIRemoveSystemMailsBackObjectImpl");


REGISTER_MEMPOOLMGR(gamemempool, RMIInitPersonRankBackObjectImpl, 64, " RMIInitPersonRankBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISavePersonRankBackObjectImpl, 64, " RMISavePersonRankBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitTuodanBackObjectImpl, 64, " RMIInitTuodanBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveTuodanBackObjectImpl, 64, " RMISaveTuodanBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitTeamRankBackObjectImpl, 64, " RMIInitTeamRankBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveTeamRankBackObjectImpl, 64, " RMISaveTeamRankBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitWorldStatusBackObjectImpl, 64, " RMIInitWorldStatusBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveWorldStatusBackObjectImpl, 64, " RMISaveWorldStatusBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitOfflineRegisterBackObjectImpl, 64, " RMIInitOfflineRegisterBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveOfflineRegisterBackObjectImpl, 64, " RMISaveOfflineRegisterBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitSpaceRecordBackObjectImpl, 64, " RMIInitSpaceRecordBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveSpaceRecordBackObjectImpl, 64, " RMISaveSpaceRecordBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitChallengeUserObjectImpl, 64, " RMIInitChallengeUserObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveChallengeUserObjectImpl, 64, " RMISaveChallengeUserObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMICreateGuildBackObjectImpl, 64, "RMICreateGuildBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIDismissGuildBackObjectImpl, 64, "RMIDismissGuildBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIInitGuildBackObjectImpl, 64, "RMIInitGuildBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveGuildBackObjectImpl, 64, "RMISaveGuildBackObjectImpl")


REGISTER_MEMPOOLMGR(gamemempool, RMISavePublicSaleBackObjectImpl, 64, "RMISavePublicSaleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIInitPublicSaleBackObjectImpl, 64, "RMIInitPublicSaleBackObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitFishPoolObjectImpl, 64, " RMIInitFishPoolObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveFishPoolObjectImpl, 64, " RMISaveFishPoolObjectImpl")

#include "global/challengefield/challengeuser.hpp"
REGISTER_MEMPOOLMGR(gamemempool, ChallengeUser, 64, "ChallengeUser")

#include "global/crosschallengefield/crosschallengeuser.hpp"
REGISTER_MEMPOOLMGR(gamemempool, CrossChallengeUser, 64, "CrossChallengeUser")

#include "forcekickuserdelay.hpp"
REGISTER_MEMPOOLMGR(gamemempool, ForceKickUserDelay, 64, "ForceKickerUserDelay")

#include "global/fishpool/fishpool.hpp"
REGISTER_MEMPOOLMGR(gamemempool, FishPool, 64, "FishPool")

#include "rmiclient/rmigroleclient.h"
REGISTER_MEMPOOLMGR(gamemempool, RMIGCreateCrossRoleBackObjectImpl, 64, "RMIGCreateCrossRoleBackObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMIGRoleSaveBackObjectImplCrossRole, 64, "RMIGRoleSaveBackObjectImplCrossRole")

#include "global/cross/crossactivity/crossrecord.hpp"
REGISTER_MEMPOOLMGR(gamemempool, CrossRecord, 64, "CrossRecord")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitCrossRecordObjectImpl, 64, " RMIInitCrossRecordObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveCrossRecordObjectImpl, 64, " RMISaveCrossRecordObjectImpl")

REGISTER_MEMPOOLMGR(gamemempool, RMIInitGlobalRoleObjectImpl, 64, "RMIInitGlobalRoleObjectImpl")
REGISTER_MEMPOOLMGR(gamemempool, RMISaveGlobalRoleObjectImpl, 64, "RMISaveGlobalRoleObjectImpl")

REGISTER_MEMPOOL(gamemempool, RMIInitWorldStatus2BackObjectImpl, 64, " RMIInitWorldStatus2BackObjectImpl")
REGISTER_MEMPOOL(gamemempool, RMISaveWorldStatus2BackObjectImpl, 64, " RMISaveWorldStatus2BackObjectImpl")

REGISTER_MEMPOOL(gamemempool, RMIInitWorldStatus3BackObjectImpl, 64, " RMIInitWorldStatus3BackObjectImpl")
REGISTER_MEMPOOL(gamemempool, RMISaveWorldStatus3BackObjectImpl, 64, " RMISaveWorldStatus3BackObjectImpl")

REGISTER_MEMPOOL(gamemempool, RMISwapRoleBackObjectImpl, 16, " RMISwapRoleBackObjectImpl")

