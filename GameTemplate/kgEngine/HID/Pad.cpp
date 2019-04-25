/*!
 *@brief	パッド。
 */
#include "KGstdafx.h"
#include "HID/Pad.h"
#include <limits.h>

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )  // 入力の遊びとなる範囲。
Pad g_pad[Pad::CONNECT_PAD_MAX];		//g_padの定義。

/*!
 *@brief	仮想ボタンとXBoxコントローラのボタンとの関連付けを表す構造体。
 */
struct VirtualPadToXPad {
	EnButton vButton;		//!<仮想ボタン。
	DWORD	 xButton;		//!<XBoxコントローラのボタン。
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
*@brief	仮想ボタンとキーボードとの関連付けを表す構造体。
*/
struct VirtualPadToKeyboard {
	EnButton vButton;		//!<仮想ボタン
	DWORD keyCoord;			//!<キーボードのキーコード。
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
*@brief	ボタンの入力情報を更新。
*/
void Pad::UpdateButtonInput()
{
	//XInputState.Gamepad.wButtonsに入力されたパッドの情報がビットパターンとして入っている。
	//XINPUT_GAMEPAD_???には、対応するボタンのビットの位置に1が入っている。
	//例えば、Aボタンは１３ビット目、Bボタンは１４ビット目に１が立つ場合、
	//AとB両方が押されていたら、wButtonsには下記のような値になっている。
	//0011 0000 0000 0000
	//定数、XINPUT_GAMEPAD_Aは 0001 0000 0000 0000という値が入っているので、
	//wButtonsとXINPUT_GAMEPAD_Aの論理積(&)を取ると、下記のような結果となる。
	//   0011 0000 0000 0000(wButtons)
	// & 0001 0000 0000 0000(XINPUT_GAMEPAD_A)
	// = 0001 0000 0000 0000(wButtons & XINPUT_GAMEPAD_A の結果)
	//このように、ビットパターンと論理積を取った時に、0出ない場合はそのパッドが入力されていると判断することができる。

	//実習 1 残りのボタンも入力をとれるようにしてみよう。
	// vPadToXPadTableは仮想キー(enButton～)とXBoxコントローラのボタン(XINPUT_GAMEPAD_～)の対応データの配列となっている。
	for (const auto& vPadToXPad : vPadToXPadTable) {
		if ((m_state.xInputState.Gamepad.wButtons & vPadToXPad.xButton) != 0) {
			if (m_press[vPadToXPad.vButton] == 0) {
				//1フレーム前は押されていないので、トリガー入力のフラグを立てる。
				m_trigger[vPadToXPad.vButton] = 1;
			}
			else {
				//1フレーム前にも押されているので、トリガー入力は終わり。
				m_trigger[vPadToXPad.vButton] = 0;
			}
			//押されているフラグを立てる。
			m_press[vPadToXPad.vButton] = 1;

		}
		else {
			//押されていない。
			m_trigger[vPadToXPad.vButton] = 0;
			m_press[vPadToXPad.vButton] = 0;
		}
	}

	//左トリガー(LB2ボタン)の入力判定。
	if (m_state.xInputState.Gamepad.bLeftTrigger != 0) {
		m_trigger[enButtonLB2] = 1 ^ m_press[enButtonLB2];
		m_press[enButtonLB2] = 1;
	}
	else {
		m_trigger[enButtonLB2] = 0;
		m_press[enButtonLB2] = 0;
	}
	//右トリガー(RB2ボタン)の入力判定。
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
*@brief	アナログスティックの入力情報を更新。
*/
void Pad::UpdateAnalogStickInput()
{
	//xInputState.Gamepad.sThumbLX、sThumbLYにLスティックのX方向のとY方向の入力情報がsigned short(-32,768 ～ 32,767)の範囲で入っている。
	//この値がINPUT_DEADZONE以下の場合は、入力されていないとする。スティックの遊びです。
	//これがないとちょっと触れただけでキャラが動いてしまって、イライラすることとなります。
	if ((m_state.xInputState.Gamepad.sThumbLX < INPUT_DEADZONE &&
		m_state.xInputState.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
		(m_state.xInputState.Gamepad.sThumbLY < INPUT_DEADZONE &&
			m_state.xInputState.Gamepad.sThumbLY > -INPUT_DEADZONE))
	{
		//何も入力されていないとする。
		m_state.xInputState.Gamepad.sThumbLX = 0;
		m_state.xInputState.Gamepad.sThumbLY = 0;
		m_lStickX = 0.0f;
		m_lStickY = 0.0f;
	}
	else {
		//左スティックの入力量を-32,768 ～ 32,767から-1.0～1.0に変換する。
		//なぜ変換するかというと、その方がゲームを作りやすいから。
		if (m_state.xInputState.Gamepad.sThumbLX > 0) {
			//SHRT_MAXはlimits.hに定義されている定数で、32767という値が入っています。
			m_lStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbLX) / SHRT_MAX;
		}
		else {
			//SHRT_MINはlimits.hに定義されている定数で、-32768という値が入っています。
			m_lStickX = static_cast<float>(m_state.xInputState.Gamepad.sThumbLX) / -SHRT_MIN;
		}
		if (m_state.xInputState.Gamepad.sThumbLY > 0) {
			m_lStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbLY) / SHRT_MAX;
		}
		else {
			m_lStickY = static_cast<float>(m_state.xInputState.Gamepad.sThumbLY) / -SHRT_MIN;
		}
	}

	//実習２　左スティックの入力の取得の仕方を参考にして、右スティックも入力をとれるようにしてみよう。
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
		//右スティックの入力量。
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
*@brief	パッドの入力を更新。
*/
void Pad::Update()
{
	//XInputGetState関数を使って、ゲームパッドの入力状況を取得する。
	DWORD result = XInputGetState(m_padNo, &m_state.xInputState);
	if (result == ERROR_SUCCESS) {
		//ゲームパッドが接続されている
		//接続されている。
		m_state.bConnected = true;

		//ボタンの入力情報を更新。
		UpdateButtonInput();

		//アナログスティックの入力情報を更新。
		UpdateAnalogStickInput();
	}
	else {
		//ゲームパッドが接続されていない。
		//接続されていない場合はキーボードの入力でエミュレートする。
		if (m_state.bConnected) {
			//未接続になった。
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
		//スティックの入力量を正規化。
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
		//スティックの入力量を正規化。
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

