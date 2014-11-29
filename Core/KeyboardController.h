#ifndef __KEYBOARD_CONTROLLER_H__
#define __KEYBOARD_CONTROLLER_H__

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
		class KeyboardController
		{
		public:
			KeyboardController(void);
			~KeyboardController(void);

			bool initialize(IDirectInput8* pDirectInput, HWND hwnd);

			bool readKeyboardData();
			bool isPressed(uint8 KEY) const;

		private:
			IDirectInputDevice8* pKeyboardDevice;
			uint8* pCharTable;

			const uint16 countSymbols;
		};
	}
}

#endif
