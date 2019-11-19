#pragma once
#include "RenderTarget.h"
#include "Shader.h"
#include "GaussianBlur.h"

class PostEffect;
class Bloom
{
public:
	//�R���X�g���N�^
	Bloom();
	//�f�X�g���N�^
	~Bloom();
	//�h���[
	void Draw(PostEffect& postEffect);
private:
	//�A���t�@�u�����f�B���O�X�e�[�g������������
	void InitAlphaBlendState();
	//�V�F�[�_�[��������
	void InitShader();
	//�����_�����O�^�[�Q�b�g�̏�����
	void InitRenderTarget();
	//�T���v���X�e�[�g�̏�����
	void InitSamplerState();
private:
	static const int NUM_DOWN_SAMPLE = 4;				//�_�E���T���v�����O�̉񐔁B
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	ID3D11BlendState* m_disableBlendState = nullptr;	//�A���t�@�u�����f�B���O�𖳌��ɂ���u�����f�B���O�X�e�[�g�B
	ID3D11BlendState* m_finalBlendState = nullptr;		//�ŏI�����p�̃u�����f�B���O�X�e�[�g�B
	RenderTarget m_luminanceRT;			//�P�x�𒊏o���郌���_�����O�^�[�Q�b�g
	RenderTarget m_blurCombineRT;		//�u���[�̉摜���������郌���_�����O�^�[�Q�b�g�B
	Shader m_vs;						//�������Ȃ����_�V�F�[�_�[�B
	Shader m_psLuminance;				//�P�x���o�p�̃s�N�Z���V�F�[�_�[�B
	Shader m_psCombine;					//�ڂ����摜�����p�̃s�N�Z���V�F�[�_�[�B
	Shader m_psFinal;					//�ŏI�����p�̃s�N�Z���V�F�[�_�[�B
	GaussianBlur m_gausianBlur[NUM_DOWN_SAMPLE];	//�P�x���ڂ������߂̃K�E�V�A���u���[�B
													//1/2��1/4��1/8��1/16�̉𑜓x�܂Ń_�E���T���v�����O����Ă����܂��B
	ID3D11RasterizerState* m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
};

