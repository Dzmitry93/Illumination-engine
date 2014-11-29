#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "MouseController.h"
#include "KeyboardController.h"
#include "ISystemManager.h"

namespace OGE
{
	namespace input
	{
		class InputManager : public ISystemManager
		{
		public:

			InputManager(void);
			~InputManager(void);

			void start();
			void shutdown();

			bool initialize(HINSTANCE hInstance, HWND hwnd);

			MouseController* getMouserController() const;
			KeyboardController* getKeyboardController() const;

		private:
			IDirectInput8* pDirectInput;

			KeyboardController* pKeyboardController;
			MouseController* pMouseController;

			void freeControllers();
		};

		inline InputManager::InputManager(void)
		{
			this->pDirectInput = NULL;
			this->pMouseController = new MouseController();
			this->pKeyboardController = new KeyboardController();
		}

		inline InputManager::~InputManager(void)
		{
			shutdown();
		}

		inline void InputManager::start()
		{
		}

		inline void InputManager::shutdown()
		{
			freeControllers();
		}

		inline MouseController* InputManager::getMouserController() const
		{
			return pMouseController;
		}

		inline KeyboardController* InputManager::getKeyboardController() const
		{
			return pKeyboardController;
		}
	}
}

#endif