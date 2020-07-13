#pragma once
#include "graphics/RenderTarget.h"


class SkinModel;
class CascadeShadowMap


{
public:
	CascadeShadowMap();
	/// <summary>
	/// ���C�g�r���[�s����擾
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightViewMatrix()
	{
		return m_lightViewMatrix;
	}
	/// <summary>
	/// ���C�g�v���W�F�N�V�����s����擾
	/// </summary>
	/// <returns></returns>
	CMatrix GetLightProjMatrix()
	{
		return m_lightProjMatrix;
	}
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	void UpdateLightViewProjMatrix(const int splitNumber);
	/// <summary>
	/// ���C�g�r���[�s��⃉�C�g�v���W�F�N�V�����s����X�V
	/// </summary>
	/// <param name="lightCameraPos"></param>
	/// <param name="lightCameraTarget"></param>
	void UpdateFromLightTaraget(const CVector3& lightCameraPos, const CVector3& lightCameraTarget);
	/// <summary>
	/// �X�V(���C�g�J�����̌������w�肷��o�[�W����)
	/// </summary>
	/// <param name="lightCameraPos"></param>
	/// <param name="lightDir"></param>
	void UpdateFromLightDirection(const CVector3& lightCameraPos, const CVector3& lightDir);
	/// <summary>
	/// �V���h�E�}�b�v�ɃV���h�E�L���X�^�[�������_�����O
	/// </summary>
	void RenderToShadowMap();
	/// <summary>
	/// �V���h�E�L���X�^�[��ǉ�
	/// </summary>
	/// <param name="shadowCaster"></param>
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCaters.push_back(shadowCaster);
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���擾
	/// </summary>
	/// <returns></returns>
	RenderTarget* GetRenderTarget(const int splitNumber, int number)
	{
		return &m_shadowMapRT[splitNumber][number];
	}
	/// <summary>
	/// ���C�g�̍���
	/// </summary>
	/// <param name="lightHeight"></param>
	void SetLightHeight(const float lightHeight)
	{
		m_lightHeight = lightHeight;
	}
	/// <summary>
	/// ���C�g�̕�����ݒ�
	/// </summary>
	/// <param name="lightDir"></param>
	void SetLightDirection(const CVector3& lightDir)
	{
		m_lightDir = lightDir;
	}
	/// <summary>
	/// ���C�g�r���[�v���W�F�N�V�����s����擾
	/// </summary>
	/// <returns></returns>
	CMatrix& GetLightViewProjMatrix(const int splitNumber)
	{
		return m_lightVieProjMatrix[splitNumber][m_shadowMapNumber];
	}
	CMatrix& GetLightViewProjMatrix(const int splitNumber, int number)
	{
		return m_lightVieProjMatrix[splitNumber][number];
	}
	/// <summary>
	/// �V���h�E�}�b�v�̔ԍ����擾�A0�����
	/// </summary>
	/// <returns></returns>
	int GetShadowMapNumber() const
	{
		return m_shadowMapNumber;
	}
	/// <summary>
	/// ���C�g�̕������擾
	/// </summary>
	/// <returns></returns>
	CVector3& GetLightDir()
	{
		return m_lightDir;
	}
	/// <summary>
	/// �e�V���h�E�}�b�v�̐[�x�l���擾
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	float GetFar(const int splitNumber, int number)
	{
		return m_farList[splitNumber][number];
	}
	/// <summary>
	/// �e�V���h�E�}�b�v�̃��C�g�r���[�̋t�s����擾
	/// </summary>
	/// <param name="number"></param>
	/// <returns></returns>
	CMatrix& GetLightViewInv(const int splitNumber,const int number)
	{
		return m_lightViewMatrixInv[splitNumber][number];
	}
	static const int SHADOWMAP_NUM = 3;
private:
	CVector3 m_lightCameraPosition = CVector3(300.0f, 300.0f, -300.0f);		//���C�g�J�����̍��W	
	CVector3 m_lightCamerataraget = CVector3(0.0f, 0.0f, 0.0f);		//���C�g�J�����̒����_
	CMatrix m_lightViewMatrix;			//���C�g�r���[�s��
	CMatrix m_lightProjMatrix;			//���C�g�v���W�F�N�V�����s��
	RenderTarget m_shadowMapRT[2][SHADOWMAP_NUM];			//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	CMatrix m_lightVieProjMatrix[2][SHADOWMAP_NUM];			//�e�V���h�E�}�b�v�̃r���[�v���W�F�N�V�����s��
	std::vector<SkinModel*> m_shadowCaters;		//�V���h�E�L���X�^�[�̃��X�g
	CVector3 m_lightDir = CVector3(-0.577f,-0.577f,-0.577f);
	float m_lightHeight = 400.0f;
	int m_shadowMapNumber = 0;
	float m_farList[2][SHADOWMAP_NUM] = {1.0f,1.0f,1.0f,1.0f};
	CMatrix m_lightViewMatrixInv[2][SHADOWMAP_NUM];
};


