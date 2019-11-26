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
	//�t���X�N���[���`��p�̎l�p�`�v���~�e�B�u�̏�����
	void InitFullScreenQuadPrimitive();
	//�`��p�̎l�p�`�v���~�e�B�u�̏�����
	void InitScreenQuadPrimitive(CVector2 pos1, const CVector2& pos2, const CVector2& pos3, const CVector2& pos4);
private:
	Bloom	m_bloom;  //�u���[��
	ID3D11Buffer* m_vertexBuffer = nullptr;		//���_�o�b�t�@�B
	ID3D11InputLayout* m_inputLayout = nullptr;
	bool m_isInit = false;
};

