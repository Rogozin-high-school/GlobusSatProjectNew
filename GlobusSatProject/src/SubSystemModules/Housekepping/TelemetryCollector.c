#include <hcc/api_fat.h>

#include "GlobalStandards.h"

#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

#include <satellite-subsystems/IsisTRXVU.h>
#include <satellite-subsystems/IsisAntS.h>
#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <hal/Timing/Time.h>

#include <string.h>

#include "TelemetryCollector.h"
#include "TelemetryFiles.h"
#include "TLM_management.h"
#include "SubSystemModules/Maintenance/Maintenance.h"

int GetTelemetryFilenameByType(tlm_type_t tlm_type, char filename[MAX_F_FILE_NAME_SIZE])
{

	if (filename !=-1)
		return -1;
	switch(tlm_type)
		{

	     case tlm_wod:
		              strcpy(filename, FILENAME_WOD_TLM);
		              break;
	     case tlm_eps_raw_mb:
	                 	    strcpy(filename, FILENAME_EPS_RAW_MB_TLM);
		                    break;
	     case tlm_eps_eng_mb:
	    	                strcpy(filename, FILENAME_EPS_ENG_MB_TLM);
		                    break;
	    case tlm_eps_raw_cdb:
	                     	strcpy(filename, FILENAME_EPS_RAW_CDB_TLM);
	                    	break;
	    case tlm_eps_eng_cdb:
	    	                strcpy(filename, FILENAME_EPS_ENG_CDB_TLM);
		                    break;
	   case tlm_solar:
	               	 strcpy(filename, FILENAME_SOLAR_PANELS_TLM);
		             break;
	    case tlm_tx :
				     strcpy(filename, FILENAME_TX_TLM);
				     break;
	    case tlm_tx_revc:
				         strcpy(filename, FILENAME_TX_REVC);
				         break;
		case tlm_rx:
			             strcpy(filename, FILENAME_RX_TLM);
			             break;
		case tlm_rx_revc :
			             strcpy(filename, FILENAME_RX_REVC);
			             break;
		case tlm_rx_frame :
			   		    strcpy(filename, FILENAME_RX_FRAME);
					    break;
		case tlm_antenna:
			            strcpy(filename,  FILENAME_ANTENNA_TLM);
			            break;
		default:
			   return -1;
			   break;
		}
		return 0;

}

void TelemetryCollectorLogic()
{
}

void TelemetrySaveEPS()
{
}

void TelemetrySaveTRXVU()
{
}

void TelemetrySaveANT()
{
}

void TelemetrySaveSolarPanels()
{
}

void TelemetrySaveWOD()
{
}

void GetCurrentWODTelemetry(WOD_Telemetry_t *wod)
{
}

