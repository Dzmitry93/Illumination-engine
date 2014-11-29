#include "MouseController.h"

namespace OGE
{
	namespace input
	{

		bool MouseController::initialize(IDirectInput8* pDirectInput, HWND hwnd)
		{
			// Initialize the direct input interface for the keyborad.
			HRESULT result = pDirectInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
			if(FAILED(result))
			{
				return false;
			}

			// Set the data format for the keyboard using the pre-defined keyboard data format.
			result = pMouseDevice->SetDataFormat(&c_dfDIMouse);
			if(FAILED(result))
			{
				return false;
			}

			// Set the cooperative level of the keyboard to share with other programs.
			result = pMouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
			if(FAILED(result))
			{
				return false;
			}

			// Acquire the keyboard.
			result = pMouseDevice->Acquire();
			if(FAILED(result))
			{
				return false;
			}

			return true;
		}
	}
}