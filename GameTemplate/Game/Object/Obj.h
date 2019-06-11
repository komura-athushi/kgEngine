#pragma once
#include "IMove.h"
class Obj : public IGameObject{
public:
	Obj();
	~Obj();
	bool Start() override;
	void Update() override;
	void Init(EnMove state ,const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity());
private:
	CSkinModelRender* m_skin = nullptr;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	const wchar_t* m_filepath = nullptr;
	EnMove m_state;
	IMove*  m_move = nullptr;

};
