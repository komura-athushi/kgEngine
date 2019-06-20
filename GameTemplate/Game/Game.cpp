#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Ground.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "Object/Move/IMove.h"
#include "Object/Rotation/IRot.h"
#include "Object/ObjectData.h"
#include "Moji.h"
Game::Game()
{

}

Game::~Game()
{
	DeleteGO(m_ground);
	DeleteGO(m_gamecamera);
	DeleteGO(m_player);
	for (Obj* obj : m_objList) {
		DeleteGO(obj);
	}
}

bool Game::Start()
{
	ObjectData::GetInstance();
	m_gamecamera = NewGO<GameCamera>(0);
	m_level.Init(L"Assets/level/level00.tkl", [&](LevelObjectData& objdata) {
		if (objdata.ForwardMatchName(L"o")) {
			for (int i = 0; i < ObjectData::GetInstance().GetListSize(); i++ ) {
				if (objdata.ForwardMatchName(ObjectData::GetInstance().GetObjectData(i)->s_name)) {
					Obj* obj = NewGO<Obj>(0);
					obj->SetObjData(ObjectData::GetInstance().GetObjectData(i));
					MOVESTATUS ms = FindMove(objdata.name);
					obj->InitMove(ms.s_state, objdata.position, ms.s_move, ms.s_limit, objdata.rotation);
					ROTSTATUS rs = FindRot(objdata.name);
					obj->InitRot(rs.s_state, rs.s_speed);
					obj->SetFilePath(objdata.name);
					m_objList.push_back(obj);
					return true;
				}
			}
			return true;
		}
		else if (objdata.EqualObjectName(L"sphere")) {
			Player* m_player = NewGO<Player>(0);
			m_player->SetPosition(objdata.position);
			return true;
		}
		else if (objdata.EqualObjectName(L"ground")) {
			Ground* m_ground = NewGO<Ground>(0);
			m_ground->SetPosition(objdata.position);
			return true;
		}
		return true;
	});
	return true;
}

void Game::Update()
{
	
}