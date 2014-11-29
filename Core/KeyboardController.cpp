#include "KeyboardController.h"

namespace OGE
{
	namespace input
	{
		KeyboardController::KeyboardController(void) : countSymbols(256)
		{
			pKeyboardDevice = NULL;
			pCharTable = new uint8[countSymbols];
		}

		KeyboardController::~KeyboardController(void)
		{
			if(pKeyboardDevice != NULL)
			{
				pKeyboardDevice->Unacquire();
				pKeyboardDevice->Release();
				pKeyboardDevice = NULL;
			}
			if(pCharTable != NULL)
			{
				delete pCharTable;
				pCharTable = NULL;
			}
		}

		bool KeyboardController::initialize(IDirectInput8* pDirectInput, HWND hwnd)
		{
			// Initialize the direct input interface for the keyborad.
			HRESULT result = pDirectInput->CreateDevice(GUID_SysKeyboard, &pKeyboardDevice, NULL);
			if(FAILED(result))
			{
				error_msg("cannot create keyboard device");
				return false;
			}

			// Set the data format for the keyboard using the pre-defined keyboard data format.
			result = pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
			if(FAILED(result))
			{
				error_msg("cannot set keyboard data format");
				return false;
			}

			// Set the cooperative level of the keyboard to share with other programs.
			result = pKeyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
			if(FAILED(result))
			{
				error_msg("cannot set keyboard cooperative level");
				return false;
			}

			// Acquire the keyboard.
			result = pKeyboardDevice->Acquire();
			if(FAILED(result))
			{
				error_msg("cannot acquire keyboard");
				return false;
			}

			return true;
		}

		bool KeyboardController::readKeyboardData()
		{
			HRESULT result = pKeyboardDevice->GetDeviceState(sizeof(uint8) * 256, (void*)pCharTable);

			if(FAILED(result))
			{
				// If the keyboard lost focus or was not acquired then try to get control back.
				if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
				{
					//error_msg("keyboard device is lost");
					pKeyboardDevice->Acquire();
				}
				log_msg("fail to read mouse input");
				return false;
			}
			else
			{
				return true;
			}
		}

		bool KeyboardController::isPressed(uint8 key) const
		{
			if(pCharTable != NULL)
			{
				bool result = pCharTable[key];
				pCharTable[key] = false;
				return result;
			}
			else
			{
				error_msg("keyboard char table is null");
				return false;
			}
		}
	}
}
