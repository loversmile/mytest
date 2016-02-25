#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>

int GetTable()
{
	sqlite3 *db;
	int result;
	char *errmsg = NULL;
	char **dbResult;
	int nRow, nColumn;
	int i, j;
	int index;

	result = sqlite3_open("test.db", &db);

	if(result != SQLITE_OK)
	{
		printf("DB open failed!\n");
		return -1;
	}

	result = sqlite3_get_table(db, "select id from tt", &dbResult,
			&nRow, &nColumn, &errmsg);
	if(SQLITE_OK == result)
	{
		printf("select success!\n");
		index = nColumn;
		printf("select %d rows record!\n", nRow);

		for(i = 0; i < nRow; i++)
		{
			printf("The %dth record:\n", i + 1);
			for(j = 0; j < nColumn; j++)
			{
				printf("name:%s?>value:%s\n", dbResult[j], dbResult[index]);
				index++;
			}
			printf("------\n");
		}
	}
	sqlite3_free_table(dbResult);

	sqlite3_close(db);
	return 0;
}


int main(int argc, char* argv[])
{
	GetTable();
	return 0;
}

