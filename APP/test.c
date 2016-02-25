#include "sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
int main( int argc, char **argv )
{
    char *file = "database.sqlite3";
    sqlite3 *db = NULL;
    int rc = 0;
	sqlite3_stmt *stmt = NULL;
	const char* data = NULL;

    sqlite3_initialize( );
    rc = sqlite3_open_v2( file, &db, SQLITE_OPEN_READWRITE | 
         SQLITE_OPEN_CREATE, NULL );
	if ( rc != SQLITE_OK) 
    {
		sqlite3_close( db );
		exit( -1 );
    }

/*	rc = sqlite3_prepare_v2( db, "CREATE TABLE tb1 ( str TEXT )", -1, &stmt, NULL );
	if(rc != SQLITE_OK) exit(-1);

    rc = sqlite3_step( stmt );
	if(rc != SQLITE_DONE) exit(-1);
*/

	rc = sqlite3_prepare_v2( db, "INSERT INTO  tb1 values('jgg')", -1, &stmt, NULL );
	if(rc != SQLITE_OK) exit(-1);

    rc = sqlite3_step( stmt );
	if(rc != SQLITE_DONE) exit(-1);

/*	rc = sqlite3_prepare_v2(db, "DELETE FROM tb1 where str LIKE 'k__'", -1, &stmt, NULL);
	rc = sqlite3_prepare_v2(db, "DELETE FROM tb1 where str not LIKE 'a__'", -1, &stmt, NULL);
	if(rc != SQLITE_OK) exit(-1);

    rc = sqlite3_step( stmt );
    rc = sqlite3_step( stmt );
	if(rc != SQLITE_DONE) exit(-1);

	rc = sqlite3_prepare_v2(db, "UPDATE tb1 set str='lkk' where str='jhh'", -1, &stmt, NULL);
	if(rc != SQLITE_OK) exit(-1);

	rc = sqlite3_step(stmt);
	if(rc != SQLITE_DONE) exit(-1);

*/
	rc = sqlite3_prepare_v2(db, "SELECT str FROM tb1 ORDER BY 1", -1, &stmt, NULL);
	if(rc != SQLITE_OK) exit(-1);
	while(sqlite3_step(stmt) == SQLITE_ROW){
		data = (const char*)sqlite3_column_text(stmt, 0);
		printf("...%s\n", data ? data : "[NULL]");
	}

	sqlite3_finalize(stmt);
    /*  perform database operations  */
  
    sqlite3_close( db );
    sqlite3_shutdown( );
}
