

#include "tablefactroy.h"

#include "dbopera/dataadapter.h"
#include "dbopera/dbcommand.h"
#include "cache/cachecommand.h"

#include "tabledef/tabledef.h"
#include "tabledef/registertables.h"
#include "tabledef/tableid.h"

namespace tabledef
{

CommandManager::OperatorList GetTableFactroy()
{
	CommandManager::OperatorList operalist;
	CommandManager::TableOperator oper;
	TableInitArg arg;


	arg = GetaccountgoldTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<accountgold>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ACCOUNTGOLD;
	operalist.push_back(oper);

	arg = GetaccountidentityTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<accountidentity>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ACCOUNTIDENTITY;
	operalist.push_back(oper);

	arg = Getblack_listsTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<black_lists>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_BLACK_LISTS;
	operalist.push_back(oper);

	arg = GetcampTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<camp>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_CAMP;
	operalist.push_back(oper);

	arg = GetchallengeruserTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<challengeruser>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_CHALLENGERUSER;
	operalist.push_back(oper);

	arg = GetcommandTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<command>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_COMMAND;
	operalist.push_back(oper);

	arg = Getcross_personrankTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<cross_personrank>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_CROSS_PERSONRANK;
	operalist.push_back(oper);

	arg = GetcrossrecordTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<crossrecord>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_CROSSRECORD;
	operalist.push_back(oper);

	arg = GetcrossstatusTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<crossstatus>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_CROSSSTATUS;
	operalist.push_back(oper);

	arg = GetdailyfindTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<dailyfind>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_DAILYFIND;
	operalist.push_back(oper);

	arg = GetenemyTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<enemy>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ENEMY;
	operalist.push_back(oper);

	arg = GetfishpoolTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<fishpool>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_FISHPOOL;
	operalist.push_back(oper);

	arg = GetfriendsTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<friends>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_FRIENDS;
	operalist.push_back(oper);

	arg = GetglobalroleTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<globalrole>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_GLOBALROLE;
	operalist.push_back(oper);

	arg = GetguildTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<guild>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_GUILD;
	operalist.push_back(oper);

	arg = GetguildapplyforTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<guildapplyfor>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_GUILDAPPLYFOR;
	operalist.push_back(oper);

	arg = GetguildeventTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<guildevent>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_GUILDEVENT;
	operalist.push_back(oper);

	arg = GetguildmemberTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<guildmember>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_GUILDMEMBER;
	operalist.push_back(oper);

	arg = GethotkeyTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<hotkey>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_HOTKEY;
	operalist.push_back(oper);

	arg = GetitemlistTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<itemlist>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ITEMLIST;
	operalist.push_back(oper);

	arg = GetloginTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<login>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_LOGIN;
	operalist.push_back(oper);

	arg = GetmailTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<mail>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_MAIL;
	operalist.push_back(oper);

	arg = GetofflineregisterTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<offlineregister>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_OFFLINEREGISTER;
	operalist.push_back(oper);

	arg = GetpersonrankTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<personrank>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_PERSONRANK;
	operalist.push_back(oper);

	arg = GetpublicnoticeTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<publicnotice>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_PUBLICNOTICE;
	operalist.push_back(oper);

	arg = GetpublicsaleTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<publicsale>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_PUBLICSALE;
	operalist.push_back(oper);

	arg = GetroleTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE;
	operalist.push_back(oper);

	arg = Getrole_attr_detailTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL;
	operalist.push_back(oper);

	arg = Getrole_attr_detail10TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail10>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL10;
	operalist.push_back(oper);

	arg = Getrole_attr_detail2TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail2>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL2;
	operalist.push_back(oper);

	arg = Getrole_attr_detail3TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail3>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL3;
	operalist.push_back(oper);

	arg = Getrole_attr_detail4TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail4>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL4;
	operalist.push_back(oper);

	arg = Getrole_attr_detail5TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail5>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL5;
	operalist.push_back(oper);

	arg = Getrole_attr_detail6TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail6>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL6;
	operalist.push_back(oper);

	arg = Getrole_attr_detail7TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail7>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL7;
	operalist.push_back(oper);

	arg = Getrole_attr_detail8TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail8>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL8;
	operalist.push_back(oper);

	arg = Getrole_attr_detail9TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_attr_detail9>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ATTR_DETAIL9;
	operalist.push_back(oper);

	arg = Getrole_id_mapTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_id_map>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_ID_MAP;
	operalist.push_back(oper);

	arg = Getrole_name_mapTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<role_name_map>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLE_NAME_MAP;
	operalist.push_back(oper);

	arg = GetroleskillTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<roleskill>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_ROLESKILL;
	operalist.push_back(oper);

	arg = GetspacerecordTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<spacerecord>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_SPACERECORD;
	operalist.push_back(oper);

	arg = GetsystemmailTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<systemmail>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_SYSTEMMAIL;
	operalist.push_back(oper);

	arg = GetteamrankTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<teamrank>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_TEAMRANK;
	operalist.push_back(oper);

	arg = GettuodanTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<tuodan>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_TUODAN;
	operalist.push_back(oper);

	arg = GetworldstatusTableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<worldstatus>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_WORLDSTATUS;
	operalist.push_back(oper);

	arg = Getworldstatus2TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<worldstatus2>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_WORLDSTATUS2;
	operalist.push_back(oper);

	arg = Getworldstatus3TableDef();
	oper.table = new MataTableDef();
	oper.command = new DBCommand();
	oper.cache = new CacheCommand<worldstatus3>();
	oper.table->Init(arg);
	oper.table->m_table_id = TABLE_WORLDSTATUS3;
	operalist.push_back(oper);

	return operalist;
}
}

