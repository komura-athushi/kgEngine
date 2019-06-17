#include "KGstdafx.h"
#include "GameObject.h"
#include "GameObjectManager.h"
IGameObject::IGameObject() :
	m_priority(0),
	m_isStart(false),
	m_isDead(false),
	m_isNewFromGameObjectManager(false)
{
	GameObjectManager().AddGameObject(0, nullptr, this);
}

IGameObject::~IGameObject() {
	//ゲームオブジェクトマネージャでNewされてなかったらここでリストから削除する
	if (!this->m_isNewFromGameObjectManager) {
		GameObjectManager().DeleteList(this);
	}
}