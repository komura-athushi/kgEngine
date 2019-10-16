#pragma once
//�g�D�[���V�F�[�_�[�p�̃e�N�X�`����ǂݍ��ނ�
class ToonMap
{
private: 
	ToonMap() {
		Init();
	}
	~ToonMap() {}
public:
	static ToonMap& GetIntance()
	{
		static ToonMap instance;
		return instance;
	}
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
private:
	ID3D11ShaderResourceView* m_srv = nullptr;				//SRV
	ID3D11SamplerState* m_samplerState = nullptr;		//�g�D�[���V�F�[�_�[�p�̃T���v���X�e�[�g
};

