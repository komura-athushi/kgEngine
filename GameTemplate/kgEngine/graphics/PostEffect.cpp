#include "KGstdafx.h"
#include "PostEffect.h"

PostEffect::PostEffect()
{
	//�t���X�N���[���`��̂��߂̎l�p�`�v���~�e�B�u���������B
	InitFullScreenQuadPrimitive();
}


PostEffect::~PostEffect()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
}

void PostEffect::Update()
{
}

void PostEffect::Draw()
{
	m_bloom.Draw(*this);
}

struct SVertex {
	float position[4];		//���_���W�B
	float uv[2];			//UV���W�B���ꂪ�e�N�X�`�����W
};
void PostEffect::InitFullScreenQuadPrimitive()
{
	//���_�o�b�t�@���������B
	SVertex vertex[4] = {
		//���_�P
		{
			//���W�@position[4]
			-1.0f,  -1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 1.0f
		},
		//���_�Q
		{
			//���W�@position[4]
			1.0f, -1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 1.0f
		},
		//���_�R
		{
			//���W�@position[4]
			-1.0f,  1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			0.0f, 0.0f
		},
		//���_�S
		{
			//���W�@position[4]
			1.0f,  1.0f, 0.0f, 1.0f,
			//UV���W uv[2]
			1.0f, 0.0f
		},
	};
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));				//�\���̂�0�ŏ���������B
	bd.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݂���я������݂̕��@�B
												//��肠������D3D11_USAGE_DEFAULT�ł悢�B
	bd.ByteWidth = sizeof(vertex);				//���_�o�b�t�@�̃T�C�Y�B���_�̃T�C�Y�~���_���ƂȂ�B
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	//���ꂩ��쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertex;

	//���_�o�b�t�@�̍쐬�B
	Engine().GetGraphicsEngine().GetD3DDevice()->CreateBuffer(&bd, &InitData, &m_vertexBuffer);
}

void PostEffect::DrawFullScreenQuadPrimitive(ID3D11DeviceContext* deviceContext, Shader& vsShader, Shader& psShader)
{
	//�v���~�e�B�u�̃g�|���W�[�Ƃ��āA�g���C�A���O���X�g���b�v��ݒ肷��B
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	unsigned int vertexSize = sizeof(SVertex);
	unsigned int offset = 0;
	//�P�x���o�p�̃V�F�[�_�[��ݒ肷��B
	deviceContext->VSSetShader(
		(ID3D11VertexShader*)vsShader.GetBody(), nullptr, 0
	);
	deviceContext->PSSetShader(
		(ID3D11PixelShader*)psShader.GetBody(), nullptr, 0
	);
	deviceContext->IASetInputLayout(vsShader.GetInputLayout());

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	deviceContext->Draw(4, 0);
}