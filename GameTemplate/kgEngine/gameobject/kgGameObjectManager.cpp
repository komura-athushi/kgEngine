#include "KGstdafx.h"
#include "kgGameObjectManager.h"


void CGameObjectManager::Start()
{
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->StartWrapper();
		}
	}
}

void CGameObjectManager::PreUpdate()
{
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->PreUpdateWrapper();
		}
	}
}


void CGameObjectManager::Update()
{
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->UpdateWrapper();
		}
	}
}

void CGameObjectManager::PostRender()
{
	
	//DirectX::CommonStates state(Engine().GetGraphicsEngine().GetD3DDevice());
	//��Z�X�e�[�g��ݒ�
	/*Engine().GetGraphicsEngine().GetSpriteBatchPMA()->Begin(DirectX::SpriteSortMode_BackToFront, state.NonPremultiplied());*/
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->PostRenderWrapper();
		}
	}
	GameTime().Draw();
	/*Engine().GetGraphicsEngine().GetSpriteBatchPMA()->End();*/
	//Engine().GetGraphicsEngine().ResetLayerDepthCnt();
}

void CGameObjectManager::Draw()
{
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->DrawWrapper();
		}
	}
}

void CGameObjectManager::Delete()
{
	//�폜���Ă�����
	for (auto& GO : m_DeletegameobjectList) {
		GameObjectPrio prio = GO->GetPriority();
		GameObjectList& goExecList = m_GogameobjectList.at(prio);
		//�Q�[���I�u�W�F�N�g���X�g����Y���̃I�u�W�F�N�g�̉ӏ���T����
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//�Q�[���I�u�W�F�N�g�}�l�[�W���[��New����Ă�����delete����
		if (GO->IsNewFromgameObjectManager()) {
			//�폜
			delete (*it);
		}
		//�Q�[���I�u�W�F�N�g���X�g����폜
		goExecList.erase(it);
	}
	m_DeletegameobjectList.clear();
	
}

void CGameObjectManager::Init(int gameObjectPrioMax)
{
	m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
	//resize�ŗv�f����ύX����
	m_GogameobjectList.resize(gameObjectPrioMax);
}