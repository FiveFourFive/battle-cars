#ifndef _CTHREAD_H 
#define _CTHREAD_H 
#define WINDOWSXP TRUE 
//#include "stdafx.h" 
#include <Windows.h>
#include <WinBase.h>
#include <vector>
using namespace std;
typedef unsigned int (__stdcall *pfthread_t) (void *); 
#if WINDOWSXP 
struct tSyncObj 
{ 
bool change; 
CRITICAL_SECTION mutex; 
HANDLE var; 
}; 
#else 
struct tSyncObj 
{ 
bool change; 
CRITICAL_SECTION mutex; 
CONDITION_VARIABLE var; 
}; 
#endif 

class CThreadSystem 
{ 
CRITICAL_SECTION SYNC_OBJ; 
vector<tSyncObj> m_vSyncObj; 
CThreadSystem(); 
~CThreadSystem(){}; 
CThreadSystem(const CThreadSystem&); 
CThreadSystem& operator = (const CThreadSystem&); 
public: 
static CThreadSystem* GetInstance(); 
void InitSyncSystem(); 
int CreateLockObject(); 
int CreateChangeObject(); 
void LockIt(int ObjectIndex); 
void UnLockIt(int ObjectIndex); 
void WaitChange(int ObjectIndex); 
void WaitChangeTimed(int ObjectIndex, int TimeToWait); 
void SendChangeWakeup(int ObjectIndex); 
void BroadcastChangeWakeup(int ObjectIndex); 
void *StartThread(pfthread_t threadFunc, void *arg); 
void *LeaveThread(void *rtn_value); 
void CloseSyncSystem(); 
void FreeThread(void **SelectedThread); 
void * GetMyThreadID(void); 
}; 



#endif 
