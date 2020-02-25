static size_t g_commandID  = 391;
static const char* g_scriptCommands[] = {"", "GOTO",
"GOSUB",
"RETURN",
"WAIT",
"BEEP",
"END",
"DIM_FLT",
"LET",
"IF",
"RND",
"DIM_FRM",
"DIM_ACT",
"FINDFRAME",
"FINDACTOR",
"FRM_LINKTO",
"HUMAN_GETITEMINRHAND",
"CALL",
"FRM_SETON",
"IFFLTINRANGE",
"PLAYSOUND",
"PLAYSOUNDSTOP",
"PLAYSOUNDEX",
"STOPSOUND",
"GETSOUNDTIME",
"ACT_SETSTATE",
"DETECTOR_ISSIGNAL",
"DETECTOR_SETSIGNAL",
"DETECTOR_INRANGE",
"SETCOMPASS",
"SETLMLEVEL",
"DETECTOR_WAITFORUSE",
"DETECTOR_WAITFORHIT",
"COMMANDBLOCK",
"CALLSUBROUTINE",
"SOUNDFADE",
"ENDOFMISSION",
"PROGRAM_STORAGE",
"DOOR_LOCK",
"DOOR_OPEN",
"DOOR_ENABLEUS",
"RECCLEAR",
"RECADDACTOR",
"RECLOAD",
"RECLOADFULL",
"RECWAITFOREND",
"RECUNLOAD",
"LOADCOLLTREE",
"EXPLOSION",
"TIMERON",
"TIMEROFF",
"CHANGE_MISSION",
"CAMERA_SETSWING",
"AIRPLANE_START",
"AIRPLANE_START"
"DEBUG_TEXT",
"GETACTORSDIST",
"GETCARDAMAGE",
"HUMAN_ADDWEAPON",
"HUMAN_CANADDWEAPON",
"HUMAN_DELWEAPON",
"HUMAN_ISWEAPON",
"PRELOADMODEL",
"HUMAN_ANYWEAPONINHAND",
"HUMAN_ANYWEAPONININVENTORY",
"WINGMAN_SETINDICATOR",
"GET_PM_HUMANSTATE",
"LOADDIFFERENCES",
"CLEARDIFFERENCES",
"{",
"}",
"SETAIPRIORITY",
"EVENT",
"SETEVENT",
"EVENT_USE_CB",
"ENEMY_CHANGEANIM",
"HUMAN_CHANGEANIM",
"DIALOG_BEGIN",
"DIALOG_END",
"DIALOG_CAMSWITCH",
"GAME_NIGHTMISSION",
"HUMAN_LOOKTOACTOR",
"HUMAN_LOOKTOFRAME",
"GETMISSIONNUMBER",
"SETMISSIONNUMBER",
"HUMAN_LINKTOHAND",
"HUMAN_UNLINKFROMHAND",
"CREATEWEAPONFROMFRAME",
"GARAGE_ADDCAR",
"GARAGE_ADDCARIDX",
"GARAGE_DELCAR",
"GARAGE_GENERATECARS",
"GARAGE_RELEASECARS",
"GARAGE_CARMANAGER",
"PLAYER_LOCKCONTROLS",
"PERSON_PLAYANIM",
"PERSON_STOPANIM",
"GUNSHOP_MENU",
"HUMAN_SETSLOT",
"ENEMY_FORCESCRIPT",
"COMPAREOWNERWITHEX",
"HUMAN_SETPROPERTY",
"HUMAN_GETPROPERTY",
"DISABLECOLLS",
"HUMAN_GETOWNER",
"HUMAN_FORCE_SETTOCAR",
"CAR_SETDESTROYMOTOR",
"ACTOR_SETDIR",
"ACTOR_SETPLACEMENT",
"GARAGE_ENABLESTEAL",
"ACTOR_SETPOS",
"SETTIMEOUTEVENT",
"EMITPARTICLE",
"STOPPARTICLE",
"SETPLAYERFIREEVENT",
"FINDNEARACTOR",
"SETPLAYERHORNEVENT",
"SETPLAYERWFALLEVENT",
"CAMERA_LOCK",
"CAMERA_UNLOCK",
"SETMISSIONNAMEID",
"HUMAN_ACTIVATEWEAPON",
"MODEL_CREATE",
"HUMAN_RESET",
"MODEL_STOPANIM",
"HUMAN_CHANGEMODEL",
"MODEL_DESTROY",
"MODEL_PLAYANIM",
"AUTOSAVEGAME",
"AUTOSAVEGAMEFULL",
"HUMAN_DEATH",
"HUMAN_TALK",
"HUMAN_ERASEAB",
"HUMAN_CREATEAB",
"POLICEMANAGER_ON",
"POLICEMANAGER_ADD",
"POLICEMANAGER_DEL",
"CAR_ENABLEUS",
"GARAGE_ADDLASTSTOLEN",
"SETCITYTRAFFICVISIBLE",
"CAR_SETDOOROPEN",
"HUMAN_SETFIRETARGET",
"DOOR_SETOPENPOSE",
"CAR_BREAKMOTOR",
"AIRPLANE_ISDESTROYED",
"HUMAN_RETURNFROMPANIC",
"CAR_GETSPEED",
"ISCARUSABLE",
"SHOWCARDAMAGE",
"WINGMAN_DELINDICATOR",
"INVENTORY_PUSH",
"INVENTORY_POP",
"CHARACTER_PUSH",
"FUCKINGBOX_RECOMPILE",
"FUCKINGBOX_ADD_DEST",
"FUCKINGBOX_GETNUMBOX",
"FUCKINGBOX_ADD",
"FUCKINGBOX_GETNUMDEST",
"FUCKINGBOX_MOVE",
"WAGON_SETEVENT",
"WAGON_GETLASTNODE",
"CAR_SETSPECCOLL",
"CAR_GETSPECCOLL",
"MISSION_OBJECTIVES",
"MISSION_OBJECTIVESREMOVE",
"MISSION_OBJECTIVESCLEAR",
"RACING_MISSION_INIT",
"RACING_MISSION_START",
"CAR_EXPLOSION",
"HUMAN_FROMCAR",
"[BREAK]",
"CAR_REMOVE_DRIVER",
"SETMODELTOCAR",
"GETENEMYAISTATE",
"GET_PM_STATE",
"GET_PM_CRASHTIME",
"GET_PM_FIRETIME",
"GET_PM_NUMPREDATORS",
"USE_LIGHTCACHE",
"M_ZASTAVZKURVENEPOLISE",
"STREAM_CREATE",
"STREAM_CONNECT",
"STREAM_PLAY",
"STREAM_PAUSE",
"STREAM_STOP",
"STREAM_DESTROY",
"STREAM_FADEVOL",
"STREAM_SETLOOP",
"SETTANKHITCOUNT",
"STREAM_GETPOS",
"CAR_GETSEATCOUNT",
"SETNOANIMHIT",
"CARLIGHT_MAIN",
"CARLIGHT_LIGHT",
"CARLIGHT_INDIC_L",
"CARLIGHT_INDIC_R",
"CAR_MUSTSTEAL",
"SETTRAFFSECTORSND",
"CAR_SETPROJECTOR",
"GARAGE_CANSTEAL",
"CAR_INWATER",
"GARAGE_SHOW",
"GETACTIVEPLAYER",
"HUMAN_FORCEFALL",
"CREATE_PHYSICALOBJECT",
"WEATHER_RESET",
"COMPAREACTORS",
"WEATHER_SETPARAM",
"GETTICKTIME",
"GETGAMETIME",
"POLICEMANAGER_SETSPEED",
"GETANGLEACTORTOACTOR",
"POLICEMANAGER_FORCEARREST",
"CAR_SETSPEED",
"CONSOLE_ADDTEXT",
"CHARACTER_POP",
"HUMAN_THROWITEM",
"CAMERA_SETRANGE",
"ACTOR_DUPLICATE",
"ACTOR_DELETE",
"CAR_FORCESTOP",
"HUMAN_HAVEBOX",
"HUMAN_HAVEBODY",
"HUMAN_STOPTALK",
"ENEMY_BLOCK",
"ENEMY_UNBLOCK",
"GET_REMOTE_FLOAT",
"GET_REMOTE_FRAME",
"GET_REMOTE_ACTOR",
"SET_REMOTE_FLOAT",
"SET_REMOTE_FRAME",
"SET_REMOTE_ACTOR",
"GETACTORFRAME",
"ZATMYSE",
"CARDAMAGEVISIBLE",
"[DEBUG_ME]",
"[DEBUG_STOP]",
"IFPLAYERSTEALCAR",
"INTRO_SUBTITLE_ADD",
"MATH_COS",
"CARTRIDGE_INVALIDATE",
"MATH_SIN",
"MATH_ABS",
"FRM_GETDIR",
"FRM_SETDIR",
"FRM_GETPOS",
"FRM_SETPOS",
"FRM_GETSCALE",
"FRM_SETSCALE",
"FRM_GETROT",
"FRM_SETROT",
"FRM_GETWORLDDIR",
"FRM_GETWORLDPOS",
"FRM_GETWORLDSCALE",
"FRM_GETWORLDROT",
"FRM_GETLOCALMATRIX",
"FRM_GETWORLDMATRIX",
"VECT_SET",
"VECT_COPY",
"VECT_MUL_SCL",
"VECT_MUL_VECT",
"VECT_MUL_QUAT",
"VECT_MUL_MATRIX",
"VECT_ADD_VECT",
"VECT_SUB_VECT",
"VECT_CROSS",
"VECT_NORMALIZE",
"VECT_MAGNITUDE",
"VECT_INVERSE",
"VECT_ANGLETO",
"MATRIX_ZERO",
"MATRIX_IDENTITY",
"MATRIX_COPY",
"MATRIX_INVERSE",
"MATRIX_MUL",
"QUAT_COPY",
"QUAT_MAKE",
"QUAT_SETDIR",
"QUAT_EXTRACT",
"QUAT_INVERSE",
"QUAT_GETROTMATRIX",
"QUAT_ADD",
"QUAT_SUB",
"QUAT_MUL_SCL",
"QUAT_MUL_QUAT",
"QUAT_DOT",
"QUAT_NORMALIZE",
"QUAT_ROTBYMATRIX",
"QUAT_SLERP",
"COLL_TESTLINE",
"VLVP",
"POLICEITCHFORPLAYER",
"CTRL_READEX",
"GETFRAMEFROMACTOR",
"ACTORUPDATEPLACEMENT",
"RACING_AUTOINVISIBLE",
"CAR_INVISIBLE",
"HUMAN_HOLSTER",
"CAR_SWITCHSHOWENERGY",
"DEBUG_GETFRAMEINFO",
"NOANIMPRELOAD",
"CTRL_READ",
"DESTROY_PHYSICALOBJECT",
"SETNULLACTOR",
"COMPAREFRAMES",
"SETNULLFRAME",
"SETFREERIDE",
"ENABLEMAP",
"FRAMEISTELEPHONE",
"CAR_CALM",
"BRIDGE_SHUTDOWN",
"CARLIGHT_INDIC_OFF",
"CITYMUSIC_ON",
"[DAN_DEBUG]",
"CITYMUSIC_OFF",
"FRM_GETPARENT",
"ISPOINTINSECTOR",
"ISPOINTINAREA",
"GETACTIVECAMERA",
"SOUND_GETVOLUME",
"SOUND_SETVOLUME",
"RACING_CHANGE_MODEL",
"CITYCACHING_TICK",
"ACTORLIGHTNESSON",
"CAR_LOCK",
"CAR_LOCK_ALL",
"PM_SHOWSYMBOL",
"PM_SHOWPROGRESS",
"PM_SETPROGRESS",
"SETFILMMUSIC",
"GETFILMMUSIC",
"HUMAN_GETSEATIDX",
"HUMAN_ENTERTOTRUCK",
"FLOATREG_PUSH",
"FLOATREG_POP",
"AIRPLANESHOWDAMAGE",
"SETNPCKILLEVENT",
"STREAM_SETPOS",
"FRM_SETALPHA",
"HUMAN_THROWGRENADE",
"DETECTOR_CREATEDYNCOLL",
"DETECTOR_ERASEDYNCOLL",
"CAR_UNBREAKABLE",
"PHOBJ_IMPULS",
"WEATHER_PREPAREBUFFER",
"SUBTITLE",
"HUMAN_CANDIE",
"VERSION_IS_GERMANY",
"VERSION_IS_EDITOR",
"GARAGE_NEZAHAZUJ",
"DAN_INTERNAL_"
"PLAY_AVI_INTRO",
"CAMERA_SETFOV",
"CAMERA_GETFOV",
"DOOR_GETSTATE",
"CAR_GETMAXLEVELS",
"CAR_GETACTLEVEL",
"CAR_SETACTLEVEL",
"FREERIDE_SCOREON",
"FREERIDE_SCORESET",
"FREERIDE_SCOREGET",
"FREERIDE_SCOREADD",
"TAXIDRIVER_ENABLE",
"HUMAN_RETURNTOTRAFF",
"TIMER_GETINTERVAL",
"TIMER_SETINTERVAL",
"HUMAN_WAITTOREADY",
"FRM_GETCHILD",
"ACTOR_ADDDORICI",
"FRM_GETNUMCHILDREN",
"CAR_REPAIR",
"POLICE_SPEED_FACTOR",
"SUBTITLE_ADD",
"NPC_SHUTUP",
"FREERIDE_ENABLECAR",
"GETCARLINENUMFROMTABLE",
"ENEMY_GETHOSTILESTATE",
"HUMAN_SHUTUP",
"PUMPER_CANWORK",
"GROUP_DISBAND",
"CAR_DISABLE_UO",
"GETLASTSAVENUM",
"INVENTORY_CLEAR",
"FREERIDE_TRAFFSETUP",
"POCZKURVENYCHBEDENCAR",
"HUMAN_GETACTANIMID",
"CAR_SETAXIS",
"FRM_ISON",
"ISVALIDTAXIPASSENGER",
"missing command"
"DAN_INTERNAL_"
"POLICE_SUPPORT"
};