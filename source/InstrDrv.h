#pragma once


#define KEITHLEY_MAX_PLUS_VOLT 21000
#define KEITHLEY_MIN_MINUS_VOLT -21000
#define KEITHLEY_MAX_PLUS_CURR 1000
#define KEITHLEY_MIN_MINUS_CURR -1000
#define ITECH_MAX_PLUS_CURR 6000
#define ITECH_MIN_MINUS_CURR -6000

__declspec(dllexport) BOOL DllSetCharger(ViSession viCharger, TEST_STEP *pTestStep, LPSTR szInstrIDStr);
__declspec(dllexport) BOOL DllShutdownCharger(ViSession viCharger, LPSTR szInstrIDStr);
__declspec(dllexport) BOOL DllShutdownLoad(ViSession viLoad, LPSTR szInstrIDStr);
__declspec(dllexport) BOOL DllSetLoad(ViSession viLoad, TEST_STEP *pTestStep, LPSTR szInstrIDStr);
__declspec(dllexport) BOOL DllUpdateVoltage(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, char *szVoltage, LPSTR szInstrIDStr);
__declspec(dllexport) BOOL DllUpdateCurrent(BOOL bAllInstrumentsOpened, ViSession viCurrMeter, char *szCurrent, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllInitVoltMeter(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllInitAmpMeter(BOOL bAllInstrumentsOpened, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllConfigVoltMeter(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllConfigAmpMeter(BOOL bAllInstrumentsOpened, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllInitiateAndTriggerMeter(BOOL bAllInstrumentsOpened, ViSession viMeter, REC_ITEM *pRecItem, LPSTR szInstrIDStr);
__declspec(dllexport) HRESULT DllInitiateAndTriggerVAMeters(BOOL bAllInstrumentsOpened, ViSession viVoltMeter, ViSession viAmpMeter, REC_ITEM *pRecItem, LPSTR szVoltInstrIDStr, LPSTR szAmpInstrIDStr);
__declspec(dllexport) ViStatus _VI_FUNCH DllReadVoltageCallback(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle);
__declspec(dllexport) ViStatus _VI_FUNCH DllReadCurrentCallback(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle);
__declspec(dllexport) ViStatus _VI_FUNCH DllReadCurrentCompletion(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle);
__declspec(dllexport) ViStatus _VI_FUNCH DllReadVoltageCompletion(ViSession vi, ViEventType etype, ViEvent eventContext, ViAddr userHandle);
__declspec(dllexport) HRESULT DllEnableMeterInt(ViSession viMeter, LPSTR szInstrIDStr);
__declspec(dllexport) void DllGetTimer(BOOL bTimerStarted, UINT *pTimer);
__declspec(dllexport) ViStatus _VI_FUNCH DllSuspendEventHandler(ViSession vi);
__declspec(dllexport) ViStatus _VI_FUNCH DllResumeEventHandler(ViSession vi);
__declspec(dllexport) BOOL WINAPI DllReadVoltage(LPARAM lParam);
__declspec(dllexport) BOOL WINAPI DllReadCurrent(LPARAM lParam);
