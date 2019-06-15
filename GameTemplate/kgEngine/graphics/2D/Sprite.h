#pragma once
#include "graphics/Shader.h"
//�X�v���C�g
class CSprite {
public:
	//�f�t�H���g��UV���W
	static const CVector2 DEFAULT_PIVOT;
	//�R���X�g���N�^
	CSprite();
	//�f�X�g���N�^
	~CSprite();
	/*!
	* @brief	�������B
	*@param[in]	filePath	�t�@�C���p�X
	*@param[in]	w		���B
	*@param[in]	h		�����B
	*/
private:
	struct SSpriteCB {
		CMatrix WVP;				//���[���h�r���[�v���W�F�N�V�����s��
		CVector4 mulColor;			//��Z�J���[
	};
		CVector3 _position = CVector3::Zero();
		CQuaternion m_rotation = CQuaternion::Identity();
		CVector3 m_scale = CVector3::One();
		CMatrix m_world = CMatrix::Identity();
		Shader* m_ps;
		Shader* m_vs;
		CVector4 m_mulcolor = CVector4::White();


};