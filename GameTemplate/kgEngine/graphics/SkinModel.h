#pragma once

#include "Skeleton.h"
#include "ToonMap.h"
const int NUM_DIRECTION_LIG = 4;
/*!
*@brief	�f�B���N�V�������C�g�B
*/
struct SDirectionLight {
	CVector4 direction[NUM_DIRECTION_LIG];		//���C�g�̕����B
	//CVector4 direction;
	CVector4 lightcolor[NUM_DIRECTION_LIG];			//���C�g�̃J���[�B
	CVector4 ambientlight = CVector4(0.3f, 0.3f, 0.3f, 1.0f);
	CVector3 eyePos;				//�J�����̎��_�B
	float specPow = 0.3f;			//�X�y�L�������C�g�̍i��B
	CVector3 m_eyeDir;
	int isToomShader = 1;
	CVector4 color = CVector4::White();
};

/*!
*@brief	FBX�̏�����B
*/
enum EnFbxUpAxis {
	enFbxUpAxisY,		//Y-up
	enFbxUpAxisZ,		//Z-up
};
/*!
*@brief	�X�L�����f���N���X�B
*/
class SkinModel
{
public:
	//���b�V�������������Ƃ��̃R�[���o�b�N�֐��B
	using OnFindMesh = std::function<void(const std::unique_ptr<DirectX::ModelMeshPart>&)>;
	SkinModel();
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~SkinModel();
	
	/*!
	*@brief	�������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*@param[in] enFbxUpAxis		fbx�̏㎲�B�f�t�H���g��enFbxUpAxisZ�B
	*/
	void Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis = enFbxUpAxisZ);
	//�C���X�^���V���O�`��̃f�[�^�̐���ݒ�
	void SetInstancingNumber(const int& numInstance)
	{
		m_maxInstance = numInstance;
	}
	//�C���X�^���V���O�`��̃f�[�^�̏�����
	void InitInstancingData();
	/*!
	*@brief	���f�������[���h���W�n�ɕϊ����邽�߂̃��[���h�s����X�V����B
	*@param[in]	position	���f���̍��W�B
	*@param[in]	rotation	���f���̉�]�B
	*@param[in]	scale		���f���̊g�嗦�B
	*/
	void UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale);
	/*!
	*@brief	�{�[���������B
	*@param[in]		boneName	�{�[���̖��O�B
	*@return	���������{�[���B������Ȃ������ꍇ��nullptr��Ԃ��܂��B
	*/
	Bone* FindBone(const wchar_t* boneName)
	{
		int boneId = m_skeleton.FindBoneID(boneName);
		return m_skeleton.GetBone(boneId);
	}
	/*!
	*@brief	���f����`��B
	*/
	void Draw(CMatrix viewMatrix, CMatrix projMatrix, EnRenderMode renderMode = enRenderMode_Normal);
	//���W�A��]�A�g����w�肵�ă��f�����h���[
	//void Draw(const CVector3& m_position, const CQuaternion& rot, const CVector3& scale, CMatrix viewMatrix, CMatrix projMatrix, EnRenderMode renderMode = enRenderMode_Normal);
	/*!
	*@brief	�X�P���g���̎擾�B
	*/
	Skeleton& GetSkeleton()
	{
		return m_skeleton;
	}
	/*!
	*@brief	���b�V������������B
	*@param[in] onFindMesh		���b�V�������������Ƃ��̃R�[���o�b�N�֐�
	*/
	void FindMesh(OnFindMesh onFindMesh) const
	{
		for (auto& modelMeshs : m_modelDx->meshes) {
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : modelMeshs->meshParts) {
				onFindMesh(mesh);
			}
		}
	}
	//�V���h�E�L���X�^�[��ݒ�
	void SetShadowCaster(bool caster)
	{
		m_isShadowCaster = caster;
	}
	//�V���h�E�L���X�^�[���ǂ���
	bool isShadowCaster() const
	{
		return m_isShadowCaster;
	}
	//�V���h�E���V�[�o�[��ݒ�
	void SetShadowReceiver(bool receiver)
	{
		m_isShadowReceiver = receiver;
	}
	//���[���h�s����擾
	CMatrix GetWorldMatrix()  const
	{
		return m_worldMatrix;
	}
	//���[���h�s���ݒ�
	void SetWorldMatrix(const CMatrix& worldmatirx)
	{
		m_worldMatrix = worldmatirx;
	}
	//�C���X�^���V���O�`��̐����擾
	int GetInstanceNumber()
	{
		return m_numInstance;
	}
	//�C���X�^���V���O�`������邩�ǂ������擾
	bool GetisInstancing()
	{
		return m_isInstancing;
	}
	/*!
	*@brief	SRV�̃��W�X�^�ԍ��B
	*/
	enum EnSkinModelSRVReg {
		enSkinModelSRVReg_DiffuseTexture = 0,		//!<�f�B�t���[�Y�e�N�X�`���B
		enSkinModelSRVReg_BoneMatrix,				//!<�{�[���s��B
	};
	bool IsFbxZUp() const
	{
		return m_enFbxUpAxis == enFbxUpAxisZ;
	}
	/// <summary>
	/// �C���X�^���V���O�p�f�[�^�̍X�V
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g��</param>
	/// <param name="enUpdateAxis">fbx�̏����</param>
	void UpdateInstancingData(
		const CVector3& pos,
		const CQuaternion& rot,
		const CVector3& scale,
		EnFbxUpAxis enUpdateAxis = enFbxUpAxisZ
	);
	//�C���X�^���V���O�p�f�[�^�̍X�V�A�s��𒼐ڐݒ肷��p
	void UpdateInstancingData(const CMatrix& worldMatrix);
	//�C���X�^���X�̐�������������A�C���X�^���V���O�f�[�^�̍X�V�O�ɌĂяo��
	void BeginUpdateInstancingData()
	{
		if (m_maxInstance >= 1) {
			m_numInstance = 0;
		}
	}
	//�C���X�^���V���O�f�[�^�̍X�V���I�������Ăяo���A�C���X�^���X�̐����m�肷��
	void EndUpdateInstancingData()
	{

	}
	/*!
	*@brief	�T���v���X�e�[�g�̏������B
	*/
	void InitSamplerState();
	/*!
	*@brief	�萔�o�b�t�@�̍쐬�B
	*/
	void InitConstantBuffer();
	/*!
	*@brief	�X�P���g���̏������B
	*@param[in]	filePath		���[�h����cmo�t�@�C���̃t�@�C���p�X�B
	*/
	void InitSkeleton(const wchar_t* filePath);
	//�g�D�[���V�F�[�_�[���I�t
	void SetOffToonShader()
	{
		m_isToonShader = false;
	}
	//�F��ݒ�
	void SetColor(const CVector4& color)
	{
		m_dirLight.color = color;
	}
private:
	//�萔�o�b�t�@�B
	struct SVSConstantBuffer {
		CMatrix mWorld;
		CMatrix mView;
		CMatrix mProj;
		CMatrix mLightView;		//todo ���C�g�r���[�s��B
		CMatrix mLightProj;		//todo ���C�g�v���W�F�N�V�����s��B
		int isShadowReciever;	//todo �V���h�E���V�[�o�[�̃t���O�B
	};
	EnFbxUpAxis			m_enFbxUpAxis = enFbxUpAxisZ;	//!<FBX�̏�����B
	ID3D11Buffer*		m_cb = nullptr;					//!<�萔�o�b�t�@�B
	Skeleton			m_skeleton;						//!<�X�P���g���B
	CMatrix				m_worldMatrix;					//!<���[���h�s��B
	DirectX::Model*		m_modelDx;						//!<DirectXTK���񋟂��郂�f���N���X�B
	ID3D11SamplerState* m_samplerState = nullptr;		//!<�T���v���X�e�[�g�B
	ID3D11Buffer* m_lightCb = nullptr;	//!<���C�g�p�̒萔�o�b�t�@�B
	ID3D11Buffer* m_isuvscroll = nullptr;
	ID3D11Buffer* m_uvscroll = nullptr;
	SDirectionLight	m_dirLight;				//!<�f�B���N�V�������C�g�B
	bool m_isShadowCaster = true;
	bool m_isShadowReceiver = true;
	std::unique_ptr<CMatrix[]>	m_instancingData;		//!<�C���X�^���V���O�`��p�̃f�[�^�B
	ID3D11Buffer* m_instancingDataSB = nullptr;					//!<�C���X�^���V���O�`��p�̒萔�o�b�t�@
	ID3D11ShaderResourceView* m_srv = nullptr;
	int m_maxInstance = 0;
	int m_numInstance = 0;
	bool m_isInstancing = false;
	int m_isUVscroll = true;
	float m_UVscroll = 0.0f;
	ToonMap* m_toonMap = nullptr;
	bool m_isToonShader = true;
};

