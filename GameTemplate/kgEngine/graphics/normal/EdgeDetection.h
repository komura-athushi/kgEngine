#pragma once

#include "graphics/RenderTarget.h"
#include "graphics/Shader.h"
#include "graphics/PostEffect.h"

class PostEffect;
//�G�b�W���o���΂����肵�Ă������`
class EdgeDetection
{
public:
	EdgeDetection();
	~EdgeDetection();
	void EdgeRender(PostEffect& postEffect);
	
private:
	RenderTarget m_edgeMapRT;								//�@����`�悷�郌���_�����O�^�[�Q�b�g
	ID3D11SamplerState* m_samplerState = nullptr;		//�T���v���X�e�[�g�B
	Shader m_vsShader;
	Shader m_psShader;

};

