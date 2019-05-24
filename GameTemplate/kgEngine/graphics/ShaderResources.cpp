/*!
 *@brief	�V�F�[�_�[���\�[�X�B
 *@ Flyweight�p�^�[�����g�p�B
 */

#include "KGstdafx.h"
#include "graphics/ShaderResources.h"
#include "graphics/Shader.h"
#include "util/Util.h"

namespace {
	std::unique_ptr<char[]> ReadFile(const char* filePath, int& fileSize)
	{
		FILE* fp = fopen(filePath, "rb");
		fseek(fp, 0, SEEK_END);
		fpos_t fPos;
		fgetpos(fp, &fPos);
		fseek(fp, 0, SEEK_SET);
		fileSize = (int)fPos;
		std::unique_ptr<char[]> readBuff = std::make_unique<char[]>(fileSize);
		fread(readBuff.get(), fileSize, 1, fp);
		fclose(fp);
		return readBuff;
	}
	/*!
	*@brief	���_�V�F�[�_�[���璸�_���C�A�E�g�𐶐��B
	*/
	HRESULT CreateInputLayoutDescFromVertexShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
	{
		// �V�F�[�_�[��񂩂烊�t���N�V�������s���B
		ID3D11ShaderReflection* pVertexShaderReflection = NULL;
		if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
		{
			return S_FALSE;
		}

		// �V�F�[�_�[�����擾�B
		D3D11_SHADER_DESC shaderDesc;
		pVertexShaderReflection->GetDesc(&shaderDesc);

		// ���͏���`��ǂݍ���
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
		for (unsigned int i = 0; i< shaderDesc.InputParameters; i++)
		{
			D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
			pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

			// �G�������g��`��ݒ�B
			D3D11_INPUT_ELEMENT_DESC elementDesc;
			elementDesc.SemanticName = paramDesc.SemanticName;
			elementDesc.SemanticIndex = paramDesc.SemanticIndex;
			elementDesc.InputSlot = 0;
			elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			elementDesc.InstanceDataStepRate = 0;

			// determine DXGI format
			if (paramDesc.Mask == 1)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
			}
			else if (paramDesc.Mask <= 3)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
			}
			else if (paramDesc.Mask <= 7)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			}
			else if (paramDesc.Mask <= 15)
			{
				if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
				else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			}

			//�G�������g�̒�`��ۑ��B
			inputLayoutDesc.push_back(elementDesc);
		}

		// ���̓��C�A�E�g���쐬�B
		HRESULT hr = pD3DDevice->CreateInputLayout(
			&inputLayoutDesc[0],
			static_cast<UINT>(inputLayoutDesc.size()),
			pShaderBlob->GetBufferPointer(),
			pShaderBlob->GetBufferSize(),
			pInputLayout);

		//���t���N�V�����p�Ɋm�ۂ���������������B
		pVertexShaderReflection->Release();
		return hr;
	}
}
/*!
 *@brief	�R���X�g���N�^�B
 */
ShaderResources::ShaderResources()
{
}
/*!
 *@brief	�f�X�g���N�^�B
 */
ShaderResources::~ShaderResources()
{
	Release();
}
void ShaderResources::Release()
{
	for (auto it = m_shaderResourceMap.begin(); it != m_shaderResourceMap.end(); it++) {
		switch (it->second->type) {
		case Shader::EnType::VS:
			((ID3D11VertexShader*)it->second->shader)->Release();
			break;
		case Shader::EnType::PS:
			((ID3D11PixelShader*)it->second->shader)->Release();
			break;
		case Shader::EnType::CS:
			((ID3D11ComputeShader*)it->second->shader)->Release();
			break;
		}
		if (it->second->inputLayout) {
			it->second->inputLayout->Release();
		}
		it->second->blobOut->Release();
	}
	m_shaderResourceMap.clear();
	m_shaderProgramMap.clear();
}
bool ShaderResources::Load(
	void*& shader,
	ID3D11InputLayout*& inputLayout,
	ID3DBlob*& blob,
	const char* filePath, 
	const char* entryFuncName,
	Shader::EnType shaderType
)
{
	//�t�@�C���p�X����n�b�V���l���쐬����B
	int hash = Util::MakeHash(filePath);
	//�V�F�[�_�[�v���O���������[�h�ς݂����ׂ�B
	auto it = m_shaderProgramMap.find(hash);
	SShaderProgram* shaderProgram;
	if (it == m_shaderProgramMap.end()) {
		//�V�K�B
		SShaderProgramPtr prog = std::make_unique<SShaderProgram>();
		prog->program = ReadFile(filePath, prog->programSize);
		shaderProgram = prog.get();
		std::pair<int, SShaderProgramPtr> pair;
		pair.first = hash;
		pair.second = std::move(prog);
		m_shaderProgramMap.insert(std::move(pair));
		
	}
	else {
		//���łɓǂݍ��ݍς݁B
		shaderProgram = it->second.get();
	}

	//�����āA�V�F�[�_�[���R���p�C���ςݒ��ׂ�B
	static char buff[1024];
	strcpy(buff, filePath);
	strcat(buff, entryFuncName );
	//�t�@�C���p�X�{�G���g���[�|�C���g�̊֐����Ńn�b�V���l���쐬����B
	hash = Util::MakeHash(buff);
	auto itShaderResource = m_shaderResourceMap.find(hash);
	if (itShaderResource == m_shaderResourceMap.end()) {
		//�V�K�B
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
		/*if (strcmp(filePath, "Assets/shader/bloom.fx") == 0
		|| strcmp(filePath, "Assets/shader/model.fx") == 0) {
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
		}*/
#if BUILD_LEVEL == BUILD_LEVEL_DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		static const char* shaderModelNames[] = {
			"vs_5_0",
			"ps_5_0",
			"cs_5_0"
		};
		ID3DBlob* blobOut;
		ID3DBlob* errorBlob;

		hr = D3DCompile(shaderProgram->program.get(), shaderProgram->programSize, filePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryFuncName,
			shaderModelNames[(int)shaderType], dwShaderFlags, 0, &blobOut, &errorBlob);
		
		if (FAILED(hr))
		{
			if (errorBlob != nullptr) {
				static char errorMessage[10 * 1024];
				sprintf(errorMessage, "filePath : %s, %s", filePath, (char*)errorBlob->GetBufferPointer());
				MessageBox(NULL, errorMessage, "�V�F�[�_�[�R���p�C���G���[", MB_OK);
			}
			return false;
		}
		SShaderResourcePtr resource = std::make_unique<SShaderResource>();
		resource->inputLayout = nullptr;
		resource->type = shaderType;
		ID3D11Device* pD3DDevice = Engine().GetGraphicsEngine().GetD3DDevice();
		switch (shaderType) {
		case Shader::EnType::VS: {
			//���_�V�F�[�_�[�B
			hr = pD3DDevice->CreateVertexShader(blobOut->GetBufferPointer(), blobOut->GetBufferSize(), nullptr, (ID3D11VertexShader**)&resource->shader);
			if (FAILED(hr)) {
				return false;
			}
			//���̓��C�A�E�g���쐬�B
			hr = CreateInputLayoutDescFromVertexShaderSignature(blobOut, pD3DDevice, &resource->inputLayout);
			if (FAILED(hr)) {
				//���̓��C�A�E�g�̍쐬�Ɏ��s�����B
				return false;
			}
		}break;
		case Shader::EnType::PS: {
			//�s�N�Z���V�F�[�_�[�B
			hr = pD3DDevice->CreatePixelShader(blobOut->GetBufferPointer(), blobOut->GetBufferSize(), nullptr, (ID3D11PixelShader**)&resource->shader);
			if (FAILED(hr)) {
				return false;
			}
		}break;
		case Shader::EnType::CS: {
			//�R���s���[�g�V�F�[�_�[�B
			hr = pD3DDevice->CreateComputeShader(blobOut->GetBufferPointer(), blobOut->GetBufferSize(), nullptr, (ID3D11ComputeShader**)&resource->shader);
			if (FAILED(hr)) {
				return false;
			}
		}break;
		}
		resource->blobOut = blobOut;
		shader = resource->shader;
		inputLayout = resource->inputLayout;
		blob = blobOut;
		std::pair<int, SShaderResourcePtr> pair;
		pair.first = hash;
		pair.second = std::move(resource);
		m_shaderResourceMap.insert(std::move(pair));
		 
	}
	else {
		//���łɓǂݍ��ݍς݁B
		shader = itShaderResource->second->shader;
		inputLayout = itShaderResource->second->inputLayout;
		blob = itShaderResource->second->blobOut;
	}
	return true;
}
