#ifndef _HCoreTIMER_
#define _HCoreTIMER_

class HCoreTimer
{
public:
	enum TimerState
	{
		HCoreTIMER_RUNNING,
		HCoreTIMER_STOPPED,
	};
	
	float m_fTime;
	TimerState m_eState;
	LARGE_INTEGER m_PerformanceCount;

public:
	HCoreTimer(void);
	float Start(void);
	float Stop(void);
	void  Restart(void);
	
	float GetTime_MS(void);
	float GetTime_Second(void);
	float GetTime(void) { return GetTime_Second(); }
};

#endif // _HCoreTIMER_
