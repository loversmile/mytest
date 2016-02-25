#include <stdio.h>
#include <sqlite3.h>
#include <pthread.h>

#define LOOP_NUM  10

static sqlite3* Ucmdb = NULL;
static sqlite3* Ucmdb2 = NULL;

update(sqlite3* db, const char *sql)
{
    int err;
    char *szErrMsg = 0;
    err = sqlite3_exec(Ucmdb, sql, NULL, NULL, &szErrMsg);
    if(SQLITE_OK != err)
    {
        printf("update failed!\n");
        sqlite3_free(szErrMsg); 
        sqlite3_close(db);
        return -1;
    }
}

update2(sqlite3* db, const char *sql)
{
    int err;
    char *szErrMsg = 0;
    err = sqlite3_exec(Ucmdb2, sql, NULL, NULL, &szErrMsg);
    if(SQLITE_OK != err)
    {
        printf("update failed!\n");
        sqlite3_free(szErrMsg); 
        sqlite3_close(db);
        return -1;
    }
}

void *test1(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update(Ucmdb, "update tt set name='abc' where id=1");
        printf("test 1 ret = %d \n", ret);
    }
}
void *test2(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update(Ucmdb, "insert into AA VALUES (1,'dd','pp') ");
        printf("test 2 ret = %d \n", ret);
    }
}

void *test3(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update(Ucmdb, "update tt set name='abc' where id=3");
        printf("test 3 ret = %d \n", ret);
    }
}
void *test4(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update(Ucmdb, "delete from AA;");
        printf("test 4 ret = %d \n", ret);
    }
}
void *test5(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update(Ucmdb, "update tt set name='abc' where id=3");
        printf("test 5 ret = %d \n", ret);
    }
}
void *test6(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update2(Ucmdb2, "update tt set name='abc' where id=3");
        printf("test 6 ret = %d \n", ret);
    }
}
void *test7(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update2(Ucmdb2, "insert into aa values(23,'3434','asdf')");
        printf("test 7 ret = %d \n", ret);
    }
}
void *test8(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update2(Ucmdb2, "update tt set name='abc' where id=3");
        printf("test 8 ret = %d \n", ret);
    }
}
void *test9(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update2(Ucmdb2, "update tt set name='abc' where id=3");
        printf("test 9 ret = %d \n", ret);
    }
}
void *test0(void *tt)
{
    int ret = 0;
    int i = 0;
    for(i = 0; i < LOOP_NUM; i++){
        ret = update2(Ucmdb2, "update tt set name='abc' where id=3");
        printf("test 0 ret = %d \n", ret);
    }
}

int main()
{
    int ret = 0;
    int open = sqlite3_open("./test.db", &Ucmdb);
    int open2 = sqlite3_open("./test.db", &Ucmdb2);
    printf("____________---\n");
    pthread_t pid1;
    pthread_t pid2;
    pthread_t pid3;
    pthread_t pid4;
    pthread_t pid5;
    pthread_t pid6;
    pthread_t pid7;
    pthread_t pid8;
    pthread_t pid9;
    pthread_t pid0;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&pid1, &attr, test1, NULL);
    pthread_create(&pid2, &attr, test2, NULL);
    pthread_create(&pid3, &attr, test3, NULL);
    pthread_create(&pid4, &attr, test4, NULL);
    pthread_create(&pid5, &attr, test5, NULL);
    pthread_create(&pid6, &attr, test6, NULL);
    pthread_create(&pid7, &attr, test7, NULL);
    pthread_create(&pid8, &attr, test8, NULL);
    pthread_create(&pid9, &attr, test9, NULL);
    pthread_create(&pid0, &attr, test0, NULL);


    sleep(10);

    return 0;
}
