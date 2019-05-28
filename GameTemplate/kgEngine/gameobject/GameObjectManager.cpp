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
	//ゲームオブジェクトリストから削除予定のオブジェクトを削除する
	for (GameObjectList& goList : m_GogameobjectList) {
		auto it = goList.begin();
		while (it != goList.end()) {
			if ((*it)->m_isDead) {
				it = goList.erase(it);
			}
			++it;
		}
	}
	for (auto& GO : m_DeletegameobjectList) {
		delete GO;
	}
	m_DeletegameobjectList.clear();
	
}

void CGameObjectManager::Init(int gameObjectPrioMax)
{
	m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
	//resizeで要素数を変更する
	m_GogameobjectList.resize(gameObjectPrioMax);
	m_DeletegameobjectList.resize(gameObjectPrioMax);
}