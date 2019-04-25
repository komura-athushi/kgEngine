/*!
 *@brief	�p�b�h
 */

#pragma once

 //XInput�̃w�b�_�[�t�@�C���B
#include <Xinput.h>

/*!
 *@brief	���z�{�^����`�B
 */
enum EnButton{
	enButtonUp,		//!<��B
	enButtonDown,		//!<���B
	enButtonLeft,		//!<���B
	enButtonRight,		//!<�E�B
	enButtonA,			//!<A�{�^���B
	enButtonB,			//!<B�{�^���B
	enButtonX,			//!<X�{�^���B
	enButtonY,			//!<Y�{�^���B
	enButtonSelect,	//!<�Z���N�g�{�^���B
	enButtonStart,		//!<�X�^�[�g�{�^���B
	enButtonRB1,		//!<RB1�{�^���B
	enButtonRB2,		//!<RB2�{�^���B
	enButtonRB3,		//!<RB3�{�^���B
	enButtonLB1,		//!<LB1�{�^���B
	enButtonLB2,		//!<LB2�{�^���B
	enButtonLB3,		//!<LB3�{�^���B
	enButtonNum,	//!<�{�^���̐��B
};
/*!
 *@brief	�p�b�h
 */
class Pad{
public:
	static const int CONNECT_PAD_MAX = 4;		//�ڑ��\�ȃp�b�h�̍ő吔�B
	/*!
	 *@brief	�p�b�h�X�e�[�g�B
	 */
	struct PAD_STATE
	{
		XINPUT_STATE xInputState;	//XInputGetState���g�p���āA�擾����p�b�h�̓��͏󋵁B
		bool bConnected;
	};
	/*!
	 *@brief	�R���X�g���N�^�B
	 */
	Pad();
	/*!
	 *@brief	�f�X�g���N�^�B
	 */
	~Pad();
	/*!
	*@brief	�������B
	*@param[in]	padNo	�p�b�h�ԍ��B
	*/
	void Init(int padNo)
	{
		m_padNo = padNo;
	}
	/*!
	*@brief	�X�V�B
	*@details
	* �P�t���[���Ɉ�x�Ăяo���Ă��������B
	* ������Ăяo���ƁA�g���K�[���͂����Ȃ��Ȃ��I�I�I
	*/
	void Update();
	/*!
	 *@brief	�{�^���̃g���K�[����B
	 *@param[in]	button		���ׂ����{�^���Benum EnButton���Q�ƁB
	 *@return	true���Ԃ��Ă�����g���K�[���́B
	 *@code
			//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
			if( g_pad.IsTrigger( enButtonA ) == true ){
				//A�{�^���������ꂽ�Ƃ��̏������L�q�B
				�E
				�E
				�E
			}
	 *@endcode
	 */
	bool IsTrigger(EnButton button) const
	{
		return m_trigger[button] != 0;
	}
	/*!
	 *@brief	�{�^����������Ă��邩����B
	 *@param[in]	button		���ׂ����{�^���Benum EnButton���Q�ƁB
	 *@code
		//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
		if( g_pad.IsPress( enButtonA ) == true ){
			//A�{�^���������ꂽ�Ƃ��̏������L�q����B
			�E
			�E
			�E
		}
	 *@endcode
	 *@return	true���Ԃ��Ă����牟����Ă���B
	 */
	bool IsPress(EnButton button) const
	{
		return m_press[button] != 0;
	}
	/*!
	*@brief	�����̃{�^����������Ă��邩����
	*@return	true���Ԃ��Ă����牟����Ă���B
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
	*@brief	���X�e�B�b�N��X���̓��͗ʂ��擾�B
	*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
	*@code
		//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
		//���X�e�B�b�N��X�������̓��͂��擾����B
		float lStickXF = g_pad.GetLStickXF();
	*@endcode
	*/
	float GetLStickXF() const
	{
		return m_lStickX;
	}
	/*!
	*@brief	���X�e�B�b�N��Y���̓��͗ʂ��擾�B
	*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
	*@code
		//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
		//���X�e�B�b�N��X�������̓��͂��擾����B
		float lStickYF = g_pad.GetLStickYF();
	*@endcode
	*/
	float GetLStickYF() const
	{
		return m_lStickY;
	}
	/*!
	*@brief	�E�X�e�B�b�N��X���̓��͗ʂ��擾�B
	*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
	*@code
		//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
		//�E�X�e�B�b�N��X�������̓��͂��擾����B
		float rStickXF = g_pad.GetRStickXF();
	*@endcode
	*/
	float GetRStickXF() const
	{
		return m_rStickX;
	}
	/*!
	*@brief	�E�X�e�B�b�N��Y���̓��͗ʂ��擾�B
	*@return	-1.0�`1.0�̐��K�����ꂽ�l��Ԃ��B
	*@code
		//�g�p��(g_pad�Ƃ���Pad�^�̃O���[�o���ϐ�����`����Ă�����̂Ƃ���B)
		//�E�X�e�B�b�N��Y�������̓��͂��擾����B
		float rStickXF = g_pad.GetRStickYF();
	*@endcode
	*/
	float GetRStickYF() const
	{
		return m_rStickY;
	}
private:
	/*!
	*@brief	�{�^���̓��͏����X�V�B
	*/
	void UpdateButtonInput();
	/*!
	*@brief	�A�i���O�X�e�B�b�N�̓��͏����X�V�B
	*/
	void UpdateAnalogStickInput();
private:
	PAD_STATE m_state;	//!<�p�b�h�X�e�[�g�B
	int m_padNo = 0;			//!<�p�b�h�ԍ��B
	int m_trigger[enButtonNum];	//!<�g���K�[���͂̃t���O�B
	int m_press[enButtonNum];	//!<press���͂̃t���O�B
	float m_lStickX = 0.0f;		//!<���X�e�B�b�N��X���̓��͗ʁB
	float m_lStickY = 0.0f;		//!<���X�e�B�b�N��Y���̓��͗ʁB
	float m_rStickX = 0.0f;		//!<�E�X�e�B�b�N��X���̓��͗ʁB
	float m_rStickY = 0.0f;		//!<�E�X�e�B�b�N��Y���̓��͗ʁB
};

//�Q�[���p�b�h�̃O���[�o���ϐ���extern�錾�B
//extern�錾�͕ϐ��̒�`�ł͂Ȃ��̂Œ��ӂ���I
extern Pad g_pad[ Pad::CONNECT_PAD_MAX ];
