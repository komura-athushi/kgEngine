#pragma once
#include "graphics/RenderTarget.h"
#include "OffScreenCamera.h"
#include "Object/ObjectData.h"
#include "graphics/2D/kgFont.h"

class OffScreen : public IGameObject
{
public:
	OffScreen();
	~OffScreen();
	bool Start() override;
	void PostRender() override;
	//�����ɕ\������悤�̃��f�����Z�b�g
	void SetSkinModel(SkinModel* skinModel)
	{
		m_skinModel = skinModel;
	}
	//�I�u�W�F�N�g�̃f�[�^���Z�b�g
	void SetObjData(StructObjectData* objData)
	{
		m_objData = objData;
	}
private:
	//�T���v���X�e�[�g������
	void InitSamplerState();
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

