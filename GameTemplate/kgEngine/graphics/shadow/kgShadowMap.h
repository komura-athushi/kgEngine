#pragma once
#include "Noncopyable.h"
#include "graphics/RenderTarget.h"
class SkinModel;
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	//���C�g�r���[�s����擾
	CMatrix GetLightViewMatrix() const
	{
		return m_lightViewMatrix;
	}
	//���C�g�v���W�F�N�V�����s����擾
	CMatrix GetLightProjMatrix() const
	{
		return m_lightProjMatrix;
	}
	//���C�g�r���[�s��⃉�C�g�v���W�F�N�V�����s����X�V
	void UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget);
	//�X�V(���C�g�J�����̌������w�肷��o�[�W����)
	void UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir);
	//�V���h�E�}�b�v�ɃV���h�E�L���X�^�[�������_�����O
	void RenderToShadowMap();
	//�V���h�E�L���X�^�[��ǉ�
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	//�V���h�E�}�b�v��SRV���擾
	ID3D11ShaderResourceView* GetShadowMapSRV()
	{
		return m_shadowMapRT.GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition = {0.0f,1000.0f,500.0f};			//���C�g�J�����̎��_
	CVector3 m_lightCameraTarget = {0.0f,200.0f,0.0f};			//���C�g�J�����̒����_
	CMatrix m_lightViewMatrix = CMatrix::Identity();			//���C�g�r���[�s��
	CMatrix m_lightProjMatrix = CMatrix::Identity();			//���C�g�v���W�F�N�V�����s��
	RenderTarget m_shadowMapRT;								//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g
	std::vector<SkinModel*> m_shadowCasters;				//�V���h�E�L���X�^�[�̔z��
};

