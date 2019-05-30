#include "KGstdafx.h"
#include "GameObjectManager.h"


void CGameObjectManager::Start()
{
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->StartWrapper();
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
	for (GameObjectList objList : m_GogameobjectList) {
		for (IGameObject* obj : objList) {
			obj->PostRenderWrapper();
		}
	}
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
	////�Q�[���I�u�W�F�N�g���X�g����폜�\��̃I�u�W�F�N�g���폜����
	//for (GameObjectList& goList : m_GogameobjectList) {
	//	/*auto it = goList.begin();
	//	while (it != goList.end()) {
	//		if ((*it)->m_isDead) {
	//			it = goList.erase(it);
	//		}
	//		++it;
	//	}*/
	//	for (IGameObject* go : goList) {
	//		goList.erase(go);
	//	}
	//}
	for (auto& GO : m_DeletegameobjectList) {
		GameObjectPrio prio = GO->GetPriority();
		GameObjectList& goExecList = m_GogameobjectList.at(prio);
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		delete (*it);
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