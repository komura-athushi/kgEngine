#pragma once
#include "Bloom.h"
//�|�X�g�G�t�F�N�g�N���X
class PostEffect
{
public:
	//�R���X�g���N�^
	PostEffect();
	//�f�X�g���N�^
	~PostEffect();
	//�X�V
	void Update();
	//�h���[
 	void Draw();
	//�t���X�N���[���`��
	void DrawFullScreenQuadPrimitive(ID3D11DeviceContext* deviceContext, Shader& vsShader, Shader& psShader);
private:
	//�t���X�N���[���`��p�̎l�p�`�v���~�e�B�u�̏�����
	void InitFullScreenQuadPrimitive();
	Bloom	m_bloom;  //�u���[��
	ID3D11Buffer* m_vertexBuffer = nullptr;		//���_�o�b�t�@�B
	ID3D11InputLayout* m_inputLayout = nullptr;
};

