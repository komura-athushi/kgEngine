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
	//0�ň�Ԗ�
	CMatrix GetLightViewMatrix(const int number = 0) const
	{
		return m_lightViewMatrix[number];
	}
	//���C�g�v���W�F�N�V�����s����擾
	//0�ň�Ԗ�
	CMatrix GetLightProjMatrix(const int number = 0) const
	{
		return m_lightProjMatrix[number];
	}
	//���C�g�r���[�s��⃉�C�g�v���W�F�N�V�����s����X�V
	//0�ň�Ԗ�
	void UpdateFromLightTarget(const CVector3& lightCameraPos, CVector3 lightCameraTarget , const int number = 0);
	//�X�V(���C�g�J�����̌������w�肷��o�[�W����)
	//0�ň�Ԗ�
	void UpdateFromLightDirection(const CVector3& lightCameraPos,const CVector3& lightDir , const int number = 0);
	//�V���h�E�}�b�v�ɃV���h�E�L���X�^�[�������_�����O
	void RenderToShadowMap();
	//�V���h�E�L���X�^�[��ǉ�
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCasters.push_back(shadowCaster);
	}
	//�V���h�E�}�b�v��SRV���擾
	//0�ň�Ԗ�
	ID3D11ShaderResourceView* GetShadowMapSRV(const int number = 0)
	{
		return m_shadowMapRT[number].GetRenderTargetSRV();
	}
private:
	CVector3 m_lightCameraPosition[2] = { {0.0f,1000.0f,500.0f} ,{0.0f,1000.0f,500.0f} };			//���C�g�J�����̎��_
	CVector3 m_lightCameraTarget[2] = { {0.0f,200.0f,0.0f} ,{0.0f,200.0f,0.0f} };			//���C�g�J�����̒����_
	CMatrix m_lightViewMatrix[2] = { CMatrix::Identity() ,CMatrix::Identity()};			//���C�g�r���[�s��
	CMatrix m_lightProjMatrix[2] = { CMatrix::Identity() , CMatrix::Identity()};			//���C�g�v���W�F�N�V�����s��
	RenderTarget m_shadowMapRT[2];								//�V���h�E�}�b�v��`�悷�郌���_�����O�^�[�Q�b�g
	std::vector<SkinModel*> m_shadowCasters;				//�V���h�E�L���X�^�[�̔z��
};

