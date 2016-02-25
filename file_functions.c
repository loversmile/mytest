/****************************************************************************
 *
 * FILENAME:        $RCSfile: file_functions.c,v $
 *
 * LAST REVISION:   $Revision: 1.0 $
 * LAST MODIFIED:   $Date: 2013/06/03 09:24:42 $
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
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/easy.h>


#include "global.h"

#ifdef MEMWATCH
    #include "memwatch.h"
#endif

/*Need to free*/
char* fileRead(const char *fileName, const char *regexString)
{
    char content[WEBCGI_AMI_BUFF_SIZE] = { 0 };
    char line[2048] = { 0 };
    int space = 0;
    char *ret = NULL;
    FILE *fptr = NULL;

    if (fileName == NULL)
    {
        return ret;
    }

    fptr = fopen(fileName, "r");
    if (fptr != NULL)
    {
        cgilog(SYSLOG_DEBUG,  "Open successfully ! %s", fileName);

        fseek(fptr, 0, SEEK_SET); //start from the start of file
        while (fgets(line, sizeof(line) - 1, fptr) != NULL)
        {
            space = sizeof(content) - strlen(content);
            if (space <= 1)
            {
                cgilog(SYSLOG_WARNING,  "file content is too large !");
                break;
            }
            strncat(content, line, (space - 1));
            memset(line, 0, sizeof(line));
        }

        if (regexString != NULL)
        {
            ret = regexCheck(content, regexString); 
        }
        else
        {
            ret = strDup(content);
        }
    }

    if (fptr != NULL)
    {
        fclose(fptr);
    }

    return ret;
}

/*Need to free*/
char* cmdRead(const char *cmd, const char *regexString)
{
    char content[WEBCGI_AMI_BUFF_SIZE] = { 0 };
    char line[2048] = { 0 };
    int space = 0;
    char *ret = NULL;
    FILE *fp = NULL;

    if (cmd == NULL || cmd[0] == '\0')
    {
        cgilog(SYSLOG_ERR, "CMDRead: cmd is NULL!");
        return ret;
    }
    else
    {
        cgilog(SYSLOG_DEBUG, "CMDRead: cmd is %s!", cmd);
    }

    fp = popen(cmd, "r");

    if (fp != NULL)
    {
        while ((!feof(fp)) && (fgets(line, sizeof(line) - 1, fp) != NULL))
        {
            cgilog(SYSLOG_DEBUG, "CMDRead %s", line);
            space = sizeof(content) - strlen(content);
            if (space <= 1)
            {
                cgilog(SYSLOG_WARNING,  "file content is too large !");
                break;
            }
            strncat(content, line, (space - 1));
            memset(line, 0, sizeof(line));

        }

        if (regexString != NULL)
        {
            ret = regexCheck(content, regexString); 
        }
        else
        {
            ret = strDup(content);
        }

    }
    else
    {
        cgilog(SYSLOG_ERR, "CMDRead: cmd isn't exist!");
    }

    if (fp != NULL)
    {
        pclose(fp);
    }

    return ret;
}

int getInfoFromFileOrCMD(Task *task)
{
    cgilog(SYSLOG_DEBUG, "Enter getInfoFromFileOrCMD!!!!");

    int ret = CGICODE_SUCCESS;
    DoList *node = task->doList;
    json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);

    while (node != NULL)
    {

        cgilog(SYSLOG_DEBUG, "node != NULL!!!!");

        if (node->node != NULL && node->node->pathList != NULL)
        {
            cgilog(SYSLOG_DEBUG, "node IS %s!!!!", node->key);

            if (node->value != NULL)
            {
                free(node->value);
                node->value = NULL;
            }
            if(strcasecmp(node->key, "system-time") == 0 )
	        {
	            char date[256] = {0};
                formatLocalTime(time( NULL ), date, sizeof(date)); 
				node->value = date;
	        }
            else if (node->node->pathList->type == PATHTYPE_FILE_NORMAL)
            {
                node->value = fileRead(node->node->pathList->key, node->node->pathList->regex);
            }
            else if (node->node->pathList->type == PATHTYPE_CMD_NORMAL)
            {
                node->value = cmdRead(node->node->pathList->key, node->node->pathList->regex);
            }

            if (node->value != NULL)
            {
                cgilog(SYSLOG_DEBUG, "result is [%s]!!!!", node->value);
                json_object_object_add(response, node->key, json_object_new_string(node->value));
            }
        }
        node = node->next;
    }

    cgilog(SYSLOG_DEBUG, "Exit getInfoFromFileOrCMD!!!!");

    return ret;
}

int getInfoFromCMDWithArgs(Task *task)
{
    cgilog(SYSLOG_DEBUG, "Enter getInfoFromCMDWithArgs!!!!");

    int ret = CGICODE_SUCCESS;
    DoList *node = task->doList;
    char cmd[256] = {0};
    json_object *response = json_object_object_get(task->response, CGI_JSON_RESPONSE);
    PathList *path_node = NULL;

    while (node != NULL)
    {
        cgilog(SYSLOG_DEBUG, "node != NULL!!!!");

        if (node->node != NULL)
        {
            cgilog(SYSLOG_DEBUG, "node IS %s!!!!", node->key);

            path_node = node->node->pathList;
            while (path_node != NULL)
            {

                if (path_node->type == PATHTYPE_CMD_NORMAL)
                {
                    if(node->value != NULL)
                    {
                        snprintf(cmd, sizeof(cmd), "%s%s", path_node->key, node->value);
                        cgilog(SYSLOG_DEBUG, "cmd is:%s", cmd);
                        free(node->value);
                        node->value = cmdRead(cmd, path_node->regex);
                    }
                    else
                    {
                        node->value = cmdRead(path_node->key, path_node->regex);
                    }

                    break;
                }

                if (node->value != NULL)
                {
                    json_object_object_add(response, node->key, json_object_new_string(node->value));
                }

                path_node = path_node->next;
            }
        }

        node = node->next;
    }

    cgilog(SYSLOG_DEBUG, "Exit getInfoFromCMDWithArgs!!!!");

    return ret;    
}

static size_t writeFile(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

int fetchRemoteFile(const char *url, const char *path)
{
    CURL *curl = NULL;
    FILE *fp = NULL;

    if (url == NULL || path == NULL
        || url[0] == '\0' || path[0] == '\0')
    {
        return 1;
    }
    cgilog(SYSLOG_DEBUG, "downloadFile : %s", url);
    cgilog(SYSLOG_DEBUG, "save to : %s", path);

    /*Stop the session timeout*/
    //cookieSetTimeoutState(1);

    fp = fopen(path, "w+");

    if (fp == NULL)
    {
        return 1;
    }

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 600);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFile);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 3);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, g_userAgent);

    curl_easy_perform(curl);

    fflush(fp);
    fclose(fp);

    curl_easy_cleanup(curl);

    curl_global_cleanup();
    cgilog(SYSLOG_DEBUG, "downloadFile EXIT!");

    return 0;
}

