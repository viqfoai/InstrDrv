#include <Windows.h>
#include <stdio.h>
#include <visa.h>
#include <BatCycleController.h>
#include "InstrDrv.h"
#include "TestExecute.h"
#include <BasicFunction.h>

char szAgile6060ELoad[] = "6060";
char szKeythley2400[] = "2400";
char szE3634APowerSupply[] = "E3634";
char szKeysight34401MultiMeter[] = "34401";
char szKeysight34465MultiMeter[] = "34465";
char szIT6332BPowerSupply[] = "IT6332B";
char szIT8811ELoad[] = "IT8811";

extern CRITICAL_SECTION criticalSection;
ViSession gViAmpMeter, gViVoltMeter;

BOOL DllSetCharger(ViSession viCharger, TEST_STEP * pTestStep, LPSTR szInstrIDStr)
{

	ViStatus viStatus = VI_SUCCESS;
	DWORD retCount;
	char szTempStr[MAX_STRING_LENGTH] = { 0 };

	if (NULL != strstr(szInstrIDStr, szE3634APowerSupply))
	{
		viStatus = viWrite(viCharger, (ViConstBuf)"VOLT:PROT:CLE\n", strlen("VOLT:PROT:CLE\n"), &retCount);
		viStatus = viWrite(viCharger, (ViConstBuf)"CURR:PROT:CLE\n", strlen("CURR:PROT:CLE\n"), &retCount);
		sprintf(szTempStr, "VOLT %f\n", pTestStep->iVoltage / 1000.0);
		viStatus = viWrite(viCharger, (ViConstBuf)szTempStr, strlen(szTempStr), &retCount);
		sprintf(szTempStr, "CURR %f\n", pTestStep->dblCurrent / 1000.0);
		viStatus = viWrite(viCharger, (ViConstBuf)szTempStr, strlen(szTempStr), &retCount);
		viStatus = viWrite(viCharger, (ViConstBuf)"OUTP ON\n", strlen("OUTP ON\n"), &retCount);
		/*
		viStatus = viPrintf(viCharger, "VOLT:PROT:CLE\n");
		viStatus = viPrintf(viCharger, "CURR:PROT:CLE\n");
		viStatus = viPrintf(viCharger, "VOLT %f\n", pTestStep->iVoltage / 1000.0);
		viStatus = viPrintf(viCharger, "CURR %f\n", pTestStep->iCurrent / 1000.0);
		viStatus = viPrintf(viCharger, "OUTP ON\n");
		*/
		return TRUE;
	}

	if (NULL != strstr(szInstrIDStr, szIT6332BPowerSupply))
	{
		viStatus = viWrite(viCharger, (ViConstBuf)"VOLT:PROT:CLE\n", strlen("VOLT:PROT:CLE\n"), &retCount);
		viStatus = viWrite(viCharger, (ViConstBuf)"CURR:PRTO:CLE\n", strlen("CURR:PROT:CLE\n"), &retCount);
		sprintf(szTempStr, "VOLT %f\n", pTestStep->iVoltage / 1000.0);
		viStatus = viWrite(viCharger, (ViConstBuf)szTempStr, strlen(szTempStr), &retCount);
		sprintf(szTempStr, "CURR %f\n", pTestStep->dblCurrent / 1000.0);
		viStatus = viWrite(viCharger, (ViConstBuf)szTempStr, strlen(szTempStr), &retCount);
		viStatus = viWrite(viCharger, (ViConstBuf)"OUTP ON\n", strlen("OUTP ON\n"), &retCount);
		/*
		viStatus = viPrintf(viCharger, "VOLT:PROT:CLE\n");
		viStatus = viPrintf(viCharger, "CURR:PROT:CLE\n");
		viStatus = viPrintf(viCharger, "VOLT %f\n", pTestStep->iVoltage / 1000.0);
		viStatus = viPrintf(viCharger, "CURR %f\n", pTestStep->iCurrent / 1000.0);
		viStatus = viPrintf(viCharger, "OUTP ON\n");
		*/
		return TRUE;
	}

	if (NULL != strstr(szInstrIDStr, szKeythley2400))
	{
		if (pTestStep->iVoltage > KEITHLEY_MAX_PLUS_VOLT || pTestStep->iVoltage < KEITHLEY_MIN_MINUS_VOLT)
			return FALSE;

		viStatus = viPrintf(viCharger, "SOUR:FUNC CURR\n");
		viStatus = viPrintf(viCharger, "SOUR:CURR %lf\n", pTestStep->dblCurrent / 1000.0);
		viStatus = viPrintf(viCharger, "OUTPUT ON\n");

		return TRUE;
	}

	return FALSE;
}

BOOL DllShutdownCharger(ViSession viCharger, LPSTR szInstrIDStr)
{
	ViStatus viStatus = VI_SUCCESS;
	/*
	if (NULL != strstr(szInstrIDStr, szE3634APowerSupply))
	{
		viStatus = viPrintf(viCharger, "OUTP OFF\n");
		viStatus = viPrintf(viCharger, "VOLT %f\n", 0);
		viStatus = viPrintf(viCharger, "CURR %f\n", 0);
		return TRUE;
	}*/
	viStatus = viPrintf(viCharger, "*RST\n");
	viStatus = viPrintf(viCharger, "*RST\n");

	return TRUE;
}

BOOL DllShutdownLoad(ViSession viLoad, LPSTR szInstrIDStr)
{
	ViStatus viStatus = VI_SUCCESS;
	DWORD dwRetCount;
	/*
	if (NULL != strstr(szInstrIDStr, szAgile6060ELoad))
	{
		viStatus = viPrintf(viLoad, "OUTP OFF\n");
		viStatus = viPrintf(viLoad, "VOLT %f\n", 6);
		viStatus = viPrintf(viLoad, "CURR %f\n", 0);
		viStatus = viPrintf(viLoad, "RES %f\n", 1000);
		return TRUE;
	}
	*/
	
	viStatus = viWrite(viLoad, (ViConstBuf)"*RST\n", sizeof("*RST\n"), &dwRetCount);
	
	return TRUE;
}

BOOL DllUpdateVoltage(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, char *szVoltage, LPSTR szInstrIDStr)
{
	float fVoltage = 0, fDummy;
	ViStatus viStatus;

	if (VI_NULL == viVoltMeter)
		return FALSE;

	if (bAllInstrumentsOpened)
	{
		if (NULL != strstr(szInstrIDStr, szKeythley2400))
		{
			viStatus = viScanf(viVoltMeter, "%f, %f, %f, %f, %f\n", &fVoltage, &fDummy, &fDummy, &fDummy, &fDummy);

			if (VI_SUCCESS != viStatus)
				viScanf(viVoltMeter, "%f, %f, %f, %f, %f\n", &fVoltage, &fDummy, &fDummy, &fDummy, &fDummy);

			viPrintf(viVoltMeter, "MEAS:VOLT:DC?\n");
			sprintf(szVoltage, "%4.0f", fVoltage * 1000);
			return TRUE;
		}

		if (strstr(szInstrIDStr, szKeysight34401MultiMeter) ||\
			strstr(szInstrIDStr, szKeysight34465MultiMeter))
		{
			char szTempStr[MAX_STRING_LENGTH];
			DWORD dwRetCount;

			//viStatus = viPrintf(viVoltMeter, "FETCH?\n");
			viStatus = viRead(viVoltMeter, (ViPBuf)szTempStr, MAX_STRING_LENGTH, &dwRetCount);
			sscanf(szTempStr, "%f\n", &fVoltage);
			/*
			if (VI_SUCCESS != viStatus)
			viStatus = viScanf(viVoltMeter, "%f\n", &fVoltage);
			*/

//			viPrintf(viVoltMeter, "MEAS:VOLT:DC?\n");
			sprintf(szVoltage, "%4.0f", fVoltage * 1000);

			return TRUE;
		}

		return FALSE;
	}
	else
	{
		szVoltage = "";
		return FALSE;
	}
}

BOOL DllUpdateCurrent(BOOL bAllInstrumentsOpened, ViSession viCurrMeter, char *szCurrent, LPSTR szInstrIDStr)
{
	float fCurrent = 0, fDummy;
	ViStatus viStatus;

	if (VI_NULL == viCurrMeter)
		return FALSE;

	if (bAllInstrumentsOpened)
	{
		if (NULL != strstr(szInstrIDStr, szKeythley2400))
		{
			viStatus = viScanf(viCurrMeter, "%f, %f, %f, %f, %f\n", &fDummy, &fCurrent, &fDummy, &fDummy, &fDummy);

			if (VI_SUCCESS != viStatus)
				viScanf(viCurrMeter, "%f, %f, %f, %f, %f\n", &fDummy, &fCurrent, &fDummy, &fDummy, &fDummy);

			viPrintf(viCurrMeter, "MEAS:VOLT:DC?\n");
			sprintf(szCurrent, "%4.0f", fCurrent * 1000);
			return TRUE;
		}

		if (strstr(szInstrIDStr, szKeysight34401MultiMeter) ||\
			strstr(szInstrIDStr, szKeysight34465MultiMeter))
		{
			char szTempString[MAX_STRING_LENGTH];
			DWORD dwRetCount;

			viStatus = viRead(viCurrMeter, (ViPBuf)szTempString, MAX_STRING_LENGTH, &dwRetCount);
			sscanf(szTempString, "%f\n", &fCurrent);
			//viStatus = viScanf(viCurrMeter, "%f\n", &fCurrent);

			//if (VI_SUCCESS != viStatus)
			//	viStatus = viScanf(viCurrMeter, "%f\n", &fCurrent);

			//viPrintf(viCurrMeter, "MEAS:CURR:DC? MAX,0.00001\n");
			sprintf(szCurrent, "%4.0f", fCurrent * 1000);
			return TRUE;
		}

		return FALSE;
	}
	else
	{
		szCurrent = "";
		return FALSE;
	}
}

BOOL DllSetLoad(ViSession viLoad, TEST_STEP *pTestStep, LPSTR szInstrIDStr)
{
	ViStatus viStatus = VI_SUCCESS;

	if (NULL != strstr(szInstrIDStr, szAgile6060ELoad))
	{
		viStatus = viPrintf(viLoad, "CURR %lf\n", pTestStep->dblCurrent / 1000.0);
		viStatus = viPrintf(viLoad, "INPUT ON\n");

		return TRUE;
	}


	if (NULL != strstr(szInstrIDStr, szKeythley2400))
	{
		if (pTestStep->dblCurrent > KEITHLEY_MAX_PLUS_CURR || pTestStep->dblCurrent < KEITHLEY_MIN_MINUS_CURR)
			return FALSE;

//		viStatus = viPrintf(viLoad, "*RST\n");
		viStatus = viPrintf(viLoad, "SOUR:FUNC CURR\n");
		viStatus = viPrintf(viLoad, "SOUR:CURR %f\n", -pTestStep->dblCurrent / 1000.0);
		viStatus = viPrintf(viLoad, "OUTPUT ON\n");

		return TRUE;
	}

	if (NULL != strstr(szInstrIDStr, szIT8811ELoad))
	{
		if (pTestStep->dblCurrent > ITECH_MAX_PLUS_CURR || pTestStep->dblCurrent < ITECH_MIN_MINUS_CURR)
			return FALSE;

		//		viStatus = viPrintf(viLoad, "*RST\n");
		viStatus = viPrintf(viLoad, "SOUR:FUNC CURR\n");
		viStatus = viPrintf(viLoad, "SOUR:CURR %f\n", pTestStep->dblCurrent / 1000.0);
		viStatus = viPrintf(viLoad, "INP ON\n");

		return TRUE;
	}
	return FALSE;
}

HRESULT DllInitVoltMeter(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr)
{
	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		if (!pRecItem->bHighFrequencyDataNeeded)
		{
			DWORD dwRetCount;
			viWrite(viVoltMeter, (ViConstBuf)"CONF:VOLT:DC\n", strlen("CONF:VOLT:DC\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"TRIG:SOUR BUS\n", strlen("TRIG:SOUR BUS\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"TRIG:DEL 0\n", strlen("TRIG:DEL 0\n"), &dwRetCount);				//触发延时设为0
			viWrite(viVoltMeter, (ViConstBuf)"TRIG:COUNT 1\n", strlen("TRIG:COUNT 1\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"VOLT:NPLC 10\n", strlen("VOLT:NPLC 10\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"SAMP:COUNT 1\n", strlen("SAMP:COUNT 1\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"INITIATE\n", strlen("INITIATE\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"*TRG\n", strlen("*TRG\n"), &dwRetCount);
			viWrite(viVoltMeter, (ViConstBuf)"FETCH?\n", strlen("FETCH?\n"), &dwRetCount);
			/*
			viPrintf(viVoltMeter, "CONF:VOLT:DC\n");
			viPrintf(viVoltMeter, "TRIG:SOUR BUS\n");
			viPrintf(viVoltMeter, "TRIG:DEL 0\n");				//触发延时设为0
			viPrintf(viVoltMeter, "TRIG:COUNT 1\n");
			viPrintf(viVoltMeter, "VOLT:NPLC 10\n");
			viPrintf(viVoltMeter, "SAMP:COUNT 1\n");
			viPrintf(viVoltMeter, "INITIATE\n");
			viPrintf(viVoltMeter, "*TRG\n");
			viPrintf(viVoltMeter, "FETCH?\n");
			*/
		}
	}
	return S_OK;
}

HRESULT DllEnableMeterInt(ViSession viMeter, LPSTR szInstrIDStr)
{
	ViStatus viStatus;
	DWORD dwRetCount;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) ||\
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		viStatus = viWrite(viMeter, (ViConstBuf)"*CLS\n", strlen("*CLS\n"), &dwRetCount);
		viStatus = viWrite(viMeter, (ViConstBuf)"*ESE 1\n", strlen("*ESE 1\n"), &dwRetCount);
		viStatus = viWrite(viMeter, (ViConstBuf)"*SRE 48\n", strlen("*SRE 48\n"), &dwRetCount);
	}
	return S_OK;
}

HRESULT DllConfigVoltMeter(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr)
{
	ViStatus viStatus;
	DWORD dwRetCount;
	char szTempString[MAX_STRING_LENGTH];

	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		if (!pRecItem->uiHighFrequenceyReadCounter)
		{
			pRecItem->dwVoltReadingCount = 1;
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"DISP ON\n", sizeof("DISP ON\n"), &dwRetCount);
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"VOLT:NPLC 10\n", strlen("VOLT:NPLC 10\n"), &dwRetCount);
			viStatus = sprintf(szTempString, "SAMP:COUNT %d\n", (int)(pRecItem->dwVoltReadingCount));
			viStatus = viWrite(viVoltMeter, (ViConstBuf)szTempString, strlen(szTempString), &dwRetCount);
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"ZERO:AUTO ON\n", strlen("ZERO:AUTO ON\n"), &dwRetCount);
		}
		else
		{
			pRecItem->dwVoltReadingCount = 1000;
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"DISP OFF\n", sizeof("DISP OFF\n"), &dwRetCount);
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"VOLT:NPLC 0.02\n", strlen("VOLT:NPLC 0.02\n"), &dwRetCount);
			sprintf(szTempString, "SAMP:COUNT %d\n", (int)(pRecItem->dwVoltReadingCount));
			viStatus = viWrite(viVoltMeter, (ViConstBuf)szTempString, strlen(szTempString), &dwRetCount);
			viStatus = viWrite(viVoltMeter, (ViConstBuf)"ZERO:AUTO ONCE\n", strlen("ZERO:AUTO ONCE\n"), &dwRetCount);
		}
	}

	return S_OK;
}

HRESULT DllInitAmpMeter(BOOL bAllInstrumentsOpened, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr)
{
	ViStatus viStatus;
	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		if (!pRecItem->bHighFrequencyDataNeeded)
		{
			viStatus = viPrintf(viAmpMeter, "CONF:CURR:DC MAX,0.00001\n");
			viStatus = viPrintf(viAmpMeter, "TRIG:SOUR BUS\n");
			viStatus = viPrintf(viAmpMeter, "TRIG:DEL 0\n");				//触发延时设为0
			viStatus = viPrintf(viAmpMeter, "TRIG:COUNT 1\n");
			viStatus = viPrintf(viAmpMeter, "CURR:NPLC 10\n");
			viStatus = viPrintf(viAmpMeter, "SAMP:COUNT 1\n");
			viStatus = viPrintf(viAmpMeter, "INITIATE\n");
			viStatus = viPrintf(viAmpMeter, "*TRG\n");
			viStatus = viPrintf(viAmpMeter, "FETCH?\n");
		}
	}
	return S_OK;
}

HRESULT DllConfigAmpMeter(BOOL bAllInstrumentsOpened, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr)
{
	ViStatus viStatus;
	DWORD dwRetCount;
	char szTempString[MAX_STRING_LENGTH]; 

	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		if (!pRecItem->uiHighFrequenceyReadCounter)
		{
			pRecItem->dwCurrReadingCount = 1;
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"DISP ON\n", sizeof("DISP ON\n"), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "FUNC:CURR:DC MAX, 0.00001\n", strlen("FUNC:CURR:DC MAX, 0.00001\n"), &dwRetCount);
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"TRIG:SOUR IMM\n", strlen("TRIG:SOUR IMM\n"), &dwRetCount);
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"TRIG:DEL 0\n", strlen("TRIG:DEL 0\n"), &dwRetCount);
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"TRIG:COUNT 1\n", strlen("TRIG:COUNT 1\n"), &dwRetCount);
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"CURR:NPLC 10\n", strlen("CURR:NPLC 10\n"), &dwRetCount);
			sprintf(szTempString, "SAMP:COUNT %d\n", (int)(pRecItem->dwCurrReadingCount));
			viStatus = viWrite(viAmpMeter, (ViConstBuf)szTempString, strlen(szTempString), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "READ?\n", sizeof("READ?\n"), &dwRetCount);
			/*
			viStatus = viWrite(viVoltMeter, "DISP ON\n", sizeof("DISP ON\n"), &dwRetCount);
			viStatus = viWrite(viVoltMeter, "VOLT:NPLC 10\n", strlen("VOLT:NPLC 10\n"), &dwRetCount);
			viStatus = sprintf(szTempString, "SAMP:COUNT %d\n", pRecItem->dwVoltReadingCount);
			viStatus = viWrite(viVoltMeter, szTempString, strlen(szTempString), &dwRetCount);
			viStatus = viWrite(viVoltMeter, "ZERO:AUTO ON\n", strlen("ZERO:AUTO ON\n"), &dwRetCount);
			*/
		}
		else
		{
			//pRecItem->dwCurrReadingCount = 1000;
			//viStatus = viWrite(viAmpMeter, "DISP OFF\n", sizeof("DISP OFF\n"), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "FUNC:CURR:DC MAX,0.00001\n", strlen("FUNC:CURR:DC MAX,0.00001\n"), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "TRIG:SOUR IMM\n", strlen("TRIG:SOUR IMM\n"), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "TRIG:DEL 0\n", strlen("TRIG:DEL 0\n"), &dwRetCount);				//触发延时设为0
			//viStatus = viWrite(viAmpMeter, "TRIG:COUNT 1\n", strlen("TRIG:COUNT 1\n"), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "CURR:NPLC 0.02\n", strlen("CURR:NPLC 0.02\n"), &dwRetCount);
			//viStatus = sprintf(szTempString, "SAMP:COUNT %d\n", pRecItem->dwCurrReadingCount);
			//viStatus = viWrite(viAmpMeter, szTempString, strlen(szTempString), &dwRetCount);
			//viStatus = viWrite(viAmpMeter, "READ?\n", sizeof("READ?\n"), &dwRetCount);
		}
	}
	return S_OK;
}

HRESULT DllInitiateAndTriggerMeter(BOOL bAllInstrumentsOpened, ViSession viMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr)
{
	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if (strstr(szInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szInstrIDStr, szKeysight34465MultiMeter))
	{
		viPrintf(viMeter, "INITIATE\n");
		viPrintf(viMeter, "*TRG\n");
		viPrintf(viMeter, "FETCH?\n");
	}
	return S_OK;
}

HRESULT DllInitiateAndTriggerVAMeters(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szVoltInstrIDStr, LPSTR szAmpInstrIDStr)
{
	ViStatus viStatus;
	DWORD dwRetCount;

	if (!bAllInstrumentsOpened)
		return S_FALSE;

	if ((strstr(szVoltInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szVoltInstrIDStr, szKeysight34465MultiMeter)) && \
		(strstr(szAmpInstrIDStr, szKeysight34401MultiMeter) || \
		strstr(szAmpInstrIDStr, szKeysight34465MultiMeter)))
	{
		//ViJobId JobIdV, JobIdI;
		//EnterCriticalSection(&(pRecItem->criticSection));

		//viStatus = viWrite(viVoltMeter, "INITIATE\n", strlen("INITIATE\n"), &dwRetCount);
		//viStatus = viWrite(viAmpMeter, "INITIATE\n", strlen("INITIATE\n"), &dwRetCount);
		//viStatus = viWrite(viVoltMeter, "*TRG\n", strlen("*TRG\n"), &dwRetCount);
		//viStatus = viWrite(viAmpMeter, "*TRG\n", strlen("*TRG\n"), &dwRetCount);
		//viStatus = viReadAsync(viAmpMeter, pRecItem->CurrReading, ELEMENT_LENGTH * MAX_CNT, &JobId);
		//viStatus = viReadAsync(viVoltMeter, pRecItem->VoltReading, ELEMENT_LENGTH * MAX_CNT, &JobId);
		viStatus = viWrite(viVoltMeter, (ViConstBuf)"READ?\n", strlen("READ?\n"), &dwRetCount);
		viStatus = viReadAsync(viVoltMeter, (ViPBuf)(pRecItem->VoltReading), ELEMENT_LENGTH * pRecItem->dwVoltReadingCount, &(pRecItem->JobIdV));

		if (!pRecItem->uiHighFrequenceyReadCounter)
		{
			viStatus = viWrite(viAmpMeter, (ViConstBuf)"READ?\n", strlen("READ?\n"), &dwRetCount);
			viStatus = viReadAsync(viAmpMeter, (ViPBuf)(pRecItem->CurrReading), ELEMENT_LENGTH * pRecItem->dwCurrReadingCount, &(pRecItem->JobIdI));
		}
		else
		{
			if (HIGH_FREQ_READ_COUNT != pRecItem->uiHighFrequenceyReadCounter)
				strcpy(pRecItem->CurrReading, "0\n");
		}
		DllGetTimer(pRecItem->bLogStarted, &(pRecItem->uiTempLogTimerMilliSec));
		//gViAmpMeter = viAmpMeter;
		//gViVoltMeter = viVoltMeter;
	}
	return S_OK;
}

BOOL WINAPI DllReadVoltage(LPARAM lParam)
{
	INST_HANDLE_SET *pInstHandleSet;
	ViJobId jobID;
	ViEventType etype;
	ViEvent eventContext;
	ViStatus viStatus;

	pInstHandleSet = (INST_HANDLE_SET *)lParam;
	while (1)
	{
		WaitForSingleObject(pInstHandleSet->hEventStartVoltReading, INFINITE);
		viStatus = viReadAsync(pInstHandleSet->viVoltMeter, (ViPBuf)(pInstHandleSet->pRecordItem->VoltReading), pInstHandleSet->pRecordItem->dwVoltReadingCount * ELEMENT_LENGTH, &jobID);

		viStatus = viWaitOnEvent(pInstHandleSet->viVoltMeter, VI_EVENT_IO_COMPLETION, VI_INFINITE, &etype, &eventContext);
		SetEvent(pInstHandleSet->hEventVoltReadDone);
	}	
}

BOOL WINAPI DllReadCurrent(LPARAM lParam)
{
	INST_HANDLE_SET *pInstHandleSet;
	ViJobId jobID;
	ViEventType etype;
	ViEvent eventContext;
	ViStatus viStatus;

	pInstHandleSet = (INST_HANDLE_SET *)lParam;

	while (1)
	{
		WaitForSingleObject(pInstHandleSet->hEventStartCurrReading, INFINITE);
		viStatus = viReadAsync(pInstHandleSet->viCurrMeter, (ViPBuf)(pInstHandleSet->pRecordItem->CurrReading), pInstHandleSet->pRecordItem->dwCurrReadingCount * ELEMENT_LENGTH, &jobID);

		viStatus = viWaitOnEvent(pInstHandleSet->viCurrMeter, VI_EVENT_IO_COMPLETION, VI_INFINITE, &etype, &eventContext);
		WaitForSingleObject(pInstHandleSet->hEventVoltReadDone, INFINITE);
		SetEvent(pInstHandleSet->hEventDMMReadDone);
	}
}

ViStatus _VI_FUNCH DllReadVoltageCallback(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle)
{
	ViJobId jobID;
	ViStatus status;
	ViUInt16 stb;
	REC_ITEM *pRecItem;

	pRecItem = userHandle;
	status = viReadSTB(vi, &stb);
	status = viReadAsync(vi, (ViBuf)(pRecItem->VoltReading), pRecItem->dwVoltReadingCount * ELEMENT_LENGTH, &jobID);
	status = viEnableEvent(vi, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);
//	status = viEnableEvent(vi, VI_EVENT_SERVICE_REQ, VI_HNDLR, VI_NULL);
	return VI_SUCCESS;
}

ViStatus _VI_FUNCH DllReadVoltageCompletion(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle)
{
	ViStatus status;
	ViUInt16 stb;
	status = viReadSTB(vi, &stb);
	status = viEnableEvent(gViAmpMeter, VI_EVENT_SERVICE_REQ, VI_HNDLR, VI_NULL);
	//status = viEnableEvent(gViAmpMeter, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);
	//SetEvent(((REC_ITEM *)userHandle)->hEventVoltReadDone);
	return VI_SUCCESS;
}

ViStatus _VI_FUNCH DllReadCurrentCallback(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle)
{
	ViJobId jobID;
	ViStatus status;
	ViUInt16 stb;
	REC_ITEM *pRecItem;

	pRecItem = userHandle;
	status = viReadSTB(vi, &stb);
	status = viReadAsync(vi, (ViBuf)(pRecItem->CurrReading), pRecItem->dwCurrReadingCount * ELEMENT_LENGTH, &jobID);
	status = viEnableEvent(vi, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);
	return VI_SUCCESS;
}

ViStatus _VI_FUNCH DllReadCurrentCompletion(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle)
{
	ViStatus status;
	ViUInt16 stb;
	status = viReadSTB(vi, &stb);
	//PostMessage(((REC_ITEM *)userHandle)->hWnd, WM_DMM_DATA_READY, 0, 0);
	SetEvent(((INST_HANDLE_SET *)userHandle)->hEventDMMReadDone);
	return VI_SUCCESS;
}

void DllGetTimer(BOOL bTimerStarted, UINT *pTimer)
{
	UINT Timer = *pTimer;
	static UINT uiStartTime;

	if (!bTimerStarted)
	{
		Timer = 0;
		uiStartTime = GetTickCount();
	}
	else
	{
		Timer = GetTickCount() - uiStartTime;
	}
	*pTimer = Timer;
	return;
}

ViStatus _VI_FUNCH DllSuspendEventHandler(ViSession vi)
{
	ViStatus viStatus;
	viStatus = viEnableEvent(vi, VI_EVENT_SERVICE_REQ, VI_SUSPEND_HNDLR, VI_NULL);
	viStatus = viEnableEvent(vi, VI_EVENT_IO_COMPLETION, VI_SUSPEND_HNDLR, VI_NULL);

	return viStatus;
}

ViStatus _VI_FUNCH DllResumeEventHandler(ViSession vi)
{
	ViStatus viStatus;
	viStatus = viEnableEvent(vi, VI_EVENT_SERVICE_REQ, VI_HNDLR, VI_NULL);
	viStatus = viEnableEvent(vi, VI_EVENT_IO_COMPLETION, VI_HNDLR, VI_NULL);

	return viStatus;
}
