/*****************************************************************//**
 * \file   EdgeDetection.h
 * \brief  EdgeDitection�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
class NormalMap;
/**
 * \brief �G�b�W���o�ƃG�b�W����������N���X.
 */
class EdgeDetection
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	EdgeDetection();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~EdgeDetection();
	/**
	 * \brief �G�b�W���o.
	 * 
	 * \param postEffect �|�X�g�G�t�F�N�g
	 */
	void EdgeRender(PostEffect& postEffect);
	/**
	 * \brief �G�b�W����.
	 * 
	 * \param postEffect �|�X�g�G�t�F�N�g
	 * \param renderTarget ������̃����_�����O�^�[�Q�b�g
	 */
	void Draw(PostEffect& postEffect ,RenderTarget* renderTarget = nullptr);
	//�K�E�X�t�B���^�[�̏����������Ǎ��͕K�v�Ȃ���
	void InitGaussian(NormalMap* normalMap);
private:
	RenderTarget m_edgeMapRT;							//�@����`�悷�郌���_�����O�^�[�Q�b�g
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	Shader m_vsShader;									//���_���o�p�̒��_�V�F�[�_�[	
	Shader m_psShader;									//���_���o�p�̃s�N�Z���V�F�[�_�[
	Shader m_vs;										//�������Ȃ����_�V�F�[�_�[�B
	Shader m_psFinal;									//�ŏI�����p�̃s�N�Z���V�F�[�_�[�B
	ID3D11BlendState* m_finalBlendState = nullptr;		//�ŏI�����p�̃u�����f�B���O�X�e�[�g�B
	ID3D11BlendState* m_disableBlendState = nullptr;	//�A���t�@�u�����f�B���O�𖳌��ɂ���u�����f�B���O�X�e�[�g�B
	GaussianBlur m_gaussianBlur;						//�K�E�X�t�B���^�[�A���͎g���ĂȂ�
};

