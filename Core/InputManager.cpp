#include "InputManager.h"

namespace OGE
{
	namespace input
	{
		bool InputManager::initialize(HINSTANCE hInstance, HWND hwnd)
		{
			// init direct input
			if(pDirectInput != NULL)
			{
				freeControllers();
			}

			HRESULT result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, 
				IID_IDirectInput8, (void**)&pDirectInput, NULL);

			if(FAILED(result))
			{
				return false;
			}

			bool mouseResult = true;
			bool keyboardResult = true;

			// init mouse controller
			if(!pMouseController->initialize(pDirectInput, hwnd))
			{
				mouseResult = !mouseResult;
			}

			// init keyboard controller
			if(!pKeyboardController->initialize(pDirectInput, hwnd))
			{
				keyboardResult = !keyboardResult;
			}

			return keyboardResult && mouseResult;
		}

		void InputManager::freeControllers()
		{
			if(pDirectInput != NULL)
			{
				pDirectInput->Release();
				pDirectInput = NULL;
			}
			
			safe_release(pMouseController);
			safe_release(pKeyboardController);
		}
	}
}
