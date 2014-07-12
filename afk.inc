//AFK Auto-Detect v1.0 
//Credits to: Mihaii (Mihaitza. - BBGaming.ro)

#include <a_samp>
#include <foreach>

new PlayerAFK[MAX_PLAYERS]; // Seteaza variabila [0-NOT-AFK | 1-AFK] | Set player variable [0-NOT-AFK | 1-AFK]
new AFKDetect[MAX_PLAYERS]; // Seteaza timpul curent din OPU | Set current date/time to player from OnPlayerUpdate(OPU)
new bool: FunctionCall[2][MAX_PLAYERS]; // Ajuta sa nu se repete functiile OnPlayerPause si/sau OnPlayerUnpause | Helps not to repeat OnPlayerPause or/and OnPlayerUnpause 

forward OnPlayerPause(playerid);
forward OnPlayerUnpause(playerid);
forward AFKChecker();

public OnPlayerUpdate(playerid)
{
	AFKDetect[playerid] = gettime();
    #if defined VFUNC_OnPlayerUpdate
        return VFUNC_OnPlayerUpdate(playerid);
    #else
        return 1;
    #endif
}
#if defined VFUNC_OnPlayerUpdate
    forward VFUNC_OnPlayerUpdate(playerid);
#endif
#if defined _ALS_OnPlayerUpdate
    #undef OnPlayerUpdate
#else
    #define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate VFUNC_OnPlayerUpdate
public OnGameModeInit()
{
	SetTimer("AFKChecker",1000,true);
    #if defined VFUNC_OnGameModeInit
        return VFUNC_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined VFUNC_OnGameModeInit
    forward VFUNC_OnGameModeInit();
#endif
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit VFUNC_OnGameModeInit
public AFKChecker()
{
	foreach(Player, i)
	{
		if(gettime()-AFKDetect[i] > 5)
		{
			if(PlayerAFK[i] == 0 && FunctionCall[0][i] == false)
				PlayerAFK[i] = 1,
					CallLocalFunction("OnPlayerPause", "i", i),
						FunctionCall[0][i] = true,
							FunctionCall[1][i] = false; 
		}
		else 
		{
			if(PlayerAFK[i] == 1 && FunctionCall[1][i] == false)
				PlayerAFK[i] = 0,
					CallLocalFunction("OnPlayerUnpause", "i", i),
						FunctionCall[1][i] = true,
							FunctionCall[0][i] = false;
		}
	}
	return 1;
}
