
#include <satellite-subsystems/IsisSolarPanelv2.h>
#include <hal/errors.h>

#include <string.h>

#include "EPS.h"
#ifdef ISISEPS
	#include <satellite-subsystems/IsisEPS.h>
#endif
#ifdef GOMEPS
	#include <satellite-subsystems/GomEPS.h>
#endif

// y[i] = a * x[i] +(1-a) * y[i-1]
voltage_t prev_avg = 0;		// y[i-1]
float alpha = 0;			//<! smoothing constant

voltage_t eps_threshold_voltages[NUMBER_OF_THRESHOLD_VOLTAGES];	// saves the current EPS logic threshold voltages

int GetBatteryVoltage(voltage_t *vbatt)
{
	int err;

	ieps_enghk_data_cdb_t p_raenghk_data_cdb;

	unsigned char index= EPS_I2C_BUS_INDEX ;

	ieps_board_t board= ieps_board_cdb1;

	ieps_statcmd_t p_rsp_code;

	err= IsisEPS_getRAEngHKDataCDB(&index, board,&p_raenghk_data_cdb,&p_rsp_code);

	&vbatt=p_raenghk_data_cdb.fields.bat_voltage;

	if(err!=E_NO_SS_ERR)
	  return 0;
	else
		return -1;
}

int EPS_Init()
{
	//a*******************************************************************************************************
    int err;
    unsigned char eps_addr= EPS_I2C_ADDR;

	err= IsisEPS_initialize(&eps_addr, 1);
	//b*******************************************************************************************************
	if (err!=E_NO_SS_ERR)
		return -1;
	//c*******************************************************************************************************
	err= IsisSolarPanelv2_initialize( slave0_spi);
	//d*******************************************************************************************************
	if (err!=0)
			return -1;
	//e*******************************************************************************************************
	err= FRAM_read((unsigned char *)&alpha, EPS_ALPHA_FILTER_VALUE_ADDR, EPS_ALPHA_FILTER_VALUE_SIZE);

	if (err!=0)
				return -1;
	//f*******************************************************************************************************
	err= FRAM_read((unsigned char *)&eps_threshold_voltages, EPS_THRESH_VOLTAGES_ADDR, EPS_THRESH_VOLTAGES_SIZE);
	if (err!=0)
					return -1;
	//g********************************************************************************************************
	err= EPS_Conditioning();
	if (err!=0)
						return -1;
	//*********************************************************************************************************
	return 0;
}

int EPS_Conditioning()
{
	return 0;
}

int UpdateAlpha(float new_alpha)
{
	return 0;
}

int UpdateThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	return 0;
}

int GetThresholdVoltages(voltage_t thresh_volts[NUMBER_OF_THRESHOLD_VOLTAGES])
{
	return 0;
}

int GetAlpha(float *alpha)
{
	return 0;
}

int RestoreDefaultAlpha()
{
	return 0;
}

int RestoreDefaultThresholdVoltages()
{
	return 0;
}

