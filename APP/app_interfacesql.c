
#include "sqlite3.h"
//#include "asterisk/module.h"
//#include "asterisk/channel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char app[] = "Interfacesql";

int UpdateDB(sqlite3 *db, const char *sql)
{
	int err;
	sqlite3_stmt* stmt = NULL;
	char *argv, *res;
	const char *data = NULL;

	argv = strdup(sql);
	res = strsep(&argv, " ");

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(('s' == res[0] || 'S' == res[0]) && SQLITE_ROW == err)
	{
		sqlite3_reset(stmt);
		while(sqlite3_step(stmt) == SQLITE_ROW)
		{
			int count = sqlite3_column_count(stmt);
			int i = 0;
			for(i = 0; i < count; i++)
			{
				data = (const char*)sqlite3_column_text(stmt, i);
				if(0 == i)
					printf("%s ", data ? data : "[NULL]");
				else
					printf("| %s ", data ? data : "[NULL]");
			}
			printf("\n");
		}
	}
	else if(SQLITE_DONE != err)
	{
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

int main(int argc, char *argv[])
{
	int err = 0;
	char path[] = "database.sqlite3";
	sqlite3 * db;
	err = sqlite3_open(path, &db);
	if(SQLITE_OK != err)
	{
		return -1;
	}
	UpdateDB(db, "select str from xx");
	UpdateDB(db, "insert into xx values ('789','kkk','pop')");
//	UpdateDB(db, "update xx set str='007' where sss='lll'");
	UpdateDB(db, "delete from xx where str='789'");
	printf("----------!!----------\n");
	UpdateDB(db, "create table nimei(woqu TEXT)");
	UpdateDB(db, "select * from xx");
	UpdateDB(db, "select * from tt");

	return 0;
}
/*static int interface_exec(struct ast_channel *chan, const char *data)
{
}

static int unload_modele(void)
{
	return ast_unregister_application(app);
}

static int load_module(void)
{
	if(ast_register_application_xml(app, interface_exec))
		return AST_MODULE_LOAD_FAILURE;
	return AST_MODULE_LOAD_SUCCESS;
}

AST_MODULE_INFO_STANDARD(ASTERISK_GPL_KEY, "Interfacesql Application");
*/
