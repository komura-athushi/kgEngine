#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"

class Fade;
class Collection : public IGameObject
{
public:
	Collection();
	~Collection();
	bool Start() override;
	void Draw() override;
private:
	//�T���v���X�e�[�g������
	void InitSamplerState();
	//���f���`�悵�Ă���
	void OffScreenRender();
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
	std::map<float, SkinModel_ObjData*> m_modelList;	//���f���̃}�b�v
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

