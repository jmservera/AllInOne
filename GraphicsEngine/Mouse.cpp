#include "stdafx.h"

#include "AllHeaders.h"

namespace GraphicsEngineSpace {

// Here we define a custom data format to store input from a mouse. In a 
// real program you would almost certainly use either the predefined 
// DIMOUSESTATE or DIMOUSESTATE2 structure to store mouse input, but some 
// input devices such as the Sidewinder GameVoice controller are not well
// described by the provided types and may require custom formats.

struct MouseState
{
    LONG lAxisX;
    LONG lAxisY;
    BYTE abButtons[3];
    BYTE bPadding;       // Structure must be DWORD multiple in size.   
};

// Each device object for which you want to receive input must have an entry
// in this DIOBJECTDATAFORMAT array which is stored in the custom DIDATAFORMAT.
// The DIOBJECTDATAFORMAT maps detected device object to a particular offset
// within MouseState structure declared above. Inside the input routine, a
// MouseState structure is provided to the GetDeviceState method, and
// DirectInput uses this offset to store the input data in the provided
// structure. 
// 
// Any of the elements which are not flagged as DIDFT_OPTIONAL, and
// which describe a device object which is not found on the actual device will
// cause the SetDeviceFormat call to fail. For the format defined below, the
// system mouse must have an x-axis, y-axis, and at least one button. 

DIOBJECTDATAFORMAT g_aObjectFormats[] =
{
    { &GUID_XAxis, FIELD_OFFSET( MouseState, lAxisX ),    // X axis
        DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
    { &GUID_YAxis, FIELD_OFFSET( MouseState, lAxisY ),    // Y axis
        DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 },
    { 0, FIELD_OFFSET( MouseState, abButtons[0] ),        // Button 0
        DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
    { 0, FIELD_OFFSET( MouseState, abButtons[1] ),        // Button 1 (optional)
        DIDFT_BUTTON | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0 },
    { 0, FIELD_OFFSET( MouseState, abButtons[2] ),        // Button 2 (optional)
        DIDFT_BUTTON | DIDFT_ANYINSTANCE | DIDFT_OPTIONAL, 0 }
};
#define numMouseObjects (sizeof(g_aObjectFormats) / sizeof(DIOBJECTDATAFORMAT))

// Finally, the DIDATAFORMAT is filled with the information defined above for 
// our custom data format. The format also defines whether the returned axis 
// data is absolute or relative. Usually mouse movement is reported in relative 
// coordinates, but our custom format will use absolute coordinates. 

DIDATAFORMAT            g_dfMouse =
{
    sizeof( DIDATAFORMAT ),
    sizeof( DIOBJECTDATAFORMAT ),
    DIDF_ABSAXIS,
    sizeof( MouseState ),
    numMouseObjects,
    g_aObjectFormats
};

	Mouse::Mouse()
	{
		m_DI=NULL;
		m_Mouse=NULL;
	}

	Mouse::~Mouse()
	{
		Destroy();
	}

	void Mouse::Destroy()
	{
		// Unacquire the device one last time just in case 
		// the app tried to exit while the device is still acquired.
		if( m_Mouse )
		{
			m_Mouse->Unacquire();
			m_Mouse->Release();
			m_Mouse=NULL;
		}

		if(m_DI)
		{
			m_DI->Release();
			m_DI=NULL;
		}
	}

	void Mouse::Update()
	{
		MouseState ms;           // Custom mouse state 

		static POINT pOrigin = {0};           // Initial position
		static bool bInitialized= FALSE;    // Indicates offsets are valid

		// Poll the device to read the current state
		HRESULT hr = m_Mouse->Poll();
		if( FAILED( hr ) )
		{
			// DInput is telling us that the input stream has been
			// interrupted. We aren't tracking any state between polls, so
			// we don't have any special reset that needs to be done. We
			// just re-acquire and try again.
			hr = m_Mouse->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = m_Mouse->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of 
			// switching, so just try again later 
			//return S_OK;
		}

		// Get the input's device state
		if( FAILED( hr = m_Mouse->GetDeviceState( sizeof( MouseState ), &ms ) ) )
		{
			InternalLog.WriteDXError("Mouse::Update. The device should have been acquired during the Poll()",hr);
			
			//return false;
		}

		// The initial mouse position should be subracted from the current point. 
		if( !bInitialized )
		{
			bInitialized = TRUE;
			pOrigin.x = ms.lAxisX;
			pOrigin.y = ms.lAxisY;

		}

		m_X=ms.lAxisX - pOrigin.x;
		m_Y=ms.lAxisY - pOrigin.y;

		m_IncX=m_X-m_lastX;
		m_IncY=m_Y-m_lastY;

		m_lastX=m_X;
		m_lastY=m_Y;

		m_Buttons0=(( ms.abButtons[0] & 0x80 )!=0);
		m_Buttons1=(( ms.abButtons[1] & 0x80 )!=0);
		m_Buttons2=(( ms.abButtons[2] & 0x80 )!=0);
	}
		
	//-----------------------------------------------------------------------------
	// Name: InitDirectInput()
	// Desc: Initialize the DirectInput variables.
	//-----------------------------------------------------------------------------
	bool Mouse::Initialize( int hDlg )
	{
		HRESULT hr;

		// Register with the DirectInput subsystem and get a pointer
		// to a IDirectInput interface we can use.
		pin_ptr<LPDIRECTINPUT8> di=&m_DI;
		if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
											 IID_IDirectInput8, ( VOID** )di, NULL ) ) )
		{
			FatalLog.WriteDXError("DirectInput8Create",hr);
			
			return false;
		}

		// Retrieve the system mouse
		pin_ptr<LPDIRECTINPUTDEVICE8> mouse=&m_Mouse;
		if( FAILED( m_DI->CreateDevice( GUID_SysMouse, mouse, NULL ) ) )
		{
			InternalLog.WriteDXError("Mouse not found",hr);

			return false;
		}

		// A data format specifies which controls on a device we are interested in,
		// and how they should be reported. This tells DInput that we will be
		// passing a MouseState structure to IDirectInputDevice::GetDeviceState().
		if( FAILED( hr = m_Mouse->SetDataFormat( &g_dfMouse ) ) )
		{
			InternalLog.WriteDXError("DirectInput8Create.SetDataFormat",hr);
			
			return false;
		}

		// Set the cooperative level to let DInput know how this device should
		// interact with the system and with other DInput applications.
		if( FAILED( hr = m_Mouse->SetCooperativeLevel( (HWND)hDlg, DISCL_NONEXCLUSIVE |
														DISCL_FOREGROUND ) ) )
		{
			InternalLog.WriteDXError("DirectInput8Create.SetCooperativeLevel",hr);
			
			return false;
		}

		return true;
	}

}
