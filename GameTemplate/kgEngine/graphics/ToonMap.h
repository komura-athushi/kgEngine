#pragma once
//�g�D�[���V�F�[�_�[�p�̃e�N�X�`����ǂݍ��ނ�
class ToonMap
{
public: 
	ToonMap() {}
	~ToonMap() {}

	//������
	void Init();
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	//�g�D�[���V�F�[�_�[�p�̃e�N�X�`����ǂݍ���
	void InitTextuer();
	//SRV���擾
	ID3D11ShaderResourceView* GetSRV()
	{
		return m_srv;
	}
	//�T���v���X�e�[�g���擾
	ID3D11SamplerState* GetSamplerState()
	{
		return m_samplerState;
	}
	ID3D11ShaderResourceView* m_srv = nullptr;				//SRV
	ID3D11SamplerState* m_samplerState = nullptr;		//�g�D�[���V�F�[�_�[�p�̃T���v���X�e�[�g
};

