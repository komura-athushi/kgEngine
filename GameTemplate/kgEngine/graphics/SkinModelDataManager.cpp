#include "KGstdafx.h"
#include "graphics/SkinModelDataManager.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/Skeleton.h"

//g_skinModelDataManager�̎��́B
SkinModelDataManager g_skinModelDataManager;

DirectX::Model* SkinModelDataManager::Load(const wchar_t* filePath, const Skeleton& skeleton)
{
	DirectX::Model* retModel = NULL;
	//�{�[���𔭌������Ƃ��̃R�[���o�b�N�֐��B
	auto onFindBone = [&](
		const wchar_t* boneName,
		const VSD3DStarter::Bone* bone,
		std::vector<int>& localBoneIDtoGlobalBoneIDTbl
		) 
	{
		int globalBoneID = skeleton.FindBoneID(boneName);
		if (globalBoneID == -1) {
			//�{�[����������Ȃ������B
			return;
		}
		localBoneIDtoGlobalBoneIDTbl.push_back(globalBoneID);
	};
	//�}�b�v�ɓo�^����Ă��邩���ׂ�B
	auto it = m_directXModelMap.find(filePath);
	if (it == m_directXModelMap.end()) {
		//���o�^�Ȃ̂ŁA�V�K�Ń��[�h����B
		//�G�t�F�N�g�t�@�N�g���B
		SkinModelEffectFactory effectFactory(Engine().GetGraphicsEngine().GetD3DDevice());
		//�e�N�X�`��������t�H���_��ݒ肷��B
		effectFactory.SetDirectory(L"Assets/modelData");
		//CMO�t�@�C���̃��[�h�B
		auto model = DirectX::Model::CreateFromCMO(	//CMO�t�@�C�����烂�f�����쐬����֐��́ACreateFromCMO�����s����B
			Engine().GetGraphicsEngine().GetD3DDevice(),			//��������D3D�f�o�C�X�B
			filePath,									//�������͓ǂݍ���CMO�t�@�C���̃t�@�C���p�X�B
			effectFactory,								//��O�����̓G�t�F�N�g�t�@�N�g���B
			false,										//��l������Cull���[�h�B���͋C�ɂ��Ȃ��Ă悢�B
			false,
			onFindBone
		);
		retModel = model.get();
		//�V�K�Ȃ̂Ń}�b�v�ɓo�^����B
		m_directXModelMap.insert({ filePath, std::move(model) });
	}
	else {
		//�o�^����Ă���̂ŁA�ǂݍ��ݍς݂̃f�[�^���擾�B
		retModel = it->second.get();
	}
	return retModel;
}

void SkinModelDataManager::Release()
{
	//map����ɂ���B
	m_directXModelMap.clear();
}