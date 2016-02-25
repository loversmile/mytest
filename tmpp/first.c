int checkExternalRecordings_auto(const char *path, const char *preferred, int create)
{
    int ret = -1;
    char real_path[256] = {0}; 
    char full_path[256] = {0};
    char mac_addr[32] = {0};
    char *ptr = NULL, *tmp = NULL;
    DIR *dp = NULL;
    struct dirent *entry = NULL;
    int processed = 0;
    const char *preferred_item = NULL;
    const char *first_item = NULL;

    if ( path == NULL)
    {
        return ret;
    }

    if ( (dp = opendir(path)) == NULL )
    {
        cout<<"can not open "<<path<<endl;
        return ret;
    }

    ptr = devMac;
    tmp = mac_addr;
    while (ptr != NULL && *ptr != '\0')
    {
        if (*ptr == ':')
        {
            ;/*Skip the ":" */
        }
        else if (tmp < mac_addr + sizeof(mac_addr))
        {
            *tmp = *ptr;
            tmp++;
        }
        ptr++;
    }
    
    ret = 0;
    first_item = NULL;
    preferred_item = NULL;
    while ( (entry = readdir(dp)) != NULL )
    {
        if ( strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 )
            continue;

        /*Try to find a preferred storage with specific prefix */
        if ( entry->d_type == DT_DIR )
        { 
            snprintf(real_path, sizeof(real_path), "%s/%s", path, entry->d_name);
            if ( isRealExternalDisk(real_path) == true )
            {
                if (first_item == NULL)
                {
                    first_item = entry->d_name;
                }

                if (preferred != NULL && preferred_item == NULL && strncmp(entry->d_name, preferred, strlen(preferred)) == 0)
                {
                    preferred_item = entry->d_name;
                }
            }
        }
    }

    /*Try to use a preferred storage*/
    if (preferred_item != NULL)
    {
        snprintf(real_path, sizeof(real_path), "%s/%s", path, preferred_item);
        nvram_set("Device_USB_path", real_path);
        nvram_commit();
        cout << "nvram set Device_USB_path=" << real_path << endl;
    }
    else
    {
        nvram_unset("Device_USB_path");
        nvram_commit();
    }
    /*If there is no preferred storage, use the found first one */
    if (first_item != NULL)
    {
        snprintf(real_path, sizeof(real_path), "%s/%s", path, first_item);
        nvram_set("Device_SD_path", real_path);
        nvram_commit();
        cout << "nvram set Device_SD_path=" << real_path << endl;
    }
    else
    {
        nvram_unset("Device_SD_path");
        nvram_commit();
    }
    
    if(first_item == NULL && preferred_item == NULL)
    {
        nvram_unset("Device_USB_path");
        nvram_unset("Device_SD_path");
        nvram_commit();
        real_path[0] = '\0';
    }

    if ( strlen(real_path) != 0 )
    {
        /* Check the link first, if it is linked to the destination which wants to set, skip the procedure */
        readlink(UCM_SYSTEM_NORMAL_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strncmp(full_path, real_path, strlen(real_path)) != 0)
        {
            unlink(UCM_SYSTEM_NORMAL_RECORDINGS);
            snprintf(full_path, sizeof(full_path), "%s/%s%s", real_path, UCM_EXTERNAL_NORMAL_RECORDINGS, mac_addr);
            ret = mkdir(full_path, 0755);
            if (ret == 0)
            {
                cout << "Create folder for normal recordings successfully " << endl;
            }
            else
            {
                cout << "Create folder for normal recordings failed: " << strerror(errno) << endl;
            }
            symlink(full_path, UCM_SYSTEM_NORMAL_RECORDINGS);
        }
        else
        {
            cout << "The link for normal recordings is unchanged:" << full_path << endl;
        }

        readlink(UCM_SYSTEM_CONF_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strncmp(full_path, real_path, strlen(real_path)) != 0)
        {
            unlink(UCM_SYSTEM_CONF_RECORDINGS);
            snprintf(full_path, sizeof(full_path), "%s/%s%s", real_path, UCM_EXTERNAL_CONF_RECORDINGS, mac_addr);
            ret = mkdir(full_path, 0755);
            if (ret == 0)
            {
                cout << "Create folder for conference recordings successfully " << endl;
            }
            else
            {
                cout << "Create folder for conference recordings failed: " << strerror(errno) << endl;
            }
            symlink(full_path, UCM_SYSTEM_CONF_RECORDINGS);
        }
        else
        {
            cout << "The link for conference recordings is unchanged:" << full_path << endl;
        }

        /*added by jklou*/
        readlink(UCM_SYSTEM_QUEUE_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strncmp(full_path, real_path, strlen(real_path)) != 0)
        {
            unlink(UCM_SYSTEM_QUEUE_RECORDINGS);
            snprintf(full_path, sizeof(full_path), "%s/%s%s", real_path, UCM_EXTERNAL_QUEUE_RECORDINGS, mac_addr);
            ret = mkdir(full_path, 0755);
            if (ret == 0)
            {
                cout << "Create folder for queue recordings successfully " << endl;
            }
            else
            {
                cout << "Create folder for queue recordings failed: " << strerror(errno) << endl;
            }
            symlink(full_path, UCM_SYSTEM_QUEUE_RECORDINGS);
        }
        else
        {
            cout << "The link for queue recordings is unchanged:" << full_path << endl;
        }
        /*end jklou*/
        processed = 1;
    }

    closedir(dp);

    if (create == 0 && processed == 1)
    {
        cout << "External storage unplugged or removed, but we still can find another storage, so use that one" << endl;
    }
    else if (create == 0 && processed == 0)
    {
        readlink(UCM_SYSTEM_NORMAL_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strcmp(full_path, UCM_SYSTEM_NORMAL_RECORDINGS_LOCAL) != 0)
        {
            cout << "External storage unplugged or removed, recover to use local storage for normal recording" << endl;
            unlink(UCM_SYSTEM_NORMAL_RECORDINGS);
            symlink(UCM_SYSTEM_NORMAL_RECORDINGS_LOCAL, UCM_SYSTEM_NORMAL_RECORDINGS);
        }

        readlink(UCM_SYSTEM_CONF_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strcmp(full_path, UCM_SYSTEM_CONF_RECORDINGS_LOCAL) != 0)
        {
            cout << "External storage unplugged or removed, recover to use local storage for conference recording" << endl;
            unlink(UCM_SYSTEM_CONF_RECORDINGS);
            symlink(UCM_SYSTEM_CONF_RECORDINGS_LOCAL, UCM_SYSTEM_CONF_RECORDINGS);
        }

        /*added by jklou */
        readlink(UCM_SYSTEM_QUEUE_RECORDINGS, full_path, sizeof(full_path) - 1);
        if (strcmp(full_path, UCM_SYSTEM_QUEUE_RECORDINGS_LOCAL) != 0)
        {
            cout << "External storage unplugged or removed, recover to use local storage for queue recording" << endl;
            unlink(UCM_SYSTEM_QUEUE_RECORDINGS);
            symlink(UCM_SYSTEM_QUEUE_RECORDINGS_LOCAL, UCM_SYSTEM_QUEUE_RECORDINGS);
        }
        /*end jklou*/
    }

    return ret;
}
