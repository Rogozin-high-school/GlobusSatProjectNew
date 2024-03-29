#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/Time.h>

#include "GlobalStandards.h"

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>

#include <hcc/api_fat.h>

#include "SubSystemModules/Communication/AckHandler.h"
#include "SubSystemModules/Communication/TRXVU.h"
#include "TLM_management.h"
#include "Maintenance.h"

Boolean CheckExecutionTime(time_unix prev_time, time_unix period)
{
	return FALSE;
}

Boolean CheckExecTimeFromFRAM(unsigned int fram_time_addr, time_unix period)
{
	return FALSE;
}

void SaveSatTimeInFRAM(unsigned int time_addr, unsigned int time_size)
{
}

Boolean IsFS_Corrupted()
{
	return FALSE;
}

int WakeupFromResetCMD()
{
	return 0;
}

void ResetGroundCommWDT()
{
}

// check if last communication with the ground station has passed WDT kick time
// and return a boolean describing it.
Boolean IsGroundCommunicationWDTKick()
{
	if ( DEFAULT_NO_COMM_WDT_KICK_TIME)
		restart();//LAST TIME THAT WAS COMMUNICATION
	return FALSE;
}

//TODO: add to command dictionary
int SetGsWdtKickTime(time_unix new_gs_wdt_kick_time)
{

	//by diana
	if (new_gs_wdt_kick_time< 86400*2 || new_gs_wdt_kick_time>1*86400)
		return -3;
	int err=  FRAM_write(&new_gs_wdt_kick_time, LAST_WAKEUP_TIME_ADDR, LAST_WAKEUP_TIME_SIZE);
	if (err!=0)
	  return err;
	return 0;
}

time_unix GetGsWdtKickTime()
{
	//by diana
	time_unix no_comm_thresh = 0;
	int err= FRAM_read(&no_comm_thresh, NO_COMM_WDT_KICK_TIME_ADDR, NO_COMM_WDT_KICK_TIME_SIZE);
	if (err!=0)
		  return err;
	return no_comm_thresh;
}

void Maintenance()
{
}
