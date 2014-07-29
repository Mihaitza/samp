//Anti-Cheat (Anti-Speed Hack)
//Version: 0.1
//Credits to: Mihai (Mihaitza. - BBGaming.ro)

#include <a_samp>

new szString[256];
new Float:SpeedM[3][MAX_PLAYERS];
new Float:LastPos[3][MAX_PLAYERS];

forward TCheckSpeed(playerid);
forward FallingChecker();
forward OnPlayerFall(playerid);
forward Reactivate(playerid);
forward IsAPlane(vehicleid);
forward IsABoat(vehicleid);

public OnPlayerConnect(playerid)
{
	SpeedM[1][playerid] = 0;
    #if defined AC_OnPlayerConnect
        return AC_OnPlayerConnect(playerid);
    #else
        return 1;
    #endif
}
#if defined AC_OnPlayerConnect
    forward AC_OnPlayerConnect(playerid);
#endif
#if defined _ALS_OnPlayerConnect
    #undef OnPlayerConnect
#else
    #define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AC_OnPlayerConnect

#if defined AC_OnPlayerDisconnect
    forward AC_OnPlayerDisconnect(playerid,reason);
#endif
#if defined _ALS_OnPlayerDisconnect
    #undef OnPlayerDisconnect
#else
    #define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect AC_OnPlayerDisconnect

public OnGameModeInit()
{
    SetTimer("TCheckSpeed",1000,true);
    #if defined AC_OnGameModeInit
        return AC_OnGameModeInit();
    #else
        return 1;
    #endif
}
#if defined AC_OnGameModeInit
    forward AC_OnGameModeInit();
#endif
#if defined _ALS_OnGameModeInit
    #undef OnGameModeInit
#else
    #define _ALS_OnGameModeInit
#endif
#define OnGameModeInit AC_OnGameModeInit

public OnPlayerExitVehicle(playerid, vehicleid)
{
	SpeedM[1][playerid] = 0, SpeedM[2][playerid] = 0;
    #if defined AC_OnPlayerExitVehicle
        return AC_OnPlayerExitVehicle(playerid, vehicleid);
    #else
        return 1;
    #endif
}
#if defined AC_OnPlayerExitVehicle
    forward AC_OnPlayerExitVehicle(playerid, vehicleid);
#endif
#if defined _ALS_OnPlayerExitVehicle
    #undef OnPlayerExitVehicle
#else
    #define _ALS_OnPlayerExitVehicle
#endif
#define OnPlayerExitVehicle AC_OnPlayerExitVehicle

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(newstate == PLAYER_STATE_DRIVER) SpeedM[1][playerid] = 0, SpeedM[2][playerid] = 0;
    #if defined AC_OnPlayerStateChange
        return AC_OnPlayerStateChange(playerid, newstate, oldstate);
    #else
        return 1;
    #endif
}
#if defined AC_OnPlayerStateChange
    forward AC_OnPlayerStateChange(playerid, newstate, oldstate);
#endif
#if defined _ALS_OnPlayerStateChange
    #undef OnPlayerStateChange
#else
    #define _ALS_OnPlayerStateChange
#endif
#define OnPlayerStateChange AC_OnPlayerStateChange

public TCheckSpeed(playerid)
{
	foreach(Player, i)
	{
		if(IsPlayerInAnyVehicle(playerid) && !GetPVarInt(playerid, "Fall") && (!IsAPlane(GetPlayerVehicleID(playerid)) || !IsABoat(GetPlayerVehicleID(playerid))))
		{
			SpeedM[0][playerid] = GetVehicleSpeeds(GetPlayerVehicleID(playerid));
			if(SpeedM[0][playerid] > 160)
			{
				SpeedM[2][playerid] = SpeedM[0][playerid]-SpeedM[1][playerid];
				if(SpeedM[2][playerid] > 42)
				{
					GetPlayerName(playerid, playerAName,sizeof(playerAName));
					format(szString,sizeof(szString),"[BBG][Global]: {FFFFFF}Player %s has been kicked. Reason: Speed Hack",playerAName);
					SendClientMessageToAll(0xF00C17FF,szString);
				}
			}
			SpeedM[1][playerid] = SpeedM[0][playerid];
		}
	}
	FallingChecker();
	return 1;
}
public FallingChecker()
{
   new Float:x,Float:y,Float:z,Float:d;
   foreach(Player, i)
   {
     if(IsPlayerConnected(i))
     {
        GetPlayerPos(i,x,y,z);
        d = floatsqroot((x-LastPos[0][i] * x-LastPos[0][i]) + (y-LastPos[1][i] * y-LastPos[1][i]));
        if(d < 10 && (LastPos[2][i] - z) > 5) OnPlayerFall(i);
        LastPos[0][i] = x;
        LastPos[1][i] = y;
        LastPos[2][i] = z;
     }
   }
   return 1;
}
public OnPlayerFall(playerid)
{
	if(GetPVarInt(playerid, "Fall")) return 1;
	SetTimerEx("Reactivate",8000,false,"i",playerid);
	SetPVarInt(playerid, "Fall",1);
	SpeedM[0][playerid] = GetVehicleSpeeds(GetPlayerVehicleID(playerid));
	SpeedM[1][playerid] = SpeedM[0][playerid];
	SpeedM[2][playerid] = 0;
    return 1;
}
public Reactivate(playerid)
{
	SpeedM[0][playerid] = GetVehicleSpeeds(GetPlayerVehicleID(playerid));
	SpeedM[1][playerid] = SpeedM[0][playerid];
	SpeedM[2][playerid] = 0;
	SetPVarInt(playerid, "Fall", 0);
    return 1;
}
stock GetVehicleSpeeds( vehicleid )
{
    new
        Float:x,
        Float:y,
        Float:z,
        vel;
    GetVehicleVelocity( vehicleid, x, y, z );
    vel = floatround( floatsqroot( x*x + y*y + z*z ) * 180 );
    return vel;
}
public IsAPlane(vehicleid)
{
	new result;
	new model = GetVehicleModel(vehicleid);
    switch(model)
    {
        case 417, 425, 447, 460, 464, 465, 469, 476, 487, 488, 497, 511, 512, 513, 519, 520, 548, 553, 563, 577, 592, 593: result = model;
        default: result = 0;
    }
	return result;
}
public IsABoat(vehicleid)
{
	new result;
	new model = GetVehicleModel(vehicleid);
    switch(model)
    {
        case 430, 446, 452, 453, 454, 484, 595, 472, 473, 493: result = model;
        default: result = 0;
    }
	return result;
}
