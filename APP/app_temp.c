
#include "sqlite3.h"
//#include "asterisk/module.h"
//#include "asterisk/channel.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char app[] = "Interfacesql";

static int SelectDB(sqlite3 *db, const char *sql)
{
	int err;
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
		printf("Not Select language! Update failed!\n");
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		printf("Select prepare failed!\n");
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
	int err;
	sqlite3_stmt* stmt = NULL;
	const char *data = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'U' && temp != 'u')
	{
		printf("Not update language! Update failed!\n");
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		printf("Update prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		printf("Update data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int DeleteDB(sqlite3 *db, const char *sql)
{
	int err;
	sqlite3_stmt* stmt = NULL;
	const char *data = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'D' && temp != 'd')
	{
		printf("Not delete language! Delete failed!\n");
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		printf("Delete prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		printf("Delete data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int InsertDB(sqlite3 *db, const char *sql)
{
	int err;
	sqlite3_stmt* stmt = NULL;
	const char *data = NULL;
	char *argv = NULL;
	char temp = 0;

	argv = strdup(sql);
	while( (temp = *argv) == ' ')
	{
		argv++;
	}
	if(temp != 'i' && temp != 'I')
	{
		printf("Not insert language! Insert failed!\n");
		return -1;
	}

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		printf("Insert prepare failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		printf("Insert data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}

	sqlite3_finalize(stmt);
	return 0;
}

static int OperateDB(sqlite3 *db, const char *sql)
{
	int err;
	sqlite3_stmt* stmt = NULL;
	const char *data = NULL;

	err = sqlite3_prepare(db, sql, -1, &stmt, NULL);
	if(SQLITE_OK != err)
	{
		printf("operate <%s> failed!\n", sql);
		sqlite3_finalize(stmt);
		return -1;
	}

	err = sqlite3_step(stmt);
	if(SQLITE_DONE != err)
	{
		printf("Operate data failed!\n");
		sqlite3_finalize(stmt);
		return -1;
	}
	printf("<%s> success!\n", sql);

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
	SelectDB(db, "select str from xx");
	InsertDB(db, "insert  or replace into xx values ('007','kkk','5pp')");
	UpdateDB(db, "update xx set str='007' where sss='lll'");
	DeleteDB(db, "delete from xx where str='789'");
	printf("----------!!----------\n");
	OperateDB(db, "create table if not exists nimei(woqu TEXT)");
	SelectDB(db, "select * from xx");

	return 0;
}

