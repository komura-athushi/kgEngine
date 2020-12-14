/*****************************************************************//**
 * \file   Sprite.h
 * \brief  Sprite�N���X
 * 
 * \author komurra
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/Shader.h"
/**
 * \brief �摜�`��.
 */
class CSprite {
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	CSprite();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~CSprite();
	/**
	 * \brief ����������.
	 * 
	 * \param fileName �摜�̃t�@�C���p�X
	 * \param isCircleGauge true�Ȃ�~�`�Q�[�W�ɂ���
	 */
	void Init(const wchar_t* fileName, bool isCircleGauge  = false);
	/**
	 * \brief ����������.
	 * 
	 * \param srv �V�F�[�_�[���\�[�X�r���[
	 */
	void Init(ID3D11ShaderResourceView* srv);
	/**
	 * \brief �X�N���[�����W�w��ŕ`��.
	 * 
	 * \param pos ���W�A(0.0f,0.0f)�ō���A�f�t�H���g�͒���
	 * \param scale �傫���A(1.0f,1.0f)��1�{
	 * \param pivot ���j�e�B��̃s�{�b�g
	 * \param rotation ��]
	 * \param color �����x
	 * \param effects �f�t�H���g��DirectX::SpriteEffects_None
	 * \param layerDepth �`�悷�鏇��(���͋@�\���Ă܂���B�B�B)
	 * \param degree �s�N�Z����\������p�x
	 * \param isIgnoreSplit ��ʕ����������ĉ摜�`��
	 */
	void DrawScreenPos(const CVector2& pos = { 1280.0f / 2, 720.0f / 2},
		const CVector2& scale = CVector2::One(),
		const CVector2& pivot = CVector2(0.5f,0.5f),
		float rotation = 0.0f,
		const CVector4 & color = CVector4::White(),
		DirectX::SpriteEffects effects = DirectX::SpriteEffects_None,
		float layerDepth = 0.5f,
		float degree = 0.0f,
		bool isIgnoreSplit = false
	);
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void Draw();
private:
	struct ConstantBuffer {
		CVector4 mulColor;
	};
	CVector2 m_screenSize = CVector2::Zero();
	DirectX::SpriteBatch* m_spriteBatch = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	ID3D11Resource* m_tex = nullptr;
	unsigned int m_width, m_height;
	ID3D11Buffer* m_cb = nullptr;							//!<�萔�o�b�t�@�B
	ID3D11Buffer* m_dg = nullptr;							//�萔�o�b�t�@(�p�x)
	Shader m_ps;
	float m_degree = 0.0f;
	bool m_isCircleGauge = false;							//�~�`�Q�[�W�ɂ��邩�ǂ���
	RECT m_sourceRectangle;									//�摜�T�C�Y
	bool m_isNormal = false;
};