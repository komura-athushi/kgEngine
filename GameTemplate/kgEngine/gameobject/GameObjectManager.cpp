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

void CGameObjectManager::Init(int gameObjectPrioMax)
{
	m_gameObjectPriorityMax = static_cast<GameObjectPrio>(gameObjectPrioMax);
	//resize�ŗv�f����ύX����
	m_GogameobjectList.resize(gameObjectPrioMax);
	m_DeletegameobjectList.resize(gameObjectPrioMax);
}