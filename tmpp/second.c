int checkExternalRecordings(const char *path, const char *preferred, int create)
{
    int ret = -1;
    char real_path[256] = {0}; 
    char full_path[256] = {0};
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
                if (first_item == NULL && strncmp(entry->d_name, preferred, strlen(preferred)) != 0)
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
        processed = 1;
    }

    closedir(dp);

    if (create == 0 && processed == 1)
    {
        cout << "External storage unplugged or removed, but we still can find another storage, so use that one" << endl;
    }
    else if (create == 0 && processed == 0)
    {
    }

   
    char* usb_path = nvram_get("Device_USB_path");
    char* sd_path = nvram_get("Device_SD_path");
    memset(full_path, 0, sizeof(full_path));
    readlink(UCM_SYSTEM_NORMAL_RECORDINGS, full_path, sizeof(full_path) - 1);
    if(usb_path != NULL && strncmp(usb_path, full_path, strlen(usb_path)) == 0)
    {
        cout << "still USB link path : " << full_path << endl;
    }
    else if(sd_path != NULL && strncmp(sd_path, full_path, strlen(sd_path)) == 0)
    {
        cout << "still SD link path : " << full_path << endl;
    }
    else if (strcmp(full_path, UCM_SYSTEM_NORMAL_RECORDINGS_LOCAL) != 0)
    {
        cout << "External storage unplugged or removed, recover to use local storage for normal recording" << endl;
        unlink(UCM_SYSTEM_NORMAL_RECORDINGS);
        symlink(UCM_SYSTEM_NORMAL_RECORDINGS_LOCAL, UCM_SYSTEM_NORMAL_RECORDINGS);
    }

    memset(full_path, 0, sizeof(full_path));
    readlink(UCM_SYSTEM_CONF_RECORDINGS, full_path, sizeof(full_path) - 1);
    if(usb_path != NULL && strncmp(usb_path, full_path, strlen(usb_path)) == 0)
    {
        cout << "still USB link path : " << full_path << endl;
    }
    else if(sd_path != NULL && strncmp(sd_path, full_path, strlen(sd_path)) == 0)
    {
        cout << "still SD link path : " << full_path << endl;
    }
    else if (strcmp(full_path, UCM_SYSTEM_CONF_RECORDINGS_LOCAL) != 0)
    {
        cout << "External storage unplugged or removed, recover to use local storage for conference recording" << endl;
        unlink(UCM_SYSTEM_CONF_RECORDINGS);
        symlink(UCM_SYSTEM_CONF_RECORDINGS_LOCAL, UCM_SYSTEM_CONF_RECORDINGS);
    }

    memset(full_path, 0, sizeof(full_path));
    readlink(UCM_SYSTEM_QUEUE_RECORDINGS, full_path, sizeof(full_path) - 1);
    if(usb_path != NULL && strncmp(usb_path, full_path, strlen(usb_path)) == 0)
    {
        cout << "still USB link path : " << full_path << endl;
    }
    else if(sd_path != NULL && strncmp(sd_path, full_path, strlen(sd_path)) == 0)
    {
        cout << "still SD link path : " << full_path << endl;
    }
    else if (strcmp(full_path, UCM_SYSTEM_QUEUE_RECORDINGS_LOCAL) != 0)
    {
        cout << "External storage unplugged or removed, recover to use local storage for queue recording" << endl;
        unlink(UCM_SYSTEM_QUEUE_RECORDINGS);
        symlink(UCM_SYSTEM_QUEUE_RECORDINGS_LOCAL, UCM_SYSTEM_QUEUE_RECORDINGS);
    }
    

    return ret;
}
