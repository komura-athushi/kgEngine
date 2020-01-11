#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
class NormalMap;
//�G�b�W���o���΂����肵�Ă������`
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();
	//�G�b�W���o
	void EdgeRender(PostEffect& postEffect);
	//�������Ă���
	void Draw(PostEffect& postEffect ,RenderTarget* renderTarget = nullptr);
	void InitGaussian(NormalMap* normalMap);
private:
	RenderTarget m_edgeMapRT;								//�@����`�悷�郌���_�����O�^�[�Q�b�g
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	Shader m_vsShader;
	Shader m_psShader;
	Shader m_vs;						//�������Ȃ����_�V�F�[�_�[�B
	Shader m_psFinal;					//�ŏI�����p�̃s�N�Z���V�F�[�_�[�B
	ID3D11BlendState* m_finalBlendState = nullptr;		//�ŏI�����p�̃u�����f�B���O�X�e�[�g�B
	ID3D11BlendState* m_disableBlendState = nullptr;	//�A���t�@�u�����f�B���O�𖳌��ɂ���u�����f�B���O�X�e�[�g�B
	GaussianBlur m_gaussianBlur;
};

