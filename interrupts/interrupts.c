/*
 * interrupts.c
 *
 *  Created on: Mar 3, 2022
 *      Author: Lenovo
 */

#include "interrupts.h"

void genixrs485_intrpt_handler(void)
{
	static GENIX_RCV_STATE_t rcvState = RCV_HEADER_CHECK;
	static uint16_t head;
	static uint16_t tail;
	static uint8_t 	count;
	static uint8_t 	length;
	static char buffer[32];
	uint8_t data;

	XMC_UART_CH_ClearStatusFlag(XMC_USIC1_CH0,
								XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION);

	data = XMC_UART_CH_GetReceivedData(XMC_USIC1_CH0);
	switch(rcvState)
	{
		case RCV_HEADER_CHECK	:
			head = (head << 8) | data;
			if(head == 0xAABB)
			{
				XMC_GPIO_SetOutputLow(P2_11);
				rcvState = RCV_DATA_LENGTH;
				head	 = 0x0000;
				length   = 0;
			}
			break;
		case RCV_DATA_LENGTH	:
			if(data == 32)
			{

				length = data-4; // dont know why substracted by 4. LoL
				rcvState = RCV_RESPONSE_CODE;
			}
			else
			{
				rcvState = RCV_HEADER_CHECK;
			}
			break;
		case RCV_RESPONSE_CODE	:
			if(data == GENIX_SUMMARY)
			{

				rcvState = RCV_COLLECT_DATA;
				count=0;
			}
			else
			{
				rcvState = RCV_HEADER_CHECK;
			}
			break;
		case RCV_COLLECT_DATA	:
			if(count == length-1)
			{

				buffer[count] = data;
				rcvState = RCV_ECC_CHECK;
				count = 0;
			}
			else
			{
				buffer[count] = data;
				count++;
			}
			break;
		case RCV_ECC_CHECK :
			tail = 0x0000;
			rcvState = RCV_TAIL_CHECK;
			break;
		case RCV_TAIL_CHECK :
			tail = (tail << 8) | data;
			if(count == 1)
			{
				if(tail == 0x0D0A)
				{
					/* do a parse */
					/* Battery Voltage */
					genix_data.pack_voltage = (((uint16_t)(buffer[0]) << 8) | buffer[1]);
					data_group.vBat_genix = genix_data.pack_voltage;
					/* Current */
					genix_data.pack_current = ((((int16_t)(buffer[2]) << 8) | buffer[3]) - 30000);
					data_group.iBat_genix = (genix_data.pack_current)/10;
					/* Max Cell Temperature */
					genix_data.pack_max_temp = ((uint16_t)(buffer[11])-40)*10;
					data_group.temp_genix = genix_data.pack_max_temp/10;
					/* SoC */
					genix_data.pack_soc = (uint16_t)(buffer[4]);
					data_group.soC = genix_data.pack_soc/10;
					count = 0;
				}
				rcvState = RCV_HEADER_CHECK;
			}
			else count++;
			break;
		default				:break;
	}
}

void esp32uart_intrpt_handler(void)
{
	static ESP_RECEIVE_STATE_t state = ESP_SOF;
	static uint8_t length;
	static uint8_t count=0;
	uint8_t data;
	static uint8_t crc=0;
	static uint8_t buffData[7];
	static bool isValid;
	XMC_UART_CH_ClearStatusFlag(XMC_USIC0_CH0,XMC_UART_CH_STATUS_FLAG_RECEIVE_INDICATION);
	data = XMC_UART_CH_GetReceivedData(XMC_USIC0_CH0);

	switch(state)
	{
		case ESP_SOF	:
			if(data == 0x04)
			{
				isValid = false;
				state = ESP_LENGTH;
			}
			break;
		case ESP_LENGTH	:
			length = data;
			count = 0;
			crc = 0;
			state = ESP_DATA;
			break;
		case ESP_DATA :
			ESP32_SendStream((char*)&data, 1);
			if(count == length-1)
			{
				buffData[count] = data;
				crc = ESP32_CRC8_maxim(buffData, length);
				state = ESP_CRC8;
			}
			else
			{
				buffData[count] = data;
				count++;
			}
			break;
		case ESP_CRC8 :
			if(data == crc)
			{
//				date_data.year 		= buffData[0];
//				date_data.year 		= (date_data.year << 8) | buffData[1];
//				date_data.month		= buffData[2];
//				date_data.date		= buffData[3];
//				time_data.hour		= buffData[4];
//				time_data.minute	= buffData[5];
//				time_data.second	= buffData[6];
				isValid				= true;
			}
			state = ESP_EOF;
			break;
		case ESP_EOF :
			if(data == 0x0B)
			{
				if(isValid)
				{
					XMC_GPIO_ToggleOutput(P2_13);
					isValid = false;
				}
			}
			state= ESP_SOF;
			break;
		default			: break;
	}
}

void pcsCAN_intrpt_handler(void)
{
	// takes 30 us
	XMC_CAN_MO_Receive(&msgObjFromPCS);
	switch(msgObjFromPCS.can_identifier)
	{
		case 0x200 : // PCS System Properties
			pcsSystemStatus = PCS_GetPCSStatus(&msgObjFromPCS);

			data_group.pcs_system = 0x00;
			if(pcsSystemStatus.pcs_system_status == PCS_STAT_INITIAL)		{data_group.pcs_system = 0x01;}
			else if(pcsSystemStatus.pcs_system_status == PCS_STAT_STANDBY)	{data_group.pcs_system = 0x02;}
			else if(pcsSystemStatus.pcs_system_status == PCS_STAT_RUNNING)	{data_group.pcs_system = 0x03;}

			if(pcsSystemStatus.pcs_precharge_IsComplete) {data_group.pcs_system |= 0x04;}
			else {data_group.pcs_system |= 0x00;}

			if(pcsSystemStatus.pcs_system_warning == PCS_WARN_NORMAL) 			{data_group.pcs_system |= (0x00 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_PCS_OT) 	{data_group.pcs_system |= (0x01 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_BAT_OVCHG) 	{data_group.pcs_system |= (0x02 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_BAT_CHGSUP) {data_group.pcs_system |= (0x03 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_BAT_DSGSUP) {data_group.pcs_system |= (0x04 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_BAT_OVDSG)  {data_group.pcs_system |= (0x05 << 3);}
			else if (pcsSystemStatus.pcs_system_warning == PCS_WARN_BAT_OVDSG2) {data_group.pcs_system |= (0x06 << 3);}
			break;

		case 0x201 : // DC Grid Converter Properties
			pcsDCgridStatus = PCS_GetDCGridStatus(&msgObjFromPCS);

			data_group.dcgrid_system = 0x00;
			if(pcsDCgridStatus.dcgrid_system_status == GRID_STAT_NOT_RUNNING) 	{data_group.dcgrid_system |= 0x01;}
			else if(pcsDCgridStatus.dcgrid_system_status == GRID_STAT_CHG) 		{data_group.dcgrid_system |= 0x02;}
			else if(pcsDCgridStatus.dcgrid_system_status == GRID_STAT_DSG) 		{data_group.dcgrid_system |= 0x03;}

			if(pcsDCgridStatus.dcgrid_system_warning == GRID_WARN_NORMAL)	{data_group.dcgrid_system |= (0x00 << 2);}
			else if(pcsDCgridStatus.dcgrid_system_warning == GRID_WARN_OV)	{data_group.dcgrid_system |= (0x01 << 2);}
			else if(pcsDCgridStatus.dcgrid_system_warning == GRID_WARN_OC)	{data_group.dcgrid_system |= (0x02 << 2);}
			break;

		case 0x202 : // AC Inverter Properties
			pcsACinvStatus = PCS_GetACInvStatus(&msgObjFromPCS);

			data_group.inv_system = 0x00;
			if(pcsACinvStatus.acinv_system_status == INV_STAT_NOT_RUNNING) 	{data_group.inv_system |= 0x01;}
			else if(pcsACinvStatus.acinv_system_status == INV_STAT_RUNNING) {data_group.inv_system |= 0x02;}
			else if(pcsACinvStatus.acinv_system_status == INV_STAT_NONE) 	{data_group.inv_system |= 0x03;}

			if(pcsACinvStatus.acinv_system_warning == INV_WARN_NORMAL)		{data_group.inv_system |= (0x00 << 2);}
			else if(pcsACinvStatus.acinv_system_warning == INV_WARN_MCU_OV)	{data_group.inv_system |= (0x01 << 2);}
			else if(pcsACinvStatus.acinv_system_warning == INV_WARN_OT)		{data_group.inv_system |= (0x02 << 2);}
			else if(pcsACinvStatus.acinv_system_warning == INV_WARN_OV)		{data_group.inv_system |= (0x03 << 2);}
			else if(pcsACinvStatus.acinv_system_warning == INV_WARN_UV)		{data_group.inv_system |= (0x04 << 2);}
			else if(pcsACinvStatus.acinv_system_warning == INV_WARN_OL)		{data_group.inv_system |= (0x05 << 2);}
			break;

		case 0x203 : // PV Converter Properties
			pcsPVconvStatus = PCS_GetPVConvStatus(&msgObjFromPCS);

			data_group.pvconv_system = 0x00;
			if(pcsPVconvStatus.pvconv_system_status == PV_STAT_NOT_RUNNING)		{data_group.pvconv_system |= 0x01;}
			else if (pcsPVconvStatus.pvconv_system_status == PV_STAT_RUNNING) 	{data_group.pvconv_system |= 0x02;}

			if(pcsPVconvStatus.pvconv_system_warning == PV_WARN_NORMAL)		{data_group.pvconv_system |= (0x00 << 2);}
			else if(pcsPVconvStatus.pvconv_system_warning == PV_WARN_OV) 	{data_group.pvconv_system |= (0x01 << 2);}
			else if(pcsPVconvStatus.pvconv_system_warning == PV_WARN_OC) 	{data_group.pvconv_system |= (0x02 << 2);}
			break;

		case 0x204 : // Battery Converter Properties
			pcsBatconvStatus = PCS_GetBatConvStatus(&msgObjFromPCS);
			data_group.batconv_system = ((pcsBatconvStatus.batconv_system_warning & 0xFF) << 2) | (pcsBatconvStatus.batconv_system_status);

			data_group.batconv_system = 0x00;
			if(pcsBatconvStatus.batconv_system_status == BATCONV_STAT_NOT_RUNNING) 	{data_group.batconv_system |= 0x01;}
			else if(pcsBatconvStatus.batconv_system_status == BATCONV_STAT_RUNNING) {data_group.batconv_system |= 0x02;}

			if(pcsBatconvStatus.batconv_system_warning == BATCONV_WARN_NORMAL) 		 {data_group.batconv_system |= (0x00 << 2);}
			else if(pcsBatconvStatus.batconv_system_warning == BATCONV_WARN_OV) 	 {data_group.batconv_system |= (0x01 << 2);}
			else if(pcsBatconvStatus.batconv_system_warning == BATCONV_WARN_OC) 	 {data_group.batconv_system |= (0x02 << 2);}
			else if(pcsBatconvStatus.batconv_system_warning == BATCONV_WARN_DCDC_OV) {data_group.batconv_system |= (0x03 << 2);}
			else if(pcsBatconvStatus.batconv_system_warning == BATCONV_WARN_DCDC_OT) {data_group.batconv_system |= (0x04 << 2);}
			break;

		case 0x205 : // Voltage Properties
			pcsVoltages = PCS_GetVoltParam(&msgObjFromPCS);
			data_group.vPv_conv 	= pcsVoltages.vPV*0.1;
			data_group.vDC_grid 	= pcsVoltages.vdcGrid*0.1;
			data_group.vBat_conv 	= pcsVoltages.vbat*0.1;
			data_group.vAC_inv 		= pcsVoltages.vacOut*0.1;
			break;

		case 0x206 : // Current Properties
			pcsCurrents = PCS_GetAmpParam(&msgObjFromPCS);
			data_group.iPv_conv		= pcsCurrents.iPV*0.1;
			data_group.iDC_grid		= pcsCurrents.idcGrid*0.1;
			data_group.iBat_conv	= pcsCurrents.ibat*0.1;
			data_group.iAC_inv		= pcsCurrents.iacOut*0.1;
			break;

		case 0x207 : // Power Properties
			pcsPowers = PCS_GetPowerParam(&msgObjFromPCS);
			data_group.pPv_conv		= pcsPowers.pPV;
			data_group.pDC_grid		= pcsPowers.pdcGrid;
			data_group.pBat_conv	= pcsPowers.pbat;
			data_group.pAC_inv		= pcsPowers.pacOut;
			break;

		case 0x208 : // Frequency and Temperature Properties
			pcsMiscs = PCS_GetMiscParam(&msgObjFromPCS);
			data_group.temp_sensor	= pcsMiscs.tempPCS*0.1;
			data_group.frequency	= pcsMiscs.frequency*0.1;
			break;

		default    : // others
			break;
	}

	NVIC_ClearPendingIRQ(IRQ7_IRQn);
}

void adc_intrpt_handler(void)
{
	static PDU_SENSOR_AVG_t sensorAvgBuff;

	XMC_VADC_GROUP_ClearResultEvent(VADC_G1,2); // Clear Result 2 event flag
	pduSensorRaw.rawPVPT = (XMC_VADC_GROUP_GetResult(VADC_G1,0));
	pduSensorRaw.rawBTPT = (XMC_VADC_GROUP_GetResult(VADC_G1,1));
	pduSensorRaw.rawDCCT = (XMC_VADC_GROUP_GetResult(VADC_G1,2));
	pduSensorRaw.rawPVPT = (XMC_VADC_GROUP_GetResult(VADC_G1,3));



	sensorAvgBuff.sumAvgBTPT = sensorAvgBuff.sumAvgBTPT + pduSensorRaw.rawBTPT;
	sensorAvgBuff.sumAvgPVPT = sensorAvgBuff.sumAvgPVPT + pduSensorRaw.rawPVPT;
	sensorAvgBuff.sumAvgHVPT = sensorAvgBuff.sumAvgHVPT + pduSensorRaw.rawHVPT;
	sensorAvgBuff.sumAvgDCCT = sensorAvgBuff.sumAvgDCCT + pduSensorRaw.rawDCCT;

	if(pduSensorAvg.count == 31)
	{
		pduSensorAvg.sumAvgPVPT = sensorAvgBuff.sumAvgPVPT >> 5;
		pduSensorAvg.sumAvgHVPT = sensorAvgBuff.sumAvgHVPT >> 5;
		pduSensorAvg.sumAvgBTPT = sensorAvgBuff.sumAvgBTPT >> 5;
		pduSensorAvg.sumAvgDCCT = sensorAvgBuff.sumAvgDCCT >> 5;
		sensorAvgBuff.sumAvgBTPT = 0;
		sensorAvgBuff.sumAvgPVPT = 0;
		sensorAvgBuff.sumAvgHVPT = 0;
		sensorAvgBuff.sumAvgDCCT = 0;

		pduSensor.mDCCT = 0.0296*pduSensorAvg.sumAvgDCCT - 60.441;
		pduSensor.mBTPT = 0.0986*pduSensorAvg.sumAvgBTPT - 2.4981;
		pduSensor.mHVPT = 0.0977*pduSensorAvg.sumAvgHVPT + 0.413;
		pduSensor.mPVPT = 0.0977*pduSensorAvg.sumAvgPVPT + 0.4414;
		pduSensorAvg.count = 0;
	}
	else {pduSensorAvg.count++;}

	/* Do moving Average Filter */
//	MF_MovingAverage(&pduSensorAvg, &pduSensorRaw);
}

void one_msec_intrpt_handler(void)
{
	XMC_CCU4_SLICE_ClearEvent(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
	PDU_ADC_Start();
}

void hundred_msec_intrpt_handler(void)
{
	float soc_f;
	float ocv;

	ocv = genix_data.pack_voltage*0.1;

	/* piece-wise OCV-SoC transformation */
	if 		((ocv > 182.25)  && (ocv <= 201.75)) {soc_f = 0.1425*ocv - 25.962;}
	else if ((ocv > 201.75)  && (ocv <= 235.125)){soc_f = 0.3167*ocv - 61.374;}
	else if ((ocv > 235.125) && (ocv <= 245.25)){soc_f = 2.4691*ocv - 566.67;}
	else if ((ocv > 245.25) && (ocv <= 250.65)){soc_f = 9.2593*ocv - 2231.9;}
	else if ((ocv > 250.65) && (ocv <= 250.875)){soc_f = 37.037*ocv - 9194.4;}
	else if (ocv  > 250.875){soc_f = 100.0;}
	else if (ocv <= 182.25){soc_f = 0.0;}
	/* send PCS status */
	bmsSensor.vbat 		= pduSensor.mBTPT*10;
	bmsSensor.ibat 		= pduSensor.mDCCT*10;
//	bmsSensor.soc		= genix_data.pack_soc;
	bmsSensor.soc		= soc_f*10;
	bmsSensor.tempbat 	= genix_data.pack_max_temp;

	/* Obtain data from Genix Battery */
	GENIX_RequestData(GENIX_SUMMARY);
	PCS_BMUTransmitToPCS(0x100, &pcsBmsStatus, &bmsSensor);
	PCS_BMUTransmitToPCS(0x101, &pcsBmsStatus, &bmsSensor);
	XMC_CCU4_SLICE_ClearEvent(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
}

void five_hundreds_intrpt_handler(void)
{
	static uint8_t counting=0;
	if(counting == 1)
	{
		if(sysTickParamHandle.enable)
		{
//			XMC_GPIO_ToggleOutput(P2_13);
			if(sysTickParamHandle.count == 0)
			{
				sysTickParamHandle.enable = false;
				sysTickParamHandle.isTimeOut = true;
			}
			else
			{
				sysTickParamHandle.count--;
			}
		}
		counting = 0;
	}
	else
	{
		counting++;
	}
	buffptr = ESP32_Map_Data(&data_group);
	memcpy(espBuffer, buffptr, ESP_DATA_LENGTH);
	ESP32_SendStream((char*)&espBuffer, ESP_DATA_LENGTH);

	XMC_CCU4_SLICE_ClearEvent(CCU40_CC42, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
}
