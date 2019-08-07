#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <hal/Timing/Time.h>
#include <hal/errors.h>

#include <satellite-subsystems/IsisTRXVU.h>

#include <stdlib.h>
#include <string.h>

#include "GlobalStandards.h"
#include "TRXVU.h"
#include "AckHandler.h"
#include "ActUponCommand.h"
#include "SatCommandHandler.h"
#include "TLM_management.h"

#include "SubSystemModules/PowerManagment/EPS.h"
#include "SubSystemModules/Maintenance/Maintenance.h"
#include "SubSystemModules/Housekepping/TelemetryCollector.h"
#ifdef TESTING_TRXVU_FRAME_LENGTH
#include <hal/Utility/util.h>
#endif
#define SIZE_RXFRAME	200
#define SIZE_TXFRAME	235


Boolean 		g_mute_flag = MUTE_OFF;				// mute flag - is the mute enabled
time_unix 		g_mute_end_time = 0;				// time at which the mute will end
time_unix 		g_prev_beacon_time = 0;				// the time at which the previous beacon occured
time_unix 		g_beacon_interval_time = 0;			// seconds between each beacon
unsigned char	g_current_beacon_period = 0;		// marks the current beacon cycle(how many were transmitted before change in baud)
unsigned char 	g_beacon_change_baud_period = 0;	// every 'g_beacon_change_baud_period' beacon will be in 1200Bps and not 9600Bps

xQueueHandle xDumpQueue = NULL;
xSemaphoreHandle xDumpLock = NULL;
xTaskHandle xDumpHandle = NULL;			 //task handle for dump task
xSemaphoreHandle xIsTransmitting = NULL; // mutex on transmission.

void InitSemaphores()
{
}

int InitTrxvu() {


	    ISIStrxvuI2CAddress myTRXVUAddress;
		ISIStrxvuFrameLengths myTRXVUBuffers;

		int retValInt = 0;

		//Buffer definition
		myTRXVUBuffers.maxAX25frameLengthTX = SIZE_TXFRAME; //SIZE_TXFRAME;
		myTRXVUBuffers.maxAX25frameLengthRX = SIZE_RXFRAME;

		//I2C addresses defined
		myTRXVUAddress.addressVu_rc = I2C_TRXVU_RC_ADDR;
		myTRXVUAddress.addressVu_tc = I2C_TRXVU_TC_ADDR;

		//Bitrate definition
		ISIStrxvuBitrate myTRXVUBitrates = trxvu_bitrate_9600;

		retValInt = IsisTrxvu_initialize(&myTRXVUAddress, &myTRXVUBuffers,
				&myTRXVUBitrates, 1);

		if (retValInt != 0){
			return retValInt;
		}

		//DELAY FOR DOING COMMANDS
		vTaskDelay(100);

		// antenna init
		ISISantsI2Caddress myAntennaAddress;
		myAntennaAddress.addressSideA = ANTS_I2C_SIDE_A_ADDR;
		myAntennaAddress.addressSideB = ANTS_I2C_SIDE_B_ADDR;

		//Initialize the AntS system
		// ONE IS FOR TWO ANTS
		retValInt = IsisAntS_initialize(&myAntennaAddress, 1);

		if (retValInt != 0){
			return retValInt;
		}
		vTaskDelay(100);

		InitSemaphores();

		if (0 != FRAM_read(&g_beacon_change_baud_period,
		BEACON_BITRATE_CYCLE_ADDR, BEACON_BITRATE_CYCLE_SIZE)){
			g_beacon_change_baud_period = DEFALUT_BEACON_BITRATE_CYCLE;
		}

		if (0 != FRAM_read((unsigned char*) &g_beacon_interval_time,
		BEACON_INTERVAL_TIME_ADDR, BEACON_INTERVAL_TIME_SIZE)){
			g_beacon_interval_time = DEFAULT_BEACON_INTERVAL_TIME;
		}
		return E_NO_SS_ERR;

}

int TRX_Logic() {

	sat_packet_t cmd;
	int a=GetOnlineCommand(&cmd);
	if (a!=E_NO_SS_ERR)
		{	return execution_error ;}
	int b= SendAckPacket(ack_subtype_t acksubtype, sat_packet_t *cmd,
			unsigned char *data, unsigned int length);
	if (b!=E_NO_SS_ERR)
	{	return execution_error ;}
	return E_NO_SS_ERR;
}

int GetNumberOfFramesInBuffer() {
	return 0;
}

Boolean CheckTransmitionAllowed() {
	return FALSE;
}


void FinishDump(dump_arguments_t *task_args,unsigned char *buffer, ack_subtype_t acktype,
		unsigned char *err, unsigned int size) {
}

void AbortDump()
{
}

void SendDumpAbortRequest() {
}

Boolean CheckDumpAbort() {
	return FALSE;
}

void DumpTask(void *args) {
}

int DumpTelemetry(sat_packet_t *cmd) {
	return 0;
}

//Sets the bitrate to 1200 every third beacon and to 9600 otherwise
int BeaconSetBitrate() {
	return 0;
}

void BeaconLogic() {
}

int muteTRXVU(time_unix duration) {
	return 0;
}

void UnMuteTRXVU() {
}

Boolean GetMuteFlag() {
	return FALSE;
}

Boolean CheckForMuteEnd() {
	return FALSE;
}

int GetTrxvuBitrate(ISIStrxvuBitrateStatus *bitrate) {
	return 0;
}

int TransmitDataAsSPL_Packet(sat_packet_t *cmd, unsigned char *data,
		unsigned int length) {
	return 0;
}

int TransmitSplPacket(sat_packet_t *packet, int *avalFrames) {
	return 0;
}

int UpdateBeaconBaudCycle(unsigned char cycle)
{
	return 0;
}

int UpdateBeaconInterval(time_unix intrvl) {
	return 0;
}
