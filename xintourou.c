ast_debug(1, "monitorfilename = %s\n", monitorfilename);
//strcpy(qe->parent->monfmt,isrecord);
ast_debug(1, "qe->parent->monfmt = %s\n", qe->parent->monfmt);
ast_debug(1, "qe->parent->wrapuptime = %d\n", qe->parent->wrapuptime);
ast_debug(1, "qe->parent->maxlen = %d\n", qe->parent->maxlen);	
/*jklou*/
if(isrecord)
{
	ast_debug(1, "can record\n");
	//pass
}else{
	struct ast_config *cfg;
	struct ast_flags config_flags = { 0 ? CONFIG_FLAG_FILEUNCHANGED : 0 };
	const char * val;
	char exname[8] = "";
	char pp[80];
	ast_debug(1, "peer->name = %s\n", peer->name);
	ast_copy_string(pp, peer->name, sizeof(pp));
	ast_debug(1, "pp = %s\n", pp);
	int i = 0;
	int j = 0;
	while(pp[j] && pp[j] != '-'){
		if(pp[j] > '9' || pp[j] < '0'){
			j++;
		}else{
			exname[i] = pp[j];
			i++;
			j++;
		}
	}
	ast_debug(1, "exname = %s\n", exname);
	cfg = ast_config_load("sip_users.conf", config_flags);
	val = ast_variable_retrieve(cfg,exname,"auto_record");
	if(!strcasecmp(val,"yes")){
		isrecord = 1;
	}else{
		isrecord = 0;
	}
}
ast_debug(1, "isrecord = %d \n", isrecord);
const char* autorecordname = pbx_builtin_getvar_helper(qe->chan, "AUTO_RECORDING");/*ON or null*/
const char* recordprompt = pbx_builtin_getvar_helper(qe->chan, "RECORD_PROMPT");/*yes or no*/
ast_debug(1, "autorecordname = %s, recordprompt = %s\n", autorecordname, recordprompt);
/*jklou*/
