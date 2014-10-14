#pragma once

namespace GraphicsEngineSpace {

	public ref class Mouse
	{
	private:
		
		int m_IncX;
		int m_IncY;
		
		int m_lastX;
		int m_lastY;

		int m_X;
		int m_Y;

		LPDIRECTINPUT8          m_DI;
		LPDIRECTINPUTDEVICE8    m_Mouse;
		bool m_Buttons0;
		bool m_Buttons1;
		bool m_Buttons2;
	public:
		property int XInc { int get() { return m_IncX; } }
		property int YInc { int get() { return m_IncY; } }
		property int X { int get() { return m_X; } }
		property int Y { int get() { return m_Y; } }
		property bool Button0 { bool get() { return m_Buttons0; } }
		property bool Button1 { bool get() { return m_Buttons1; } }
		property bool Button2 { bool get() { return m_Buttons2; } }
		Mouse();
		~Mouse();
		bool Initialize( int hDlg );
		void Update();
		void Destroy();
	};
}
