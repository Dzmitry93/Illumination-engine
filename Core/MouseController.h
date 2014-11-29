#ifndef __MOUSE_CONTROLLER_H__
#define __MOUSE_CONTROLLER_H__

#include "Debug.h"
#include "TypeDefinitions.h"

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


namespace OGE
{
	namespace input
	{
		class MouseController
		{
		public:
			MouseController(void);
			~MouseController(void);

			bool initialize(IDirectInput8* pDirectInput, HWND hwnd);

			bool readMouseInput();
			int32 getPositionX() const;
			int32 getPositionY() const;
			void clear();

		private:
			IDirectInputDevice8* pMouseDevice;
			DIMOUSESTATE mouseState;
		};

		inline MouseController::MouseController()
		{
			pMouseDevice = NULL;
			ZeroMemory(&mouseState, sizeof(mouseState));
		}

		inline MouseController::~MouseController()
		{
			if(pMouseDevice != NULL)
			{
				pMouseDevice->Unacquire();
				pMouseDevice->Release();
			}
		}

		inline bool MouseController::readMouseInput()
		{
			HRESULT result = pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void*)&mouseState);

			if(FAILED(result))
			{
				// If the keyboard lost focus or was not acquired then try to get control back.
				if((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
				{
					pMouseDevice->Acquire();
				}
				log_msg("fail to read mouse input");
				return false;	
			}
			else
			{
				return true;
			}
		}

		inline void MouseController::clear() 
		{
			ZeroMemory(&mouseState, sizeof(mouseState));
		}

		inline int32 MouseController::getPositionX() const
		{
			return mouseState.lX;
		}

		inline int32 MouseController::getPositionY() const
		{
			return mouseState.lY;
		}
	}
}

#endif

