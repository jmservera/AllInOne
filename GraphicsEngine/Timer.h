#pragma once

namespace GraphicsEngineSpace
{
	#define FPS_INTERVAL 0.5f

	public ref class _Timer
	{
	public:

		_Timer();

		property bool FixedMode { bool get() { return m_FixedMode; } }
		void SetFixedMode(bool value) { m_FixedMode = value; if (m_FixedMode) Reset(); }

		property float DeltaTime { float get() { return m_DeltaTime; } }

		property float GetTime { float get() { return m_LastTime; } }

		property float Fps { float get() { return m_Fps; } }

		property bool IsRunning { bool get() { return m_IsRunning; } }

		property DWORD FrameCount { DWORD get() { return m_FrameCount; } }

		void Start();
		void Stop();
		void Continue();
		void Reset();
		void Update();
		void WaitForFixedTime();

		float AtOneSecond(float value) { return value * m_DeltaTime; }
		float AtXSecond(float seconds, float value) { return (value / seconds) * m_DeltaTime; }

	private:

		float CalculateTime();

		bool	m_IsRunning;
		UINT64	m_BaseTicks;
		UINT64	m_StopTicks;
		UINT64	m_TicksPerSecond;
		float	m_LastTime;
		float	m_FrameTime;
		float	m_DeltaTime;
		DWORD	m_FrameCount;
		float	m_LastUpdate;
		float	m_Fps;
		bool	m_FixedMode;
		float	m_FixedModeFps;
		bool	m_FixedModeWait;
		float	m_LastTimeForFixedTime;

	};
}
