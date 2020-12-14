/*****************************************************************//**
 * \file   CascadeShadowMap.h
 * \brief  CascadeShadowMap�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "graphics/RenderTarget.h"


class SkinModel;
/**
 * \brief �J�X�P�[�h�V���h�E�}�b�v�𐶐�����N���X.
 */
class CascadeShadowMap


{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	CascadeShadowMap();
	/**
	 * \brief �X�V.
	 * 
	 */
	void Update();
	/**
	 * \brief �V���h�E�}�b�v�𐶐�.
	 * 
	 */
	void RenderToShadowMap();
	/**
	 * \brief �V���h�E�L���X�^�[��ǉ�.
	 * 
	 * \param shadowCaster �X�L�����f��
	 */
	void RegistShadowCaster(SkinModel* shadowCaster)
	{
		m_shadowCaters.push_back(shadowCaster);
	}

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���擾
	/// </summary>
	/// <returns></returns>
	/**
	 * \brief �����_�����O�^�[�Q�b�g���擾.
	 * 
	 * \param splitNumber ��ʕ����̔ԍ�
	 * \param number �V���h�E�}�b�v�̔ԍ�
	 * \return �����_�����O�^�[�Q�b�g
	 */
	RenderTarget* GetRenderTarget(const int splitNumber,const int number)
	{
		return &m_shadowMapRT[splitNumber][number];
	}
	/// <summary>
	/// ���C�g�r���[�v���W�F�N�V�����s����擾
	/// </summary>
	/// <returns></returns>
	/**
	 * \brief ���C�g�r���[�v���W�F�N�V�����s����擾.
	 * 
	 * \param splitNumber ��ʕ����̔ԍ�
	 * \return ���C�g�r���[�v���W�F�N�V�����s��
	 */
	const CMatrix& GetLightViewProjMatrix(const int splitNumber) const
	{
		return m_lightVieProjMatrix[splitNumber][m_shadowMapNumber];
	}
	/**
	 * \brief ���C�g�r���[�v���W�F�N�V�����s����擾.
	 * 
	 * \param splitNumber ��ʕ����̔ԍ�
	 * \param number �V���h�E�}�b�v�̔ԍ�
	 * \return ���C�g�r���[�v���W�F�N�V�����s��
	 */
	const CMatrix& GetLightViewProjMatrix(const int splitNumber,const int number) const
	{
		return m_lightVieProjMatrix[splitNumber][number];
	}
	/**
	 * \brief �V���h�E�}�b�v�̔ԍ����擾(1�Ԗڂ�0).
	 * 
	 * \return �V���h�E�}�b�v�̔ԍ�
	 */
	const int GetShadowMapNumber() const
	{
		return m_shadowMapNumber;
	}
	/**
	 * \brief �V���h�E�}�b�v�̐[�x�l���擾.
	 * 
	 * \param splitNumber ��ʕ����̔ԍ�
	 * \param number �V���h�E�}�b�v�̔ԍ�
	 * \return �[�x�l
	 */
	const float GetFar(const int splitNumber,const int number) const
	{
		return m_farList[splitNumber][number];
	}
	static const int SHADOWMAP_NUM = 3;
private:
	/**
	 * \brief ���C�g�r���[�v���W�F�N�V�����s����v�Z.
	 * 
	 * \param splitNumber ��ʕ����̔ԍ�
	 */
	void UpdateLightViewProjMatrix(const int splitNumber);
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


