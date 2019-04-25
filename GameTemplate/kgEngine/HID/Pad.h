/*!
 *@brief	パッド
 */

#pragma once

 //XInputのヘッダーファイル。
#include <Xinput.h>

/*!
 *@brief	仮想ボタン定義。
 */
enum EnButton{
	enButtonUp,		//!<上。
	enButtonDown,		//!<下。
	enButtonLeft,		//!<左。
	enButtonRight,		//!<右。
	enButtonA,			//!<Aボタン。
	enButtonB,			//!<Bボタン。
	enButtonX,			//!<Xボタン。
	enButtonY,			//!<Yボタン。
	enButtonSelect,	//!<セレクトボタン。
	enButtonStart,		//!<スタートボタン。
	enButtonRB1,		//!<RB1ボタン。
	enButtonRB2,		//!<RB2ボタン。
	enButtonRB3,		//!<RB3ボタン。
	enButtonLB1,		//!<LB1ボタン。
	enButtonLB2,		//!<LB2ボタン。
	enButtonLB3,		//!<LB3ボタン。
	enButtonNum,	//!<ボタンの数。
};
/*!
 *@brief	パッド
 */
class Pad{
public:
	static const int CONNECT_PAD_MAX = 4;		//接続可能なパッドの最大数。
	/*!
	 *@brief	パッドステート。
	 */
	struct PAD_STATE
	{
		XINPUT_STATE xInputState;	//XInputGetStateを使用して、取得するパッドの入力状況。
		bool bConnected;
	};
	/*!
	 *@brief	コンストラクタ。
	 */
	Pad();
	/*!
	 *@brief	デストラクタ。
	 */
	~Pad();
	/*!
	*@brief	初期化。
	*@param[in]	padNo	パッド番号。
	*/
	void Init(int padNo)
	{
		m_padNo = padNo;
	}
	/*!
	*@brief	更新。
	*@details
	* １フレームに一度呼び出してください。
	* 複数回呼び出すと、トリガー入力が取れなくなるよ！！！
	*/
	void Update();
	/*!
	 *@brief	ボタンのトリガー判定。
	 *@param[in]	button		調べたいボタン。enum EnButtonを参照。
	 *@return	trueが返ってきたらトリガー入力。
	 *@code
			//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
			if( g_pad.IsTrigger( enButtonA ) == true ){
				//Aボタンが押されたときの処理を記述。
				・
				・
				・
			}
	 *@endcode
	 */
	bool IsTrigger(EnButton button) const
	{
		return m_trigger[button] != 0;
	}
	/*!
	 *@brief	ボタンが押されているか判定。
	 *@param[in]	button		調べたいボタン。enum EnButtonを参照。
	 *@code
		//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
		if( g_pad.IsPress( enButtonA ) == true ){
			//Aボタンが押されたときの処理を記述する。
			・
			・
			・
		}
	 *@endcode
	 *@return	trueが返ってきたら押されている。
	 */
	bool IsPress(EnButton button) const
	{
		return m_press[button] != 0;
	}
	/*!
	*@brief	何かのボタンが押されているか判定
	*@return	trueが返ってきたら押されている。
	*/
	bool IsPressAnyKey() const
	{
		return IsPress(enButtonUp)
			|| IsPress(enButtonDown)
			|| IsPress(enButtonLeft)
			|| IsPress(enButtonRight)
			|| IsPress(enButtonA)
			|| IsPress(enButtonB)
			|| IsPress(enButtonX)
			|| IsPress(enButtonY)
			|| IsPress(enButtonSelect)
			|| IsPress(enButtonStart)
			|| IsPress(enButtonRB1)
			|| IsPress(enButtonRB2)
			|| IsPress(enButtonRB3)
			|| IsPress(enButtonLB1)
			|| IsPress(enButtonLB2)
			|| IsPress(enButtonLB3);
	}
	/*!
	*@brief	左スティックのX軸の入力量を取得。
	*@return	-1.0～1.0の正規化された値を返す。
	*@code
		//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
		//左スティックのX軸方向の入力を取得する。
		float lStickXF = g_pad.GetLStickXF();
	*@endcode
	*/
	float GetLStickXF() const
	{
		return m_lStickX;
	}
	/*!
	*@brief	左スティックのY軸の入力量を取得。
	*@return	-1.0～1.0の正規化された値を返す。
	*@code
		//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
		//左スティックのX軸方向の入力を取得する。
		float lStickYF = g_pad.GetLStickYF();
	*@endcode
	*/
	float GetLStickYF() const
	{
		return m_lStickY;
	}
	/*!
	*@brief	右スティックのX軸の入力量を取得。
	*@return	-1.0～1.0の正規化された値を返す。
	*@code
		//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
		//右スティックのX軸方向の入力を取得する。
		float rStickXF = g_pad.GetRStickXF();
	*@endcode
	*/
	float GetRStickXF() const
	{
		return m_rStickX;
	}
	/*!
	*@brief	右スティックのY軸の入力量を取得。
	*@return	-1.0～1.0の正規化された値を返す。
	*@code
		//使用例(g_padというPad型のグローバル変数が定義されているものとする。)
		//右スティックのY軸方向の入力を取得する。
		float rStickXF = g_pad.GetRStickYF();
	*@endcode
	*/
	float GetRStickYF() const
	{
		return m_rStickY;
	}
private:
	/*!
	*@brief	ボタンの入力情報を更新。
	*/
	void UpdateButtonInput();
	/*!
	*@brief	アナログスティックの入力情報を更新。
	*/
	void UpdateAnalogStickInput();
private:
	PAD_STATE m_state;	//!<パッドステート。
	int m_padNo = 0;			//!<パッド番号。
	int m_trigger[enButtonNum];	//!<トリガー入力のフラグ。
	int m_press[enButtonNum];	//!<press入力のフラグ。
	float m_lStickX = 0.0f;		//!<左スティックのX軸の入力量。
	float m_lStickY = 0.0f;		//!<左スティックのY軸の入力量。
	float m_rStickX = 0.0f;		//!<右スティックのX軸の入力量。
	float m_rStickY = 0.0f;		//!<右スティックのY軸の入力量。
};

//ゲームパッドのグローバル変数のextern宣言。
//extern宣言は変数の定義ではないので注意する！
extern Pad g_pad[ Pad::CONNECT_PAD_MAX ];
