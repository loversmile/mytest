/****************************************************************************
 *
 * FILENAME:        $RCSfile: handlers.c,v $
 *
 * LAST REVISION:   $Revision: 1.0 $
 * LAST MODIFIED:   $Date: 2014/03/28 09:24:42 $
 *
 * DESCRIPTION:     
 *
 * vi: set ts=4:
 *
 * Copyright (c) 2012-2013 by Grandstream Networks, Inc.
 * All rights reserved.
 * 
 * This material is proprietary to Grandstream Networks, Inc. and, 
 * in addition to the above mentioned Copyright, may be 
 * subject to protection under other intellectual property 
 * regimes, including patents, trade secrets, designs and/or 
 * trademarks. 
 *
 * Any use of this material for any purpose, except with an 
 * express license from Grandstream Networks, Inc. is strictly 
 * prohibited.
 *
 ***************************************************************************/
#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <net/if.h>
#include <signal.h>

#include "global.h"
#include "json/json.h"
#include "users.h"
#include "database.h"
#include "mappingParser.h"
#include "file_operator.h"
#include "disk_function.h"
#include "pvalue_monitor.h"


#ifdef MEMWATCH
    #include "memwatch.h"
#endif

typedef struct _ActionList
{
    char *name;
    FuncPtr func;
    ParamList *list;
    struct _ActionList *next;
}ActionList;


static char s_pcap_name[64] = {0};
static TroubleShootTask s_tsTask[TS_TOTAL] = {
    { NULL, NULL, NULL, NULL, -1, PTHREAD_MUTEX_INITIALIZER, NULL, 0 },
    { NULL, NULL, NULL, NULL, -1, PTHREAD_MUTEX_INITIALIZER, NULL, 0 },
    { NULL, NULL, NULL, NULL, -1, PTHREAD_MUTEX_INITIALIZER, NULL, 0 }
};

static char s_err_detail[256] = { 0 };

const char *s_tsname[TS_TOTAL] = {
    "capture",
    "ping",
    "traceroute"
};

static NetInfo info = {
    -1,
    { 0 },
    { 0 },
    { 0 },
    { 0 },
    { 0 }
};

const char *route_path[3] = {
    "/etc/resolv.conf",
    "/etc/resolv.conf.eth0",
    "/etc/resolv.conf.eth1"
};

static int cleanDir(const char *path);
static int get_mac_address(char *mac, int mac_len);
static int isTSTaskRunning(TSName ts_type);
static int packageToJson(struct json_object *json, const char *target, DoList *list);
static int cgiGetConf(Task *task, ActionList *action, const char *target);
static char* getDignosticResponse(const char *key, const char *target, ParamNode *node);
static int getCaptureFile(Task *task);
static int cgiSetupDiagnostic(Task *task);
static int cgiStopDiagnostic(Task *task);
static pid_t getProcessPIDByName(const char *pid_name);
static int killProgress(const char *name);

static pid_t getProcessPIDByName(const char *pid_name)
{
    DIR *dir = NULL;
    struct dirent *next = NULL;
    pid_t pid = -1;
    FILE *status = NULL;
    char filename[64] = { 0 };
    char buffer[64] = { 0 };
    char name[64] = { 0 };

    if (pid_name == NULL)
    {
        return pid;
    }

    dir = opendir("/proc");
    if (dir == NULL)
    {
        return pid;
    }

    while ((next = readdir(dir)) != NULL)
    {
        status = NULL;
        memset(filename, 0, sizeof(filename));
        memset(buffer, 0, sizeof(buffer));
        memset(name, 0, sizeof(name));
        /* If it isn't a number, we don't want it */
        /* Must skip ".." since that is outside /proc */
        if (strcmp(next->d_name, "..") == 0 || isdigit(*next->d_name) == 0)
        {
            continue;
        }

        sprintf(filename, "/proc/%s/status", next->d_name);
        if (!(status = fopen(filename, "r")))
        {
            continue;
        }

        if (fgets(buffer, sizeof(buffer) - 1, status) == NULL)
        {
            fclose(status);
            continue;
        }
        fclose(status);

        /* Buffer should contain a string like "Name:   binary_name" */
        sscanf(buffer, "%*s %s", name);
        if (strcmp(name, pid_name) == 0)
        {
            pid = strtol(next->d_name, NULL, 0);
            cgilog(SYSLOG_INFO, WEBCGI_SYSLOG_FCGI "Get pid: [%s]=>(%d)", pid_name, pid);
            break;
        }
    }
    closedir(dir);

    return pid;
}

/*only for stop single thread progress*/
static int killProgress(const char *name)
{
    int ret = -1;
    if (name == NULL)
    {
        return ret;
    }

    pid_t pid = getProcessPIDByName(name);

    if (pid > 0)
    {
        ret = kill(pid, SIGINT);
    }

    return ret;
}

static int getFileSize(const char *file)
{
    if (file == NULL)
    {
        return -1;
    }

    struct stat temp;
    stat(file, &temp);
    return temp.st_size;
}

static int cleanDir(const char *path)
{
    DIR *dir = NULL;
    struct dirent *ent = NULL;
    char buf[512] = { 0 };
    int ret_rm = 0;

    if (path == NULL || (dir = opendir(path)) == NULL)
    {
        cgilog(SYSLOG_ERR, "Open dir '%s' fail !!!", path);
        return -1;
    }

    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
        {
            snprintf(buf, sizeof(buf), "%s/%s", path, ent->d_name);
            ret_rm = remove(buf);
            cgilog(SYSLOG_DEBUG, "%s deleted result is %d", buf, ret_rm);
        }
    }
    closedir(dir);

    return 0;
}

static int get_mac_address(char *mac, int mac_len)
{
    struct ifreq ifr;
    int fd;
    char buf[32] = { 0 };

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd >= 0)
    {
        //get_network_interface(ifr.ifr_name, sizeof(ifr.ifr_name) - 1);
        strncpy(ifr.ifr_name, "eth0", sizeof(ifr.ifr_name) - 1);
        if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
        {
            snprintf(buf, sizeof(buf), "%02x%02x%02x%02x%02x%02x",
                     (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                     (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                     (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                     (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                     (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                     (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
        }
        close(fd);
    }
    snprintf(mac, mac_len, "%s", buf);

    return 0;
}

static int cgiDignosticThreadRun(int *ts_type)
{
    cgilog(SYSLOG_DEBUG, "---------------------Enter cgiDignosticThreadRun----------------\n");
    char buf[1024] = { 0 };
    int buf_len = 0;

    if (ts_type == NULL)
    {
        return -1;
    }
    int index = *ts_type;
    free(ts_type);
    ts_type = NULL;

    cgilog(SYSLOG_INFO, "----------------Dignostic start!------------------ %d", index);

    if (index < 0 || index >= TS_TOTAL)
    {
        strcpy(s_err_detail, NEW_CGI_TS_INVALID);
        cgilog(SYSLOG_ERR, "ERROR INDEX!!!!");
        cgilog(SYSLOG_INFO, "----------------Dignostic EXIT!------------------ %d", index);

        return CGICODE_ERROR;
    }

    pthread_mutex_lock(&(s_tsTask[index].mutex));
    if (s_tsTask[index].output != NULL)
    {
        free(s_tsTask[index].output);
    }

    s_tsTask[index].output = (char *)malloc(WEBCGI_AMI_BUFF_SIZE);
    memset(s_tsTask[index].output, 0, WEBCGI_AMI_BUFF_SIZE);
    s_tsTask[index].outputSize = WEBCGI_AMI_BUFF_SIZE;

    pthread_mutex_unlock(&(s_tsTask[index].mutex));

    s_tsTask[index].cmdfp = popen(s_tsTask[index].command, "r");

    if (s_tsTask[index].cmdfp != NULL)
    {
        cgilog(SYSLOG_DEBUG, "cgiDignostic: s_tsTask.cmdfp != NULL");

        while (s_tsTask[index].cmdfp != NULL
               && (!feof(s_tsTask[index].cmdfp))
               && (fgets(buf, sizeof(buf) - 1, s_tsTask[index].cmdfp) != NULL))
        {
            cgilog(SYSLOG_DEBUG, "webcgiDignosticRead: %s", buf);
            buf_len = strlen(buf);
            if (buf_len < s_tsTask[index].outputSize)
            {
                pthread_mutex_lock(&(s_tsTask[index].mutex));
                strncat(s_tsTask[index].output, buf, buf_len);
                s_tsTask[index].outputSize -= buf_len;
                pthread_mutex_unlock(&(s_tsTask[index].mutex));
            }
            else
            {
                cgilog(SYSLOG_DEBUG, "webcgiDignosticRead: Out of output buffer!");
            }
        }
        cgilog(SYSLOG_DEBUG, "cgiDignosticThread: Thread stopped!");

        if (index == TS_CAPTURE)
        {
            /*EXIT caused by the error of filter*/
			cgilog(SYSLOG_DEBUG, "in s_err_detail = NEW_CGI_TS_INVALID_FILTE_______________\n");
            strcpy(s_err_detail, NEW_CGI_TS_INVALID_FILTER);
        }

        pthread_mutex_lock(&(s_tsTask[index].mutex));
        if (s_tsTask[index].cmdfp != NULL)
        {
            pclose(s_tsTask[index].cmdfp);
            s_tsTask[index].cmdfp = NULL;
        }
        pthread_mutex_unlock(&(s_tsTask[index].mutex));
    }
    else
    {
        cgilog(SYSLOG_ERR, "DignosticRead: cmd isn't exist!");
    }

    cgilog(SYSLOG_ERR, "DignosticThread: exist!");
    cgilog(SYSLOG_INFO, "----------------Dignostic end!------------------ %d", index);

    return CGICODE_SUCCESS;
}


static void stopTSTask(TSName ts_type)
{
    if (ts_type >= TS_TOTAL || ts_type < 0)
    {
        return;
    }

    cgilog(SYSLOG_DEBUG, "Get lock!");
    pthread_mutex_lock(&(s_tsTask[ts_type].mutex));
    cgilog(SYSLOG_DEBUG, "Got the lock!");

    if (ts_type == TS_CAPTURE)
    {
        cgilog(SYSLOG_DEBUG, "Kill tcpdump!");
        char buf[256] = { 0 };

        killProgress("tcpdump");

        /*move to /data*/
#ifdef BUILD_ON_ARM
        chdir("/data");
#else
        chdir("/tmp");
#endif
        /*Change name from xxx.pcap0 to xxx0.pcap*/
        char org[256] = { 0 };
        char chg[256] = { 0 };
        for (int i = 0; i < 5; i++)
        {
            snprintf(org, sizeof(org), "./troubleshooting/capture.pcap%d", i);
            snprintf(chg, sizeof(chg), "./troubleshooting/capture%d.pcap", i);
            rename(org, chg);
        }

        snprintf(buf, sizeof(buf), "%s %s %s", "tar -czvf", s_pcap_name, "troubleshooting");
        cgilog(SYSLOG_DEBUG, "tar cmd is %s", buf);
        system(buf);
        snprintf(buf, sizeof(buf), "%s %s %s", "mv", s_pcap_name, "troubleshooting");
        system(buf);
    }
    else if (ts_type == TS_PING)
    {
        cgilog(SYSLOG_DEBUG, "Kill ping!");
        killProgress("ping");
    }
    else if (ts_type == TS_TRACEROUTE)
    {
        cgilog(SYSLOG_DEBUG, "Kill traceroute!");
        killProgress("traceroute");
    }
    pthread_mutex_unlock(&(s_tsTask[ts_type].mutex));

    /*Wait for the final result*/
    pthread_mutex_lock(&(s_tsTask[ts_type].mutex));

    if (s_tsTask[ts_type].thread != -1)
    {
        pthread_cancel(s_tsTask[ts_type].thread);
        s_tsTask[ts_type].thread = -1;
    }

    if (s_tsTask[ts_type].cmdfp != NULL)
    {
        /*the pipe is closed when the process is killed*/
        //pclose(s_tsTask[ts_type].cmdfp);
        s_tsTask[ts_type].cmdfp = NULL;
    }

    pthread_mutex_unlock(&(s_tsTask[ts_type].mutex));
	s_err_detail[0] = '\0';

    cgilog(SYSLOG_DEBUG, "Exit!!!!");
}

void cleanTSTask(TSName ts_type)
{
    if (ts_type >= TS_TOTAL || ts_type < 0)
    {
        return;
    }

    cgilog(SYSLOG_DEBUG, "cleanTSTask: COMMAND IS %s", s_tsTask[ts_type].command == NULL ? "" : s_tsTask[ts_type].command);

    pthread_mutex_lock(&(s_tsTask[ts_type].mutex));
    if (s_tsTask[ts_type].thread != -1)
    {
        pthread_cancel(s_tsTask[ts_type].thread);
        s_tsTask[ts_type].thread = -1;
    }

    if (s_tsTask[ts_type].output != NULL)
    {
        free(s_tsTask[ts_type].output);
        s_tsTask[ts_type].output = NULL;
        s_tsTask[ts_type].outputSize = 0;
    }

    if (s_tsTask[ts_type].interface != NULL)
    {
        free(s_tsTask[ts_type].interface);
        s_tsTask[ts_type].interface = NULL;
    }

    if (s_tsTask[ts_type].param != NULL)
    {
        free(s_tsTask[ts_type].param);
        s_tsTask[ts_type].param = NULL;
    }

    if (s_tsTask[ts_type].command != NULL)
    {
        free(s_tsTask[ts_type].command);
        s_tsTask[ts_type].command = NULL;
    }

    if (s_tsTask[ts_type].cmdfp != NULL)
    {
        pclose(s_tsTask[ts_type].cmdfp);
        s_tsTask[ts_type].cmdfp = NULL;
    }

    pthread_mutex_unlock(&(s_tsTask[ts_type].mutex));

    pthread_mutex_destroy(&(s_tsTask[ts_type].mutex));
}

static int createTSTask(TSName ts_type)
{
    int ret = 0;
    int result = CGICODE_SUCCESS;
    pthread_attr_t attr;

    if (ts_type >= TS_TOTAL || ts_type < 0)
    {
        return CGICODE_ERROR;
    }

    pthread_mutex_init(&(s_tsTask[ts_type].mutex), NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, WEBCGI_STACK_SIZE);

    /*Create webcgi main thread(fast-cgi)*/
    cgilog(SYSLOG_DEBUG, "Starting %s %d thread!!!!!", s_tsname[ts_type], ts_type);
    int *index = (int *)malloc(sizeof(int)); /*Need to free in the thread function*/
    *index = ts_type;
    ret = pthread_create(&(s_tsTask[ts_type].thread), &attr, (void *)cgiDignosticThreadRun, (void *)index);

	if (ret != 0)
    {
        cgilog(SYSLOG_ERR, WEBCGI_SYSLOG_ERR "Create cgi dignostic thread error");
        result = CGICODE_ERROR;
    }

    return result;
}


static int isTSTaskRunning(TSName ts_type)
{
    int err = CGICODE_SUCCESS;
    if (s_tsTask[ts_type].cmdfp != NULL)
    {
        cgilog(SYSLOG_ERR, "A task is running, reject this request!");
        err = CGICODE_TSHOOT_RUNNING;
        return err; /*have to quit directly*/
    }

    if (s_tsTask[ts_type].cmdfp == NULL)
    {
        /*The thread is stopped, Clean it!*/
        cgilog(SYSLOG_DEBUG, "Init the used TSTask!");

        cleanTSTask(ts_type);
    }
    return err;
}

static int packageToJson(struct json_object *json, const char *target, DoList *list)
{
    cgilog(SYSLOG_DEBUG, "Enter: packageToJson !! \n");
    struct json_object *values = json;
    struct json_object *task_json = NULL;

    int have_target = (int)(target != NULL && target[0] != '\0');

    if (have_target > 0)
    {
        /*Have target !*/
        values = json_object_new_object();
    }

    DoList *node = list;
    while (node != NULL)
    {
        cgilog(SYSLOG_DEBUG, "while Loop ping? traceroute ?\n");
        if (node->value != NULL && node->key != NULL
            && node->node != NULL && node->node->pathList != NULL)
        {
            if (strcasestr(node->key, "ping") != NULL
                     || strcasestr(node->key, "traceroute") != NULL)
            {
                cgilog(SYSLOG_DEBUG, "process Trouble Shooting value list:");
                cgilog(SYSLOG_DEBUG, "value: %s", node->value);
                char data[2048] = { 0 };
                char name[64] = { 0 };
                getNthData(node->value, '`', 0, name, sizeof(name));
                cgilog(SYSLOG_DEBUG, "the 0th value is: %s", name);
                task_json = json_object_new_object();

                getNthData(node->value, '`', 1, data, sizeof(data));
                cgilog(SYSLOG_DEBUG, "the 1th value is: %s", data);
                json_object_object_add(task_json, "finish", json_object_new_boolean(atoi(data)));

                getNthData(node->value, '`', 2, data, sizeof(data));
                cgilog(SYSLOG_DEBUG, "the 2th value is: %s", data);
                json_object_object_add(task_json, "content", json_object_new_string(data));
                json_object_object_add(values, name, task_json);
            }
            else if (strcasestr(node->key, "shooting-state") != NULL)
            {
                cgilog(SYSLOG_DEBUG, "process Trouble Shooting state list:");
                int ts_type = 0;

                while (ts_type < TS_TOTAL)
                {
                    if (s_tsTask[ts_type].command == NULL || s_tsTask[ts_type].cmdfp == NULL)
                    {
                        cgilog(SYSLOG_DEBUG, "the %s task isn't running", s_tsname[ts_type]);
                        ts_type++;
                        continue;
                    }

                    task_json = json_object_new_object();
                    json_object_object_add(task_json, "param",
                                           json_object_new_string(s_tsTask[ts_type].param == NULL ? "" : s_tsTask[ts_type].param));
                    json_object_object_add(task_json, "interface",
                                           json_object_new_string(s_tsTask[ts_type].interface == NULL ? "" : s_tsTask[ts_type].interface));
                    json_object_object_add(values, s_tsname[ts_type], task_json);

                    ts_type++;
                }

            }
			else if (strcasestr(node->key, "net-mode") != NULL)
			{
				cgilog(SYSLOG_DEBUG, "process net-mode:");
				char mode[8] = { 0 };
				char key[8] = { 0 };
				char value[64] = { 0 };
				char tmp1[1024] = { 0 };
				char tmp2[1024] = { 0 };
				int i, j;

				for (i = 0; i < 4; i++)
				{
					getNthData(node->value, ';', i, tmp1, sizeof(tmp1));

					if (i == 0 && tmp1[0] == '\0' && node->value[0] != '\0')
					{
						strncpy(tmp1, node->value, sizeof(tmp1) - 1);
					}

					if (tmp1[0] != '\0')
					{
						getNthData(tmp1, '|', 0, mode, sizeof(mode));
						getNthData(tmp1, '|', 1, tmp2, sizeof(tmp2));
						if (mode[0] != '\0' && tmp2[0] != '\0')
						{
							struct json_object *network_obj1 = json_object_new_object();
							json_object_object_add(values, mode, network_obj1);

							for (j = 0; j < 5; j++)
							{
								getNthData(tmp2, '#', j, tmp1, sizeof(tmp1));

								if (j == 0 && tmp1[0] == '\0' && tmp2[0] != '\0')
								{
									strncpy(tmp1, tmp2, sizeof(tmp1) - 1);
								}

								if (tmp1[0] != '\0')
								{
									getNthData(tmp1, ':', 0, key, sizeof(key));
									getNthData(tmp1, ':', 1, value, sizeof(value));

									cgilog(SYSLOG_ERR, "respone value %s", value);
									if (key[0] != '\0')
									{
										json_object_object_add(network_obj1, key, json_object_new_string(value));
									}
								}
							}
						}
					}
				}
			}
			else
            {
                json_object_object_add(values, node->key, json_object_new_string(node->value));
            }
        }

        node = node->next;
    }

    if (have_target > 0)
    {
        json_object_object_add(json, target, values);
    }
    cgilog(SYSLOG_DEBUG, "Exit: packageToJson !! \n");
    return 0;
}


static int cgiGetConf(Task *task, ActionList *action, const char *target)
{
    cgilog(SYSLOG_DEBUG, "Enter : cgiGetConf");
    int err = CGICODE_ERROR;
    char query[2048] = { 0 };
    DoList *node= task->doList;
    DoList *next = NULL;
    PathList *path = NULL;
    struct json_object *val_json = json_object_new_object();
	json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);

    while (node != NULL && node->node != NULL)
    {
        cgilog(SYSLOG_DEBUG, "while node !!!!!!!!!! %s", node->key);

        path = node->node->pathList;
        if (path != NULL)
        {
            cgilog(SYSLOG_DEBUG, "path !!!!!!!!!! %s %d", path->key, path->type);
            memset(query, 0, sizeof(query));
            switch (path->type)
            {
                case PATHTYPE_CMD_NORMAL:
                    query[0] = '\0';
					if (strcasecmp(node->key, "shooting-state") == 0)
					{
					    node->value = strDup("");
					}
                    else if (strcasecmp(node->key, "ping") == 0 || strcasecmp(node->key, "traceroute") == 0)
                    {
                        node->value = getDignosticResponse(node->key, target, node->node);
                        if (node->value == NULL)
                        {
                            err = CGICODE_TSHOOT_INVALID;
                            goto ret_error;
                        }
                    }
                    else if (strcasecmp(node->key, "capture") == 0)
                    {
                        err = getCaptureFile(task);
                        /*Specail ending!!*/
                        goto ret_error;
                    }
                    else
                    {
                        /*802.1x networkupdat timezoneupdate*/
                        /*Specail ending!!*/
                        err = CGICODE_UNSUPPORT;
                        goto ret_error;
                    }
                    break;
                default:
                    err = CGICODE_UNSUPPORT;
                    goto ret_error;
            }

        }

        node = node->next;
    }
    /*package result as json format*/
    packageToJson(val_json, target, task->doList);

    json_object_object_add(response, "body", val_json);

    err = CGICODE_SUCCESS;

ret_error:
    cgilog(SYSLOG_DEBUG, "Exit : cgiGetConf");
    return err;
}


static char* getDignosticResponse(const char *key, const char *target, ParamNode *node)
{
    cgilog(SYSLOG_DEBUG, "Enter : getDignosticResponse");

    char buf[2048] = { 0 };
    char *ret = NULL;
    int err = CGICODE_SUCCESS;
    int ts_type = -1;

    if (strcasecmp(key, "ping") == 0)
    {
        ts_type = TS_PING;
    }
    else if (strcasecmp(key, "traceroute") == 0)
    {
        ts_type = TS_TRACEROUTE;
    }

    if (s_tsTask[ts_type].command == NULL)
    {
        err = CGICODE_TSHOOT_INVALID;
        strcpy(s_err_detail, "No task, is running, reject this request!");
        goto ts_error;
    }

    pthread_mutex_lock(&(s_tsTask[ts_type].mutex));
    cgilog(SYSLOG_DEBUG, "Task output is %s", s_tsTask[ts_type].output);

    snprintf(buf, sizeof(buf), "%s`%s`%s", s_tsname[ts_type], s_tsTask[ts_type].cmdfp == NULL ? "1" : "0", s_tsTask[ts_type].output);

    /*clean the output buffer*/
    memset(s_tsTask[ts_type].output, 0, WEBCGI_AMI_BUFF_SIZE);
    s_tsTask[ts_type].outputSize = WEBCGI_AMI_BUFF_SIZE;

    pthread_mutex_unlock(&(s_tsTask[ts_type].mutex));

    ret = strDup(buf);
    if (ret == NULL)
    {
        strcpy(s_err_detail, "No enough Memory!");
        err = CGICODE_ERROR;
    }

ts_error:

    if (s_tsTask[ts_type].cmdfp == NULL)
    {
        /*The thread is stopped, Clean it!*/
        cleanTSTask(ts_type);
    }

    cgilog(SYSLOG_DEBUG, "Exit : getDignosticResponse");

    return ret;
}



static int getCaptureFile(Task *task)
{
    int err = 0;
    FILE *stream = NULL;

    if (task == NULL)
    {
        err = CGICODE_ERROR;
        return err;
    }

    if (s_tsTask[TS_CAPTURE].cmdfp != NULL)
    {
        cgilog(SYSLOG_ERR, "A Task is still running!");
        err = CGICODE_TSHOOT_RUNNING;
        return err;
    }

    char capfile[128] = { 0 };

    snprintf(capfile, sizeof(capfile), "%s%s", PCAP_PATH, s_pcap_name);

    if ((stream = fopen(capfile, "r")) == NULL)
    {
        cgilog(SYSLOG_ERR, "The pcap file isn't exsit!");
        err = CGICODE_FILE_ISNT_EXIST;
        task->buffer[0] = '\0';
        strcat(task->buffer, HTTP_STATUS_404);
        strcat(task->buffer, HTTP_CONTENT_PLAIN);
        strcat(task->buffer, HTTP_SERVER);
        strcat(task->buffer, "\r\n");
        task->headerSize = strlen(task->buffer);

        FCGX_PutStr(task->buffer, strlen(task->buffer), task->request.out);
        return err;
    }
    else
    {
        cgilog(SYSLOG_DEBUG, "The file was opened!");
        /*It is a new response, add the HTTP headers*/
        task->buffer[0] = '\0';
        strcat(task->buffer, HTTP_STATUS_200);
        strcat(task->buffer, HTTP_CONTENT_TAR);
        strcat(task->buffer, HTTP_SERVER);

        long filesize = getFileSize(capfile);
        char sizef[64] = { 0 };
        snprintf(sizef, sizeof(sizef), "%ld", filesize);
        cgilog(SYSLOG_DEBUG, "The file size is %s", sizef);
        strcat(task->buffer, "Content-Length: ");
        strcat(task->buffer, sizef);
        strcat(task->buffer, "\r\n");
        strcat(task->buffer, "Content-Disposition: attachment; filename=");
        strcat(task->buffer, s_pcap_name);
        strcat(task->buffer, "\r\n");
        strcat(task->buffer, "\r\n");

        task->headerSize = strlen(task->buffer);

        FCGX_PutStr(task->buffer, strlen(task->buffer), task->request.out);
        err = CGICODE_SUCCESS;
    }

    task->buffer[0] = '\0';
    int lengsize = 0;

    while ((lengsize = fread(task->buffer, 1, 1024, stream)) > 0)
    {
        cgilog(SYSLOG_DEBUG, "lengsize = %d", lengsize);
        FCGX_PutStr(task->buffer, lengsize, task->request.out);
    }

    if (stream != NULL)
    {
        fclose( stream );
    }
    /*Clean files*/
    //rmdir(PCAP_PATH);

    return err;
}

static int cgiSetupDiagnostic(Task *task)
{
    cgilog(SYSLOG_DEBUG, "Enter : cgiSetupDiagnostic");

	json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);
    int err = CGICODE_ERROR;
    int i = 0;
    int ts_type = -1;
    char *filter = NULL;
    char buf[256] = { 0 };
    char tasklist[80] = { 0 };
    char cmd[512] = { 0 };
    int first[TS_TOTAL] = { 0, 0, 0 }; /*MARK as Meet the main key*/

    DoList *node = task->doList;
    PathList *path = NULL;

    while (node != NULL && node->node != NULL && node->key != NULL)
    {
        cgilog(SYSLOG_DEBUG, "while node !!!!!!!!!! %s", node->key);
        path = node->node->pathList;

        if (strcasecmp(node->key, "ping") == 0)
        {
            ts_type = TS_PING;
            if (first[ts_type] == 0)
            {
                /*Judge and clean the task*/
                err = isTSTaskRunning(ts_type);
                first[ts_type] = 1;

                if (err != CGICODE_SUCCESS)
                {
                    goto ts_error;
                }
            }

            if (s_tsTask[ts_type].command != NULL || node->value == NULL)
            {
                err = CGICODE_TSHOOT_INVALID;
                goto ts_error;
            }

            if (path->key == NULL)
            {
                err = CGICODE_UNSUPPORT;
                goto ts_error;
            }

            if (tasklist[0] != '\0')
            {
                strncat(tasklist, "/", sizeof(tasklist));
            }
            strncat(tasklist, node->key, sizeof(tasklist));

            /*Create new TSTask!*/
            snprintf(cmd, sizeof(cmd), "%s%s %s", path->key,
                     path->table == NULL ? "" : path->table,
                     node->value);

            cgilog(SYSLOG_DEBUG, "cgiSetupDiagnostic: COMMAND IS %s", cmd);
            s_tsTask[ts_type].command = strDup(cmd);
            s_tsTask[ts_type].param = strDup(node->value);
        }
        else if (strcasecmp(node->key, "traceroute") == 0)
        {
            ts_type = TS_TRACEROUTE;

            if (first[ts_type] == 0)
            {
                /*Judge and clean the task*/
                err = isTSTaskRunning(ts_type);
                first[ts_type] = 1;

                if (err != CGICODE_SUCCESS)
                {
                    goto ts_error;
                }
            }

            if (s_tsTask[ts_type].command != NULL || node->value == NULL)
            {
                err = CGICODE_TSHOOT_INVALID;
                goto ts_error;
            }

            if (path->key == NULL)
            {
                err = CGICODE_UNSUPPORT;
                goto ts_error;
            }

            if (tasklist[0] != '\0')
            {
                strncat(tasklist, "/", sizeof(tasklist));
            }
            strncat(tasklist, node->key, sizeof(tasklist));


            /*Create new TSTask!*/
            snprintf(cmd, sizeof(cmd), "%s%s %s", path->key,
                     path->table == NULL ? "" : path->table,
                     node->value);

            s_tsTask[ts_type].command = strDup(cmd);
            s_tsTask[ts_type].param = strDup(node->value);
            cgilog(SYSLOG_DEBUG, "cgiSetupDiagnostic: COMMAND IS %s", cmd);
        }
        else if (strcasecmp(node->key, "capture") == 0)
        {
            ts_type = TS_CAPTURE;

            if (first[ts_type] == 0)
            {
                /*Judge and clean the task*/
                err = isTSTaskRunning(ts_type);
                first[ts_type] = 1;

                if (err != CGICODE_SUCCESS)
                {
                    goto ts_error;
                }
            }

            if (s_tsTask[ts_type].command != NULL || node->value == NULL)
            {
                err = CGICODE_TSHOOT_INVALID;
                goto ts_error;
            }

            if (path->key == NULL)
            {
                err = CGICODE_UNSUPPORT;
                goto ts_error;
            }

            /*Init path*/
            mkdir(PCAP_PATH, S_IRWXU);

            /*Clean files, won't rm the dir*/
            cleanDir(PCAP_PATH);

            /*Create new TSTask!*/
            char interface[16] = { 0 };

            getDeviceInfo(&info);

            if (strcasecmp(node->value, "WAN") == 0)
            {
                snprintf(interface, sizeof(interface), " -i %s", info.wan);
            }
            else if (strcasecmp(node->value, "LAN") == 0)
            {
                snprintf(interface, sizeof(interface), " -i %s", info.lan);
            }
            else if (strcasecmp(node->value, "LAN 1") == 0)
            {
                snprintf(interface, sizeof(interface), " -i %s", info.lan1);
            }
            else if (strcasecmp(node->value, "LAN 2") == 0)
            {
                snprintf(interface, sizeof(interface), " -i %s", info.lan2);
            }
            else
            {
                strncpy(interface, " -i lo", sizeof(interface));
            }

            if (tasklist[0] != '\0')
            {
                strncat(tasklist, "/", sizeof(tasklist));
            }
            strncat(tasklist, node->key, sizeof(tasklist));

            /*Create new TSTask!*/
            snprintf(buf, sizeof(buf), "%s%s%s", path->key, interface,
                     path->table == NULL ? "" : path->table);
            s_tsTask[ts_type].interface = strDup(node->value);
        }
        else if (strcasecmp(node->key, "capture-filter") == 0)
        {
            filter = node->value;
        }

        node = node->next;
    }

    /*Process Capture task*/
    if (first[TS_CAPTURE] != 0)
    {
        if (filter == NULL)
        {
            snprintf(cmd, sizeof(cmd), "%s", buf);
        }
        else
        {
            snprintf(cmd, sizeof(cmd), "%s '%s'", buf, filter);
            s_tsTask[TS_CAPTURE].param = strDup(filter);
        }

        s_tsTask[TS_CAPTURE].command = strDup(cmd);
        cgilog(SYSLOG_DEBUG, "cgiSetupDiagnostic: COMMAND IS %s", cmd);
    }

    /*Create new TSTasks!*/
    i = TS_CAPTURE;
    while (i < TS_TOTAL)
    {
        if (first[i] == 0)
        {
            i++;
            continue;
        }

        if (s_tsTask[i].command != NULL)
        {
            err = createTSTask(i);

            if (i == TS_CAPTURE)
            {
                /*WAIT FOR FILTER*/
                sleep(1);
            }

            if (err != CGICODE_SUCCESS)
            {
                goto ts_error;
            }

            if (i == TS_CAPTURE && strcasecmp(s_err_detail, NEW_CGI_TS_INVALID_FILTER) == 0)
            {
                err = CGICODE_TSHOOT_INVALID_FILTER;
                goto ts_error;
            }
        }
        else
        {
            err = CGICODE_UNSUPPORT;
            goto ts_error;
        }
        i++;
    }
	json_object_object_add(response, "body", json_object_new_string("Dignostic run"));

    err = CGICODE_SUCCESS;

ts_error:
	cgilog(SYSLOG_DEBUG, "err = %d !!!! \n", err);
    if (err != CGICODE_SUCCESS)
    {
        cgilog(SYSLOG_DEBUG, "Clean the TSTask, when fail!");
		json_object_object_add(response, "body", json_object_new_string("Invalid TroubleShooting Task"));
		if (s_tsTask[ts_type].command != NULL)
		{
		    stopTSTask(ts_type);
		}
		cleanTSTask(ts_type);
    }
    cgilog(SYSLOG_DEBUG, "Exit : webcgiSetupDianostic");

    return err;
}

static int cgiStopDiagnostic(Task *task)
{
    cgilog(SYSLOG_DEBUG, "Enter : cgiStopDiagnostic");

    struct json_object *body_json = json_object_new_object();
    struct json_object *task_json = NULL;
	json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);
    int err = CGICODE_ERROR;
    int ts_type = -1;
    int first[TS_TOTAL] = { 0, 0, 0 }; /*MARK as Meet the main key*/
    DoList *node = task->doList;
    PathList *path = NULL;

    while (node != NULL && node->node != NULL && node->key != NULL)
    {
        cgilog(SYSLOG_DEBUG, "while node !!!!!!!!!! %s", node->key);
        path = node->node->pathList;

        if (strcasecmp(node->key, "ping") == 0)
        {
            ts_type = TS_PING;
            first[ts_type]++;
        }
        else if (strcasecmp(node->key, "traceroute") == 0)
        {
            ts_type = TS_TRACEROUTE;
            first[ts_type]++;
        }
        else if (strcasecmp(node->key, "capture") == 0)
        {
            ts_type = TS_CAPTURE;
            first[ts_type]++;
        }

        if (first[ts_type] == 1)
        {
            cgilog(SYSLOG_DEBUG, "========================================= %d", ts_type);

            if (s_tsTask[ts_type].command == NULL)
            {
                cgilog(SYSLOG_DEBUG, "No task is running, ignore this request!");
                cleanTSTask(ts_type);
                cgilog(SYSLOG_DEBUG, "Clean the stopped task finish!");
            }
            else
            {
                cgilog(SYSLOG_DEBUG, "cgiStopDiagnostic: going to stop !!!!");

                /*Stop TSTask!*/
                stopTSTask(ts_type);
                cgilog(SYSLOG_DEBUG, "cgiStopDiagnostic: stopped !!!!");
            }

            task_json = json_object_new_object();
            json_object_object_add(task_json, "finish", json_object_new_boolean(1));
            json_object_object_add(task_json, "content", json_object_new_string(s_tsTask[ts_type].output == NULL ? "" : s_tsTask[ts_type].output));
            json_object_object_add(body_json, s_tsname[ts_type], task_json);

        }
        node = node->next;
    }
	json_object_object_add(response, "body", body_json);

    err = CGICODE_SUCCESS;

    cgilog(SYSLOG_DEBUG, "Exit : webcgiStopDianostic");
    return err;
}

int startTroubleShooting( Task *task )
{
    cgilog(SYSLOG_DEBUG, "start the troubleshooting !");
    char mac[64] = { 0 };
    /*Init pcap file name*/
    get_mac_address(mac, sizeof(mac));
    snprintf(s_pcap_name, sizeof(s_pcap_name), "capture-%s.tgz", mac);
	return cgiSetupDiagnostic(task);
}

int getTroubleShooting(Task *task)
{
	cgilog(SYSLOG_DEBUG, "get the troubleshooting ! ");
	ActionList *action = NULL;
	const char *target = NULL;
	return cgiGetConf(task, action, target);
}

int stopTroubleShooting(Task *task)
{
    cgilog(SYSLOG_DEBUG, "stop the troubleshooting !");
	return cgiStopDiagnostic(task);
}

static char* getDefaultGateway()
{
    FILE *fp = fopen("/proc/net/route", "r");
    char line[128] = { 0 };
    char dev[64] = { 0 };
    unsigned int dest;
    unsigned int gw;
    char buf[16] = { 0 };

    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp) != NULL)
        {
            if (sscanf(line, "%s %x %x", dev, &dest, &gw) > 0)
            {
                if (dest == 0x0)
                {
                    gw = ntohl(gw);
                    snprintf(buf, sizeof(buf), "%u.%u.%u.%u", (gw >> 24) & 0xff,
                             (gw >> 16) & 0xff, (gw >> 8) & 0xff, gw & 0xff);
                    break;
                }
            }
        }

        fclose(fp);
    }

    return strDup(buf);
}

static char* getDnsServer(int path_type)
{
    FILE *file = NULL;
    char buf[1024] = { 0 };
    char str[1024] = { 0 };
    char *ptr = NULL;
    char *end = NULL;
    int flag = 0;
    int i;
    char *p = NULL;

    file = fopen(route_path[path_type], "r+");
    if (file != NULL)
    {
        for (i = 0; i < 6 && !feof(file) && (fgets(buf, sizeof(buf) - 1, file) != NULL); i++)
        {
            flag = 0;
            ptr = strstr(buf, "nameserver");
            if (ptr != NULL && ptr != '\0')
            {
                ptr = ptr + strlen("nameserver");

                while (ptr != NULL)
                {
                    if (ptr != NULL && *ptr != ' ')
                    {
                        flag = 1;
                        break;
                    }

                    ptr++;
                }
            }

            if (flag == 1)
            {
                end = ptr;
                while (*end != '\0')
                {
                    if (*end == ' ' || *end == '\n'
                        || *end == '\r' || *end == '\t')
                    {
                        *end = '\0';
                        break;
                    }
                    end++;
                }

                if (str[0] == '\0')
                {
                    snprintf(str, sizeof(str), "%s", ptr);
                }
                else if ((p = strstr(str, ptr)) != NULL
                         && (p == str || *(p - 1) == ',')
                         && (*(p + strlen(ptr)) == '\0' || *(p + strlen(ptr)) == ','))
                {
                    cgilog(SYSLOG_ERR, "this dns server is already exist !");
                }
                else
                {
                    snprintf(str + strlen(str), sizeof(str) - strlen(str), ",%s", ptr);
                }
            }

            memset(buf, 0, sizeof(buf));
        }

        fclose(file);
        file = NULL;
    }
    else
    {
        cgilog(SYSLOG_ERR, "FileRead: filepath isn't exist!");
    }

    return strDup(str);
}

static char* getNetConfig(char *name, int flag, int path_type)
{
    struct ifreq ifr;
    int fd;
    char buf[256] = { 0 };
    char res[1024] = { 0 };
    char *p = NULL;

    if (name == NULL || name[0] == '\0')
    {
        return NULL;
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd >= 0)
    {
        u_int32_t ip;

        strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name) - 1);

        /* get IP */
        if (ioctl(fd, SIOCGIFADDR, &ifr) == 0)
        {
            ip = ntohl(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr);
            snprintf(buf, sizeof(buf), "%u.%u.%u.%u", (ip >> 24) & 0xff,
                     (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
        }

        cgilog(SYSLOG_ERR, "get ip :%s", buf);
        snprintf(res, sizeof(res), "ip:%s", buf);

        /* get mask*/
        memset(buf, 0, sizeof(buf));
        if (ioctl(fd, SIOCGIFNETMASK, &ifr) == 0)
        {
            ip = ntohl(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr);
            snprintf(buf, sizeof(buf), "%u.%u.%u.%u", (ip >> 24) & 0xff,
                     (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff);
        }

        cgilog(SYSLOG_ERR, "get mask :%s", buf);
        snprintf(res + strlen(res), sizeof(res) - strlen(res), "#mask:%s", buf);


        /* get MAC */
        memset(buf, 0, sizeof(buf));
        if (strstr(name, "ppp") != NULL)
        {
            get_mac_address(buf, sizeof(buf));
        }
        else
        {
            /* get gateway */
            if (flag == 0)
            {
                p = getDefaultGateway();
                cgilog(SYSLOG_ERR, "get gateway :%s", p);
                snprintf(res + strlen(res), sizeof(res) - strlen(res), "#gateway:%s", p ? p : "");

                if (p != NULL)
                {
                    free(p);
                    p = NULL;
                }
            }

            if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0)
            {
                snprintf(buf, sizeof(buf), "%02x%02x%02x%02x%02x%02x",
                         (unsigned char)ifr.ifr_hwaddr.sa_data[0],
                         (unsigned char)ifr.ifr_hwaddr.sa_data[1],
                         (unsigned char)ifr.ifr_hwaddr.sa_data[2],
                         (unsigned char)ifr.ifr_hwaddr.sa_data[3],
                         (unsigned char)ifr.ifr_hwaddr.sa_data[4],
                         (unsigned char)ifr.ifr_hwaddr.sa_data[5]);
            }
        }

        cgilog(SYSLOG_ERR, "get mac :%s", buf);
        snprintf(res + strlen(res), sizeof(res) - strlen(res), "#mac:%s", buf);

        close(fd);
    }


    /* get dns */
    if (flag != 1)
    {
        p = getDnsServer(path_type);
        cgilog(SYSLOG_ERR, "get dns :%s", p);
        snprintf(res + strlen(res), sizeof(res) - strlen(res), "#dns:%s", p ? p : "");
        if (p != NULL)
        {
            free(p);
            p = NULL;
        }
    }
    return strDup(res);
}

char* getNetworkInfo(NetInfo *info)
{
    char *p = NULL;
    char buf[1024] = { 0 };

    /* flag = 1 when set the mode as route, the lan as server ,remove gateway and dns*/
    /* flag = 2 when set the mode as dual, only get the default interface gateway*/
    int flag = 0;

    if (info != NULL)
    {
        if (info->lan[0] != '\0')
        {
            flag = info->wan[0] != '\0' ? 1 : 0;
            cgilog(SYSLOG_ERR, "the network server : %d", flag);
            p = getNetConfig(info->lan, flag, 0);

            if (p != NULL)
            {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "lan|%s", p);
                free(p);
                p = NULL;
            }
        }

        if (info->wan[0] != '\0')
        {
            p = getNetConfig(info->wan, 0, 0);
            if (p != NULL)
            {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ";wan|%s", p);
                free(p);
                p = NULL;
            }
        }

        if (info->lan1[0] != '\0')
        {
            flag = (info->default_if[3] == '2' ? 2 : 0);
            //p = getNetConfig(info->lan1, flag, 1);
            p = getNetConfig(info->lan1, flag, 0);
            if (p != NULL)
            {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "lan1|%s", p);
                free(p);
                p = NULL;
            }
        }

        if (info->lan2[0] != '\0')
        {
            flag = (info->default_if[3] == '2' ? 0 : 2);
            //p = getNetConfig(info->lan2, flag, 2);
            p = getNetConfig(info->lan2, flag, 0);
            if (p != NULL)
            {
                snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ";lan2|%s", p);
                free(p);
                p = NULL;
            }
        }
    }

    cgilog(SYSLOG_ERR, "the network is : %s", buf);
    return strDup(buf);
}

int getNetworkInformation(Task *task)
{   
	cgilog(SYSLOG_DEBUG, "Enter listNetworkInfo\n");
    int ret = CGICODE_SUCCESS;
    DoList *node = task->doList;
    json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);
	while(node != NULL)
	{    
	    cgilog(SYSLOG_DEBUG, "node != NULL\n");
	    cgilog(SYSLOG_DEBUG, "while loop  ... %s ...\n", node->key);
        getDeviceInfo(&info);
        node->value = getNetworkInfo(&info);
		packageToJson(response, NULL, task->doList);
	    node = node->next;
	}
	return ret;
}

