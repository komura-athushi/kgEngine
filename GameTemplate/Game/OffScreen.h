/*****************************************************************//**
 * \file   OffScreen.h
 * \brief  OffScreen�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"
#include "graphics/2D/kgFont.h"

/**
 * \brief �E���̏����Ɋ������񂾃I�u�W�F�N�g��\������N���X.
 */
class OffScreen : public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	OffScreen();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~OffScreen() override final;
	/**
	 * \brief ����������.
	 *
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief �E���̏����ɕ\�����郂�f����ݒ�.
	 * 
	 * \param skinModel ���f��
	 */
	void SetSkinModel(SkinModel* skinModel)
	{
		m_skinModel = skinModel;
	}
	/**
	 * \brief �I�u�W�F�N�g�̃f�[�^��ݒ�.
	 * 
	 * \param objData �I�u�W�F�N�g�f�[�^
	 */
	void SetObjData(StructObjectData* objData)
	{
		m_objData = objData;
	}
private:
	/**
	 * \brief �T���v���X�e�[�g������.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief �J�����X�V����.
	 * 
	 */
	void UpdateCamera();
private:
	RenderTarget m_offRenderTarget;						//�����̃����_�[�^�[�Q�b�g
	OffScreenCamera m_offScreenCamera;					//�����p�̃J����
	SkinModel* m_skinModel = nullptr;					//�����ɕ\������p�̃��f��
	CVector3 m_position = CVector3::Zero();				//���W
	CVector3 m_scale = CVector3::One();					//�傫��
	CQuaternion m_rot = CQuaternion::Identity();		//��]
	Shader m_vs;										//�����Ȃ����_�V�F�[�_�[
	Shader m_ps;										//�����Ȃ����_�V�F�[�_�[
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	PostEffect m_postEffect;							//�|�X�g�G�t�F�N�g
	ID3D11RasterizerState* m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	float m_degree = 0.0f;								//��]�̊p�x
	StructObjectData* m_objData = nullptr;				//�I�u�W�F�N�g�f�[�^
	CSprite m_sprite;
	CFont m_font;																				//����
};

