
#define AST_MODULE_LOG "dbinterface"

#include "asterisk.h"

//ASTERISK_FILE_VERSION(__FILE__, "$Revision: 399998 $")


#include "sqlite3.h"
#include "asterisk/module.h"
#include "asterisk/channel.h"
#include "asterisk/pbx.h"

struct res_row{
	struct res_sql* rowdata;
	struct res_row* next;
};
struct res_sql{
	char dbdata[128];
	struct res_sql* next;
};

static int SelectDB(struct ast_channel *chan, const char *path, char *sql, struct ast_str **buf, ssize_t len)
{
	int err = 0;
	sqlite3 * db = NULL;
	char *szErrmsg = NULL;
	int nRow = 0, nColumn = 0;
	char** dbResult;

	err = sqlite3_open(path, &db);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "DB open error!\n");
	
	}

	err = sqlite3_get_table(db, sql, &dbResult, &nRow, &nColumn, &szErrMsg);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Select failed!\n");
		return -1;
	}

	sqlite3_close(db);
	return 0;
}

static int UpdateDB(struct ast_channel *chan, const char *cmd, char *path, const char *sql)
{

	int err = 0;
	char *szErrMsg = NULL;   
	sqlite3 *db = NULL;

	err = sqlite3_open(path, &db);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "DB open error!\n");
		sqlite3_close(db);
		return -1;
	}
	sqlite3_busy_timeout(db, 5000);

	err = sqlite3_exec(db,"BEGIN IMMEDIATE", NULL,NULL, &szErrMsg);  
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "begin error!\n");
		sqlite3_free(szErrMsg);  
		sqlite3_close(db);
		return -1;
	}

	err = sqlite3_exec(db, sql, NULL, NULL, &szErrMsg);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Update failed!\n");
		sqlite3_free(szErrMsg); 
		sqlite3_close(db);
		return -1;
	}
	
	err = sqlite3_exec(db,"COMMIT", NULL,NULL, &szErrMsg);  
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "commit error!\n");
		sqlite3_exec(db,"ROLLBACK", NULL,NULL, &szErrMsg);
		sqlite3_free(szErrMsg);  
		sqlite3_close(db);
		return -1;
	}
	
	sqlite3_close(db);						
	return 0;

}

static struct ast_custom_function dbinterface_function = {
	.name = "DBINTERFACE",
	.read2 = SelectDB,
	.write = UpdateDB,
};


static int unload_module(void)
{
    return ast_custom_function_unregister(&dbinterface_function);
}

static int load_module(void)
{
	return ast_custom_function_register(&dbinterface_function);
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Interface exec Applications");
