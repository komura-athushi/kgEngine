/*!
 *@brief	�p�b�h�B
 */
#include "KGstdafx.h"
#include "HID/Pad.h"
#include <limits.h>

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // ���̗͂V�тƂȂ�͈́B
Pad g_pad[Pad::CONNECT_PAD_MAX];		//g_pad�̒�`�B

/*!
 *@brief	���z�{�^����XBox�R���g���[���̃{�^���Ƃ̊֘A�t����\���\���́B
 */
struct VirtualPadToXPad {
	EnButton vButton;		//!<���z�{�^���B
	DWORD	 xButton;		//!<XBox�R���g���[���̃{�^���B
};

VirtualPadToXPad vPadToXPadTable[enButtonNum] = {
	{ enButtonUp		, XINPUT_GAMEPAD_DPAD_UP },
	{ enButtonDown		, XINPUT_GAMEPAD_DPAD_DOWN },
	{ enButtonLeft		, XINPUT_GAMEPAD_DPAD_LEFT },
	{ enButtonRight		, XINPUT_GAMEPAD_DPAD_RIGHT },
	{ enButtonA			, XINPUT_GAMEPAD_A },
	{ enButtonB			, XINPUT_GAMEPAD_B },
	{ enButtonY			, XINPUT_GAMEPAD_Y },
	{ enButtonX			, XINPUT_GAMEPAD_X },
	{ enButtonSelect	, XINPUT_GAMEPAD_BACK },
	{ enButtonStart		, XINPUT_GAMEPAD_START },
	{ enButtonRB1		, XINPUT_GAMEPAD_RIGHT_SHOULDER },
	{ enButtonRB2		, 0 },
	{ enButtonRB3		, XINPUT_GAMEPAD_RIGHT_THUMB },
	{ enButtonLB1		, XINPUT_GAMEPAD_LEFT_SHOULDER },
	{ enButtonLB2		, 0 },
	{ enButtonLB3		, XINPUT_GAMEPAD_LEFT_THUMB },
};
/*!
*@brief	���z�{�^���ƃL�[�{�[�h�Ƃ̊֘A�t����\���\���́B
*/
struct VirtualPadToKeyboard {
	EnButton vButton;		//!<���z�{�^��
	DWORD keyCoord;			//!<�L�[�{�[�h�̃L�[�R�[�h�B
};
const VirtualPadToKeyboard vPadToKeyboardTable[enButtonNum] = {
	{ enButtonUp		, '8' },
	{ enButtonDown		, '2' },
	{ enButtonLeft		, '4' },
	{ enButtonRight		, '6' },
	{ enButtonA			, 'J' },
	{ enButtonB			, 'K' },
	{ enButtonY			, 'I' },
	{ enButtonX			, 'L' },
	{ enButtonSelect	, VK_SPACE },
	{ enButtonStart		, VK_RETURN },
	{ enButtonRB1		, '7' },
	{ enButtonRB2		, '8' },
	{ enButtonRB3		, '9' },
	{ enButtonLB1		, 'B' },
	{ enButtonLB2		, 'N' },
	{ enButtonLB3		, 'M' },
};

Pad::Pad() 
{
	memset(&m_state, 0, sizeof(m_state));
	memset(m_trigger, 0, sizeof(m_trigger));
	memset(m_press, 0, sizeof(m_press));
}
Pad::~Pad()
{
}
/*!
*@brief	�{�^���̓��͏����X�V�B
*/
void Pad::UpdateButtonInput()
{
	//XInputState.Gamepad.wButtons�ɓ��͂��ꂽ�p�b�h�̏�񂪃r�b�g�p�^�[���Ƃ��ē����Ă���B
	//XINPUT_GAMEPAD_???�ɂ́A�Ή�����{�^���̃r�b�g�̈ʒu��1�������Ă���B
	//�Ⴆ�΁AA�{�^���͂P�R�r�b�g�ځAB�{�^���͂P�S�r�b�g�ڂɂP�����ꍇ�A
	//A��B������������Ă�����AwButtons�ɂ͉��L�̂悤�Ȓl�ɂȂ��Ă���B
	//0011 0000 0000 0000
	//�萔�AXINPUT_GAMEPAD_A�� 0001 0000 0000 0000�Ƃ����l�������Ă���̂ŁA
	//wButtons��XINPUT_GAMEPAD_A�̘_����(&)�����ƁA���L�̂悤�Ȍ��ʂƂȂ�B
	//   0011 0000 0000 0000(wButtons)
	// & 0001 0000 0000 0000(XINPUT_GAMEPAD_A)
	// = 0001 0000 0000 0000(wButtons & XINPUT_GAMEPAD_A �̌���)
	//���̂悤�ɁA�r�b�g�p�^�[���Ƙ_���ς���������ɁA0�o�Ȃ��ꍇ�͂��̃p�b�h�����͂���Ă���Ɣ��f���邱�Ƃ��ł���B

	//���K 1 �c��̃{�^�������͂��Ƃ��悤�ɂ��Ă݂悤�B
	// vPadToXPadTable�͉��z�L�[(enButton�`)��XBox�R���g���[���̃{�^��(XINPUT_GAMEPAD_�`)�̑Ή��f�[�^�̔z��ƂȂ��Ă���B
	for (const auto& vPadToXPad : vPadToXPadTable) {
		if ((m_state.xInputState.Gamepad.wButtons & vPadToXPad.xButton) != 0) {
			if (m_press[vPadToXPad.vButton] == 0) {
				//1�t���[���O�͉�����Ă��Ȃ��̂ŁA�g���K�[���͂̃t���O�𗧂Ă�B
				m_trigger[vPadToXPad.vButton] = 1;
			}
			else {
				//1�t���[���O�ɂ�������Ă���̂ŁA�g���K�[���͂͏I���B
				m_trigger[vPadToXPad.vButton] = 0;
			}
			//������Ă���t���O�𗧂Ă�B
			m_press[vPadToXPad.vButton] = 1;

		}
		else {
			//������Ă��Ȃ��B
			m_trigger[vPadToXPad.vButton] = 0;
			m_press[vPadToXPad.vButton] = 0;
		}
	}

	//���g���K�[(LB2�{�^��)�̓��͔���B
	if (m_state.xInputState.Gamepad.bLeftTrigger != 0) {
		m_trigger[enButtonLB2] = 1 ^ m_press[enButtonLB2];
		m_press[enButtonLB2] = 1;
	}
	else {
		m_trigger[enButtonLB2] = 0;
		m_press[enButtonLB2] = 0;
	}
	//�E�g���K�[(RB2�{�^��)�̓��͔���B
	if (m_state.xInputState.Gamepad.bRightTrigger != 0) {
		m_trigger[enButtonRB2] = 1 ^ m_press[enButtonRB2];
		m_press[enButtonRB2] = 1;
	}
	else {
		m_trigger[enButtonRB2] = 0;
		m_press[enButtonRB2] = 0;
	}
}
/*!
*@brief	�A�i���O�X�e�B�b�N�̓��͏����X�V�B
*/
void Pad::UpdateAnalogStickInput()
{
	//xInputState.Gamepad.sThumbLX�AsThumbLY��L�X�e�B�b�N��X�����̂�Y�����̓��͏��signed short(-32,768 �` 32,767)�͈̔͂œ����Ă���B
	//���̒l��INPUT_DEADZONE�ȉ��̏ꍇ�́A���͂���Ă��Ȃ��Ƃ���B�X�e�B�b�N�̗V�тł��B
	//���ꂪ�Ȃ��Ƃ�����ƐG�ꂽ�����ŃL�����������Ă��܂��āA�C���C�����邱�ƂƂȂ�܂��B
	if ((m_state.xInputState.Gamepad.sThumbLX < INPUT_DEADZONE &&
		m_state.xInputState.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
		(m_state.xInputState.Gamepad.sThumbLY < INPUT_DEADZONE &&
			m_state.xInputState.Gamepad.sThumbLY > -INPUT_DEADZONE))
	{
		//�������͂���Ă��Ȃ��Ƃ���B
		m_state.xInputState.Gamepad.sThumbLX = 0;
		m_state.xInputState.Gamepad.sThumbLY = 0;
		m_lStickX = 0.0f;
		m_lStickY = 0.0f;
	}
	else {
		//���X�e�B�b�N�̓��͗ʂ�-32,768 �` 32,767����-1.0�`1.0�ɕϊ�����B
		//�Ȃ��ϊ����邩�Ƃ����ƁA���̕����Q�[�������₷������B
		if (m_state.xInputState.Gamepad.sThumbLX > 0) {
			//SHRT_MAX��limits.h�ɒ�`����Ă���萔�ŁA32767�Ƃ����l�������Ă��܂��B
			m_lStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbLX) / SHRT_MAX;
		}
		else {
			//SHRT_MIN��limits.h�ɒ�`����Ă���萔�ŁA-32768�Ƃ����l�������Ă��܂��B
			m_lStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbLX) / -SHRT_MIN;
		}
		if (m_state.xInputState.Gamepad.sThumbLY > 0) {
			m_lStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbLY) / SHRT_MAX;
		}
		else {
			m_lStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbLY) / -SHRT_MIN;
		}
	}

	//���K�Q�@���X�e�B�b�N�̓��͂̎擾�̎d�����Q�l�ɂ��āA�E�X�e�B�b�N�����͂��Ƃ��悤�ɂ��Ă݂悤�B
	if ((m_state.xInputState.Gamepad.sThumbRX < INPUT_DEADZONE &&
		m_state.xInputState.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
		(m_state.xInputState.Gamepad.sThumbRY < INPUT_DEADZONE &&
			m_state.xInputState.Gamepad.sThumbRY > -INPUT_DEADZONE))
	{
		m_state.xInputState.Gamepad.sThumbRX = 0;
		m_state.xInputState.Gamepad.sThumbRY = 0;
		m_rStickX = 0.0f;
		m_rStickY = 0.0f;
	}
	else {
		//�E�X�e�B�b�N�̓��͗ʁB
		if (m_state.xInputState.Gamepad.sThumbRX > 0) {
			m_rStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbRX) / SHRT_MAX;
		}
		else {
			m_rStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbRX) / -SHRT_MIN;
		}
		if (m_state.xInputState.Gamepad.sThumbRY > 0) {
			m_rStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbRY) / SHRT_MAX;
		}
		else {
			m_rStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbRY) / -SHRT_MIN;
		}
	}
}
/*!
*@brief	�p�b�h�̓��͂��X�V�B
*/
void Pad::Update()
{
	//XInputGetState�֐����g���āA�Q�[���p�b�h�̓��͏󋵂��擾����B
	DWORD result = XInputGetState(m_padNo, &m_state.xInputState);
	if (result == ERROR_SUCCESS) {
		//�Q�[���p�b�h���ڑ�����Ă���
		//�ڑ�����Ă���B
		m_state.bConnected = true;

		//�{�^���̓��͏����X�V�B
		UpdateButtonInput();

		//�A�i���O�X�e�B�b�N�̓��͏����X�V�B
		UpdateAnalogStickInput();
	}
	else {
		//�Q�[���p�b�h���ڑ�����Ă��Ȃ��B
		//�ڑ�����Ă��Ȃ��ꍇ�̓L�[�{�[�h�̓��͂ŃG�~�����[�g����B
		if (m_state.bConnected) {
			//���ڑ��ɂȂ����B
			memset(&m_state, 0, sizeof(m_state));
			memset(m_trigger, 0, sizeof(m_trigger));
			memset(m_press, 0, sizeof(m_press));
		}
		m_lStickX = 0.0f;
		m_lStickY = 0.0f;
		m_rStickX = 0.0f;
		m_rStickY = 0.0f;

		if (GetAsyncKeyState(VK_LEFT)) {
			m_rStickX = -1.0f;
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			m_rStickX = 1.0f;
		}
		if (GetAsyncKeyState(VK_UP)) {
			m_rStickY = 1.0f;
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			m_rStickY = -1.0f;
		}
		//�X�e�B�b�N�̓��͗ʂ𐳋K���B
		float t = fabsf(m_rStickX) + fabsf(m_rStickY);
		if (t > 0.0f) {
			m_rStickX /= t;
			m_rStickY /= t;
		}

		if (GetAsyncKeyState('A')) {
			m_lStickX = -1.0f;
		}
		else if (GetAsyncKeyState('D')) {
			m_lStickX = 1.0f;
		}
		if (GetAsyncKeyState('W')) {
			m_lStickY = 1.0f;
		}
		else if (GetAsyncKeyState('S')) {
			m_lStickY = -1.0f;
		}
		//�X�e�B�b�N�̓��͗ʂ𐳋K���B
		t = fabsf(m_lStickX) + fabsf(m_lStickY);
		if (t > 0.0f) {
			m_lStickX /= t;
			m_lStickY /= t;
		}

		for (const VirtualPadToKeyboard& vPadToKeyboard : vPadToKeyboardTable) {
			if (GetAsyncKeyState(vPadToKeyboard.keyCoord)) {
				m_trigger[vPadToKeyboard.vButton] = 1 ^ m_press[vPadToKeyboard.vButton];
				m_press[vPadToKeyboard.vButton] = 1;
			}
			else {
				m_trigger[vPadToKeyboard.vButton] = 0;
				m_press[vPadToKeyboard.vButton] = 0;
			}
		}
	}
	
}

