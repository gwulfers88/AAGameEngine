#include "DirectInput.h"

DirectInput::DirectInput()
{
	directInput_ = nullptr;
	keyboardDevice_ = nullptr;
}

DirectInput::~DirectInput()
{
}

void DirectInput::ReleaseAll()
{
	if (keyboardDevice_)
	{
		keyboardDevice_->Unacquire();
		keyboardDevice_->Release();
	}

	keyboardDevice_ = nullptr;
	directInput_ = nullptr;
}

bool DirectInput::Init(HINSTANCE &hinstance, HWND& hw)
{
	HRESULT result;
	ZeroMemory(keyboardKeys_, sizeof(keyboardKeys_));
	ZeroMemory(prevKeyboardKeys_, sizeof(prevKeyboardKeys_));

	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput_, 0);

	if (FAILED(result))
	{
		return false;
	}

	result = directInput_->CreateDevice(GUID_SysKeyboard, &keyboardDevice_, 0);

	if (FAILED(result))
	{
		return false;
	}

	result = keyboardDevice_->SetDataFormat(&c_dfDIKeyboard);

	if (FAILED(result))
	{
		return false;
	}

	result = keyboardDevice_->SetCooperativeLevel(hw, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	if (FAILED(result))
	{
		return false;
	}

	result = keyboardDevice_->Acquire();

	if (FAILED(result))
	{
		MessageBox(0, "DirectInput Init fall!", "DirectInput Error", MB_OK);
		return false;
	}
	return true;
}

void DirectInput::Update()
{

	keyboardDevice_->GetDeviceState(sizeof(keyboardKeys_), (LPVOID)&keyboardKeys_);

	//		We can use GetAsyncKeyState to essentially tell us if a button is being pressed.
	//The only parameter to this function is the virtual key code for the keyboard button we 
	//want to test for, and this provides an easy method to detect keyboard input.

	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	// Button up event.
	if (KEYDOWN(prevKeyboardKeys_, DIK_DOWN) && !KEYDOWN(keyboardKeys_, DIK_DOWN))
	{
		//selectedColor_--;
	}

	// Button up event.
	if (KEYDOWN(prevKeyboardKeys_, DIK_UP) && !KEYDOWN(keyboardKeys_, DIK_UP))
	{
		//selectedColor_++;
	}

	memcpy(prevKeyboardKeys_, keyboardKeys_, sizeof(keyboardKeys_));


	//if (selectedColor_ < 0) selectedColor_ = 2;
	//if (selectedColor_ > 2) selectedColor_ = 0;
}