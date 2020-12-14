/*****************************************************************//**
 * \file   ToonMap.h
 * \brief  ToomMap�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
/**
 * \brief �g�D�[���}�b�v��ǂݍ���.
 */
class ToonMap
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	ToonMap() {
		Init();
	}
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~ToonMap() {}
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static ToonMap& GetIntance()
	{
		static ToonMap instance;
		return instance;
	}
	/**
	 * \brief ����������.
	 * 
	 */
	void Init();
	/**
	 * \brief �T���v���X�e�[�g�̏�����.
	 * 
	 */
	void InitSamplerState();
	/**
	 * \brief �g�D�[���V�F�[�_�[�p�̃e�N�X�`����ǂݍ���.
	 * 
	 */
	void InitTextuer();
	/**
	 * \brief �V�F�[�_�[���\�[�X�r���[���擾.
	 * 
	 * \return �V�F�[�_�[���\�[�X�r���[
	 */
	ID3D11ShaderResourceView* GetSRV()
	{
		return m_srv;
	}
	/**
	 * \brief �T���v���X�e�[�g���擾.
	 * 
	 * \return �T���v���X�e�[�g
	 */
	ID3D11SamplerState* GetSamplerState()
	{
		return m_samplerState;
	}
private:
	ID3D11ShaderResourceView* m_srv = nullptr;				//SRV
	ID3D11SamplerState* m_samplerState = nullptr;		//�g�D�[���V�F�[�_�[�p�̃T���v���X�e�[�g
};

