
#include "asterisk.h"

ASTERISK_FILE_VERSION(__FILE__, "$Revision: 399998 $")


#include "sqlite3.h"
#include "asterisk/module.h"
#include "asterisk/channel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DBPATH "/cfg/etc/ucm_config.db"

static const char app[] = "InterfaceSql";

static int SelectDB(sqlite3 *db, const char *sql);
static int InsertDB(sqlite3 *db, const char *sql);
static int UpdateDB(sqlite3 *db, const char *sql);
static int DeleteDB(sqlite3 *db, const char *sql);


static int SelectDB(sqlite3 *db, const char *sql)
{
	int err = 0;
	sqlite3_stmt* stmt = NULL;
	const char *data = NULL;
	int count = 0, iCol = 0;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'S' && temp != 's')
	{
		ast_log(LOG_WARNING, "Not Select language! Update failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Select prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		count = sqlite3_column_count(stmt);
		for(iCol = 0; iCol < count; iCol++)
		{
			data = (const char*)sqlite3_column_text(stmt, iCol);
			if(0 == iCol)
				printf("%s ", data ? data : "[NULL]");
			else
				printf("| %s ", data ? data : "[NULL]");
		}
		printf("\n");
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int UpdateDB(sqlite3 *db, const char *sql)
{
	int err = 0;
	sqlite3_stmt* stmt = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'U' && temp != 'u')
	{
		ast_log(LOG_WARNING, "Not update language! Update failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Update prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		ast_log(LOG_WARNING, "Update data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int DeleteDB(sqlite3 *db, const char *sql)
{
	int err = 0;
	sqlite3_stmt* stmt = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'D' && temp != 'd')
	{
		ast_log(LOG_WARNING, "Not delete language! Delete failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Delete prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		ast_log(LOG_WARNING, "Delete data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int InsertDB(sqlite3 *db, const char *sql)
{
	int err = 0;
	sqlite3_stmt* stmt = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'i' && temp != 'I')
	{
		ast_log(LOG_WARNING, "Not insert language! Insert failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		ast_log(LOG_WARNING, "Insert prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		ast_log(LOG_WARNING, "Insert data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}


static int interface_exec(struct ast_channel *chan, const char *data)
{
	int err = 0;
	sqlite3* db = NULL;
	
	char *argv = NULL;
	char temp = 0;

	if(!data)
		return -1;
	
	err = sqlite3_open(DBPATH, &db);
    if(SQLITE_OK != err)
    {   
    	ast_log(LOG_WARNING, "connect db failed!\n");
		sqlite3_close(db);
        return -1; 
    }   

	argv = strdup(data);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp == 'i' || temp == 'I')
		InsertDB(db, data);
	else if(temp == 'd' || temp == 'D')
		DeleteDB(db, data);
	else if(temp == 'u' || temp == 'U')
		UpdateDB(db, data);
	else if(temp == 's' || temp == 'S')
		SelectDB(db, data);
	else{
		ast_log(LOG_WARNING, "sql language failed!\n");
		return -1;
	}

	sqlite3_close(db);
	return 0;
}

static int unload_module(void)

{
    return ast_unregister_application(app);
}

static int load_module(void)
{
    if(ast_register_application_xml(app, interface_exec))
        return AST_MODULE_LOAD_FAILURE;
    return AST_MODULE_LOAD_SUCCESS;
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Interface exec Applications");
