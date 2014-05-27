#include <windows.h>
#include "HCoreTimer.h"

HCoreTimer::HCoreTimer(void)
{
	Restart();
}

float HCoreTimer::Start(void)
{
	m_eState = HCoreTIMER_RUNNING;
	QueryPerformanceCounter(&m_PerformanceCount);
	return m_fTime;
}

float HCoreTimer::Stop(void)
{
	m_eState = HCoreTIMER_STOPPED;

	LARGE_INTEGER pcount;
	QueryPerformanceCounter(&pcount);
	
	LARGE_INTEGER counter_frequency;
	QueryPerformanceFrequency(&counter_frequency);

	LARGE_INTEGER count_diff;
	count_diff.QuadPart = pcount.QuadPart - m_PerformanceCount.QuadPart;
	float time_diff = (float) count_diff.QuadPart / (float) counter_frequency.QuadPart;

	m_fTime += time_diff;

	return m_fTime;
}

void HCoreTimer::Restart(void)
{
	m_eState = HCoreTIMER_RUNNING;
	m_fTime = 0;
	QueryPerformanceCounter(&m_PerformanceCount);
}

float HCoreTimer::GetTime_MS(void)
{
	return GetTime_Second() * 1000.0f;
}

float HCoreTimer::GetTime_Second(void)
{
	if ( m_eState==HCoreTIMER_RUNNING )
	{
		LARGE_INTEGER pcount;
		QueryPerformanceCounter(&pcount);
		
		LARGE_INTEGER counter_frequency;
		QueryPerformanceFrequency(&counter_frequency);

		LARGE_INTEGER count_diff;
		count_diff.QuadPart = pcount.QuadPart - m_PerformanceCount.QuadPart;
		float time_diff = (float) count_diff.QuadPart / (float) counter_frequency.QuadPart;

		return time_diff;
	}
	else
	{
		return m_fTime;
	}
}
