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
	//削除していくぅ
	for (auto& GO : m_DeletegameobjectList) {
		GameObjectPrio prio = GO->GetPriority();
		GameObjectList& goExecList = m_GogameobjectList.at(prio);
		//ゲームオブジェクトリストから該当のオブジェクトの箇所を探して
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//ゲームオブジェクトマネージャーでNewされていたらdeleteする
		if (GO->IsNewFromgameObjectManager()) {
			//削除
			delete (*it);
		}
		//ゲームオブジェクトリストから削除
		goExecList.erase(it);
	}
	m_DeletegameobjectList.clear();
	
}

void CGameObjectManager::Init(int gameObjectPrioMax)
{
	m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
	//resizeで要素数を変更する
	m_GogameobjectList.resize(gameObjectPrioMax);
}