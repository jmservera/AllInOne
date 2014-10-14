#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace
{
#pragma unmanaged
	UINT64 GetFreq()
	{
		UINT64 a;
		if(!QueryPerformanceFrequency((LARGE_INTEGER *)&a)) return 1000;

		return a;
	}

	UINT64 GetCounter()
	{
		UINT64 m_BaseTicks;

		if (!QueryPerformanceCounter((LARGE_INTEGER *)&m_BaseTicks))
			m_BaseTicks = (UINT64)timeGetTime();

		return m_BaseTicks;
	}

#pragma managed

	_Timer::_Timer()
	{
		m_TicksPerSecond=GetFreq();
		m_IsRunning = false;
		m_BaseTicks = 0;
		m_StopTicks = 0;
		m_LastTime = 0.0f;
		m_FrameTime = 0.0f;
		m_DeltaTime = 0.0f;
		m_FrameCount = 0;
		m_LastUpdate = 0.0f;
		m_Fps = 0.0f;
		m_FixedMode = false;
		m_FixedModeFps = 1/30.0f;
		m_FixedModeWait = false;
		m_LastTimeForFixedTime = 0.0f;
	}

	void _Timer::Start()
	{
		// Get the actual time to get in which moment the timer was set.
		// Test first the high resolution clock. If not, use timeGetTime.
		m_BaseTicks = GetCounter();

		m_IsRunning = TRUE;

		m_LastUpdate = 0.0f;
		m_FrameCount = 0;

		m_FrameTime = 0.0f;
		m_DeltaTime = 0.0f;
	}

	void _Timer::Stop()
	{
		if (m_IsRunning)
		{
			// Take the time from when the counter is stopped to later know
			// how long we have being stopped. Try first the high resolution
			// clock of the PC, if not use timeGetTime.
			m_StopTicks = GetCounter();

			m_IsRunning = FALSE;
		}
	}

	void _Timer::Continue()
	{
		if (!m_IsRunning)
		{
			UINT64 ticks = 0;

			// Get the actual time
			ticks = GetCounter();

			// Increment baseTicks to get how much time we have been stopped
			m_BaseTicks += ticks - m_StopTicks;

			m_IsRunning = TRUE;
		}
	}

	void _Timer::Reset()
	{
		// Get either the current time or the stop time

		if (m_StopTicks != 0)
		{
			m_BaseTicks = m_StopTicks;
		}
		else
		{
			m_BaseTicks = GetCounter();
		}

		m_StopTicks = 0;

		m_IsRunning = TRUE;

		m_LastUpdate = 0.0f;
		m_FrameCount = 0;

		m_FrameTime = 0.0f;
		m_DeltaTime = 0.0f;

		if (m_FixedMode)
		{
			m_LastTime = 0;
			m_FrameTime = 0;
			m_DeltaTime = m_FixedModeFps;
			m_LastTimeForFixedTime = 0;
		}
	}

	void _Timer::Update()
	{
    	// If the engine is working in fixedmode, it means
		// that the speed is established by a constant
    	
        if (m_FixedMode)
		{
            m_LastTime += m_DeltaTime;

		}
        else
            m_LastTime = CalculateTime();

        m_DeltaTime = m_LastTime - m_FrameTime;
        m_FrameTime += m_DeltaTime;

    	// Increment the number of frames
		m_FrameCount++;

    	// Calculate the framerate
		if (m_FrameTime - m_LastUpdate > FPS_INTERVAL)
		{
			m_Fps = (float)m_FrameCount / (m_FrameTime - m_LastUpdate);
			m_FrameCount = 0;
			m_LastUpdate = m_FrameTime;
		}
	}

	float _Timer::CalculateTime()
	{
		UINT64 ticks = 0;

		if (m_IsRunning)
		{
			if (!QueryPerformanceCounter((LARGE_INTEGER *)&ticks))
				ticks = (UINT64)timeGetTime();
		}
		else
		{
			ticks = m_StopTicks;
		}

		// Substract the time when we started to calculate how longer 
		// had the timer being working
		ticks -= m_BaseTicks;

		// Return the time in seconds
		return (float)ticks / (float)m_TicksPerSecond;
	}

	void _Timer::WaitForFixedTime()
	{
		if (m_LastTimeForFixedTime == 0)
			m_LastTimeForFixedTime = CalculateTime();

		while ((CalculateTime() - m_LastTimeForFixedTime) < m_FixedModeFps);

		m_LastTimeForFixedTime = CalculateTime();
	}
}
