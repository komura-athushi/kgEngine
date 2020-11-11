/*****************************************************************//**
 * \file   Collection.h
 * \brief  �R���N�V����
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"

class Fade;
/**
 * \brief �}�ӂ�\������N���X�B
 */
class Collection : public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Collection();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Collection() override final;
	/**
	 * \brief �X�V�O�̏���������.
	 * 
	 * \return true�ŏ��������I�����.
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void Draw() override final;
private:
	/**
	 * \brief �T���v���X�e�[�g������.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief �e�탂�f���̕`�揈��.
	 * 
	 */
	void OffScreenRender();
	/**
	 * \brief �J�[�\���̍X�V����.
	 * 
	 */
	void UpdateCursor();
	/**
	 * \brief �J�����̍X�V����.
	 * 
	 * \param skinModel�@�X�L�����f��
	 */
	void UpdateCamera(SkinModel_ObjData* data);
private:
	const static int W_NUMBER = 4;						//����
	const static int H_NUMBER = 4;						//�c��
	RenderTarget m_offRenderTarget;						//�I�t�X�N���[���̃����_�[�^�[�Q�b�g
	OffScreenCamera m_offScreenCamera;					//�I�t�X�N���[���p�̃J����
	CVector3 m_position = CVector3::Zero();				//���f���̍��W
	CVector3 m_scale = CVector3::One();					//�傫��
	CQuaternion m_rot = CQuaternion::Identity();		//��]
	Shader m_vs;										//�������Ȃ��V�F�[�_�[
	Shader m_ps;										//�������Ȃ��V�F�[�_�[
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	PostEffect m_postEffect[W_NUMBER * H_NUMBER];		//�|�X�g�G�t�F�N�g�A1�y�[�W�̃��f����
	ID3D11RasterizerState* m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
	const std::map<float, SkinModel_ObjDataPtr>* m_modelList = nullptr;	//���f���̃}�b�v
	float m_degree = 0.0f;								//���f���̉�]�̊p�x
	int m_page = 1;										//���y�[�W�ڂ�
	int m_listSize = 0;									//���f���̐�
	CSprite m_sprite;									//�摜
	CSprite m_haikei;									//�摜
	CSprite m_collectionCursor;							//�J�[�\��
	int m_cursorNumber = 1;								//�J�[�\�������Ԗڂ�
	PostEffect m_mainPostEffect;						//���̑傫����p�̃|�X�g�G�t�F�N�g
	int m_maximumPage = 1;								//�y�[�W�̍ő吔
	int m_mimmumPage = 1;								//�y�[�W�̍ŏ���
	int m_finalPageNumber = 0;							//�Ō�̃y�[�W
	CFont m_font;										//����
	int m_hitNumber = 0;								//�������񂾂��̂̐�
	SkinModel m_model;									//���[�p�̃��f��
	bool m_isWaitFadeout = false;						
	Fade* m_fade = nullptr;				
};

