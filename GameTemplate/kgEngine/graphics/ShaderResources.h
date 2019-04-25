/*!
 *@brief	�V�F�[�_�[���\�[�X�B
 *@Flyweight�p�^�[�����g�p�B
 */

#pragma once

#include "graphics/Shader.h"

/*!
 *@brief	�V�F�[�_�[���\�[�X
 *@details
 * �V���O���g���p�^�[���B
 * Flyweight�p�^�[����FlyweightFactory�N���X�B
 */
class ShaderResources{
	/*!
	*@brief	�R���X�g���N�^�B
	*/
	ShaderResources();
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~ShaderResources();
public:
	
	/*!
	*@brief	�V�F�[�_�[���\�[�X�̃C���X�^���X���擾�B
	*/
	static ShaderResources& GetInstance()
	{
		static ShaderResources instance;
		return instance;
	}
	/*!
	 *@brief	���[�h
	 *@param[out]	shader			���[�h�����V�F�[�_�[�̊i�[��B
	 *@param[out]	inputLayout		���̓��C�A�E�g�̊i�[��B
	 *@param[out]	blob
	 *@param[in]	filePath		���[�h����V�F�[�_�[�̃t�@�C���p�X�B
	 *@param[in]	entryFuncName	�G���g���[�|�C���g�̊֐����B
	 *@param[in]	shaderType		�V�F�[�_�[�^�C�v�B
	 */
	bool Load(
		void*& shader, 
		ID3D11InputLayout*& inputLayout, 
		ID3DBlob*& blob,
		const char* filePath, 
		const char* entryFuncName,
		Shader::EnType shaderType
	);
	/*!
	*@brief	�J���B
	*@details
	* �����I�ȃ^�C�~���O�ŊJ���������ꍇ�Ɏg�p���Ă��������B
	*/
	void Release();	
private:
	struct SShaderProgram {
		std::unique_ptr<char[]> program;
		int programSize;
	};
	struct SShaderResource{
		void* shader;					//!<�V�F�[�_�[�B
		ID3D11InputLayout* inputLayout;	//!<���̓��C�A�E�g�B
		Shader::EnType type;			//!<�V�F�[�_�[�^�C�v�B
		ID3DBlob* blobOut;
	};
	typedef std::unique_ptr<SShaderResource>	SShaderResourcePtr;
	typedef std::unique_ptr<SShaderProgram>	SShaderProgramPtr;
	std::map<int, SShaderProgramPtr>		m_shaderProgramMap;		//!<�ǂݍ��ݍς݂̃V�F�[�_�[�v���O�����̃}�b�v�B
	std::map<int, SShaderResourcePtr>	m_shaderResourceMap;	//!<�V�F�[�_�[���\�[�X�̃}�b�v�B
};

