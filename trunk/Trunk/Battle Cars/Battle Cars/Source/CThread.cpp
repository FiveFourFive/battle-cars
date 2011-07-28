#include "CThread.h" 
#include <process.h> 

CThreadSystem::CThreadSystem() 
{} 
CThreadSystem* CThreadSystem::GetInstance() 
{ 
static CThreadSystem cedric; 
return &cedric; 
} 
void CThreadSystem::InitSyncSystem() 
{ 
InitializeCriticalSection(&SYNC_OBJ); 
EnterCriticalSection(&SYNC_OBJ); 
m_vSyncObj.clear(); 
LeaveCriticalSection(&SYNC_OBJ); 
} 
int CThreadSystem::CreateLockObject() 
{ 
EnterCriticalSection(&SYNC_OBJ); 
tSyncObj syncobj; 
InitializeCriticalSection(&syncobj.mutex); 
syncobj.change = false; 
m_vSyncObj.push_back(syncobj); 
int ret = m_vSyncObj.size()-1; 
LeaveCriticalSection(&SYNC_OBJ); 
return ret; 
} 
#if WINDOWSXP 
int CThreadSystem::CreateChangeObject() 
{ 
EnterCriticalSection(&SYNC_OBJ); 
tSyncObj syncobj; 
InitializeCriticalSection(&syncobj.mutex); 
syncobj.var = CreateEvent(NULL,false,false,NULL); 
syncobj.change = true; 
m_vSyncObj.push_back(syncobj); 
int ret = m_vSyncObj.size()-1; 
LeaveCriticalSection(&SYNC_OBJ); 
return ret; 
} 
void CThreadSystem::WaitChange(int ObjectIndex) 
{ 
UnLockIt(ObjectIndex); 
WaitForSingleObject(m_vSyncObj[ObjectIndex].var,INFINITE); 
LockIt(ObjectIndex); 
} 
void CThreadSystem::WaitChangeTimed(int ObjectIndex, int TimeToWait) 
{ 
WaitForSingleObject(m_vSyncObj[ObjectIndex].var,TimeToWait); 
} 
void CThreadSystem::SendChangeWakeup(int ObjectIndex) 
{ 
SetEvent(m_vSyncObj[ObjectIndex].var); 
} 
void CThreadSystem::BroadcastChangeWakeup(int ObjectIndex) 
{ 
HANDLE tarvar = m_vSyncObj[ObjectIndex].var; 
for(int i =0;i<(int)m_vSyncObj.size();i++) 
if(m_vSyncObj[i].var == tarvar) 
SetEvent(m_vSyncObj[i].var); 
} 
#else 
int CThreadSystem::CreateChangeObject() 
{ 
EnterCriticalSection(&SYNC_OBJ); 
tSyncObj syncobj; 
InitializeCriticalSection(&syncobj.mutex); 
InitializeConditionVariable(&syncobj.var); 
syncobj.change = true; 
m_vSyncObj.push_back(syncobj); 
int ret = m_vSyncObj.size()-1; 
LeaveCriticalSection(&SYNC_OBJ); 
return ret; 
} 
void CThreadSystem::WaitChange(int ObjectIndex) 
{ 
SleepConditionVariableCS(&m_vSyncObj[ObjectIndex].var,&m_vSyncObj[ObjectIndex].mutex,INFINITE); 
} 
void CThreadSystem::WaitChangeTimed(int ObjectIndex, int TimeToWait) 
{ 
SleepConditionVariableCS(&m_vSyncObj[ObjectIndex].var,&m_vSyncObj[ObjectIndex].mutex,TimeToWait); 
} 
void CThreadSystem::SendChangeWakeup(int ObjectIndex) 
{ 
WakeConditionVariable(&m_vSyncObj[ObjectIndex].var); 
} 
void CThreadSystem::BroadcastChangeWakeup(int ObjectIndex) 
{ 
WakeAllConditionVariable(&m_vSyncObj[ObjectIndex].var); 
} 
#endif 
void CThreadSystem::LockIt(int ObjectIndex) 
{ 
EnterCriticalSection(&m_vSyncObj[ObjectIndex].mutex); 
} 
void CThreadSystem::UnLockIt(int ObjectIndex) 
{ 
LeaveCriticalSection(&m_vSyncObj[ObjectIndex].mutex); 
} 
void *CThreadSystem::StartThread(pfthread_t threadFunc, void *arg) 
{ 
unsigned int ret; 
HANDLE handle; 
handle = (HANDLE)_beginthreadex(NULL,NULL,threadFunc,arg,0,&ret); 
return (void*)handle; 
} 
void *CThreadSystem::LeaveThread(void *rtn_value) 
{ 
_endthreadex((unsigned int)rtn_value); 
return rtn_value; 
} 
void CThreadSystem::CloseSyncSystem() 
{ 
EnterCriticalSection(&SYNC_OBJ); 
for(int i =0;i<(int)m_vSyncObj.size();i++) 
{ 
DeleteCriticalSection(&m_vSyncObj[i].mutex); 
} 
m_vSyncObj.clear(); 
LeaveCriticalSection(&SYNC_OBJ); 
DeleteCriticalSection(&SYNC_OBJ); 
} 
void CThreadSystem::FreeThread(void **SelectedThread) 
{ 
CloseHandle(*SelectedThread); 
*SelectedThread = NULL; 
} 
void * CThreadSystem::GetMyThreadID(void) 
{ 
return (void *)GetCurrentThreadId(); 
} 

