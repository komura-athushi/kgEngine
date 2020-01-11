#include "stdafx.h"
#include "Obj.h"
#include "Move/MoveLR.h"
#include "Move/MoveUp.h"
#include "Move/MovePath.h"
#include "Rotation/IRot.h"
#include "Rotation/RotSelf.h"
#include "Rotation/RotDirection.h"
#include "Player.h"
#include "GameData.h"
#include "OffScreen.h"
#include "sound/SoundSource.h"


ObjModelDataFactory::ObjModelDataFactory()
{

}

bool ObjModelDataFactory::Start()
{
	return true;
}

void ObjModelDataFactory::PreUpdate()
{
	BeginUpdateInstancingData();
}

ObjModelData* ObjModelDataFactory::Load(const wchar_t* path) 
{
	int key = Util::MakeHash(path);
	if (m_modelmap.count(key) == 0) {
		wchar_t filepath[256];
		swprintf_s(filepath, L"Resource/modelData/%ls.cmo", path);
		//std::make_unique  スマートポインタ生成のヘルパー関数
		//newが不要になる、()に初期値
		m_modelmap.emplace(key, std::make_unique<ObjModelData>());
		m_modelmap[key].get()->s_skinmodel.Init(filepath, nullptr, 0, enFbxUpAxisZ, true);
		SetPriorityGO(&m_modelmap[key].get()->s_skinmodel, 2);
		m_modelmap[key].get()->s_hashKey = key;
		SkinModel* skinModel = new SkinModel();
		skinModel->Init(filepath);
		m_skinModelmap.emplace(key, skinModel);
	}
	m_modelmap[key].get()->s_maxInstance += 1;
	m_modelmap[key].get()->s_skinmodel.SetInstanceNumber(m_modelmap[key].get()->s_maxInstance);
	//getでマッピングされている値
	return m_modelmap[key].get();
}

void ObjModelDataFactory::InitInstancingData()
{
	//各モデルのインスタンシングデータを初期化
	if (m_modelmap.size() != 0) {
		for (auto itr = m_modelmap.begin(); itr != m_modelmap.end(); ++itr) {
			itr->second.get()->s_skinmodel.InitInstancing();
		}
	}
}

void ObjModelDataFactory::BeginUpdateInstancingData()
{
	//インスタンシングを開始するぞい
	if (m_modelmap.size() != 0) {
		for (auto itr = m_modelmap.begin(); itr != m_modelmap.end(); ++itr) {
			itr->second.get()->s_skinmodel.BeginUpdateInstancingData();
		}
	}
}

void ObjModelDataFactory::DeleteAllData()
{
	//マップクリアー
	m_modelmap.clear();
}

Obj::Obj()
{

}

Obj::~Obj()
{
	if (m_move != nullptr) {
		delete m_move;
	}
	if (m_rot != nullptr) {
		delete m_rot;
	}
	if (m_gamedata->GetScene() == enScene_Result && m_movestate == enMove_MoveHit) {
		ObjectData::GetInstance().SetisHit(m_objdata->s_volume);
	}
}

void Obj::SetFilePath(const wchar_t* path)
{
	m_modeldata = GetObjModelDataFactory().Load(path);
	if (m_objdata->s_isanimation == 1) {
		m_anim.Init(m_objdata->s_name, &m_modeldata->s_skinmodel);
	}
}

bool Obj::Start()
{
	m_size = (m_objdata->s_x + m_objdata->s_y + m_objdata->s_z) / 3;
	if (m_objdata->s_issphere == 1) {
		//球体である
		m_issphere = true;
		m_size = m_objdata->s_x;
		m_lenght = m_size * 2;
		m_radius = m_objdata->s_x;
		m_staticobject.CreateSphereObject(m_size, m_position, m_rotation);
	}
	//メッシュコライダーじゃない？
	else if(m_objdata->s_isMeshCollider == 0) {
		m_staticobject.CreateBoxObject(m_position, m_rotation, {m_objdata->s_x * 2,m_objdata->s_y * 2,m_objdata->s_z * 2});
		m_lenght = (m_objdata->s_x + m_objdata->s_y + m_objdata->s_z) * 2;
		m_radius = pow(m_objdata->s_volume, 1.0f / 3.0f) / 2.0f;
	}
	//メッシュコライダー
	else {
		m_staticobject.CreateMeshObject(&m_modeldata->s_skinmodel,m_position, m_rotation);
		m_lenght = (m_objdata->s_x + m_objdata->s_y + m_objdata->s_z) * 2;
		m_radius = pow(m_objdata->s_volume, 1.0f / 3.0f) / 2.0f;
	}
	if (m_objdata->s_islinesegment == 1) {
		m_islinesegment = true;
		m_linevector = m_objdata->s_linevector;
	}
	
	m_box.Init(CVector3(m_objdata->s_x,m_objdata->s_y,m_objdata->s_z));
	ClcVertex();
	m_modeldata->s_skinmodel.UpdateInstancingData(m_position, m_rotation,CVector3::One(),m_anim.GetPlayAnimationType());
	m_gamedata = &GetGameData();
	m_staticobject.SetSize(m_radius);
	m_staticobject.GetRigidBody()->GetBody()->setUserIndex(3);

	
	return true;
}

void Obj::ReadMovePath(const int& number)
{
	MovePath* mp = new MovePath();
	wchar_t aiueo[256];
	swprintf_s(aiueo, L"Assets/path/stage%d/%ls%d.tks", GetGameData().GetStageNumber(),m_objdata->s_name, number);
	mp->ReadPath(aiueo);
	m_move = mp;
}

void Obj::InitMove(EnMove state, const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot)
{
	//移動用のクラスのインスタンスを生成
	switch (state)
	{
	case enMove_Lr:
		m_move = new MoveLR();
		break;
	case enMove_Up:
		m_move = new MoveUp();
		break;
		case enMove_Path:
		//ここでnewしない
		break;
	default:
		break;
	}
	if (state != enMove_No) {
		m_move->Init(pos, move, movelimit, rot);
		m_move->SetMoveState();
	}
	m_movestate = state;
	m_position = pos;
	m_rotation = rot;
}

void Obj::InitRot(EnRotate state, const float& speed)
{
	//回転用のクラスのインスタンスを生成
	switch (state)
	{
	case enRot_Rot:
		m_rot = new RotSelf();
		break;
	case enRot_DirectionRot:
		m_rot = new RotDirection();
		break;
	default:
		break;
	}
	if (state != enRot_No) {
		m_rot->Init(m_rotation, speed);
	}
	m_rotstate = state;
}

void Obj::ClcVertex()
{
	if (m_movestate != enMove_MoveHit) {
		m_box.Update(m_position,m_rotation);
	}
	else {
		m_box.Update(m_worldMatrix);
		if (m_box.GetisLowPositionY(m_player->GetPosition(),m_objdata->s_state)) {
			m_isclclinesegment = false;
		}
		else {
			m_linevector = m_box.SurfaceLineSegment(m_objdata->s_state) - m_player->GetPosition();
			m_isclclinesegment = true;
		}
	}
}



void Obj::ClcLocalMatrix(const CMatrix& worldMatrix)
{
	const float seVolume = 0.4f;

	//プレイヤーの逆行列を求める
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix objworldMatrix,transMatrix, rotMatrix, scaleMatrix;
	//平行移動行列を作成する。
	transMatrix.MakeTranslation(m_position);
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//拡大行列を作成する。
	scaleMatrix.MakeScaling(CVector3::One());
	//ワールド行列を作成する。
	//拡大×回転×平行移動の順番で乗算するように！
	//順番を間違えたら結果が変わるよ。
	objworldMatrix.Mul(scaleMatrix, rotMatrix);
	objworldMatrix.Mul(objworldMatrix, transMatrix);
	m_localMatrix.Mul(objworldMatrix, ReverseMatrix);
	m_movestate = enMove_MoveHit;
	m_staticobject.Release();
	if (m_islinesegment) {
		m_linesegment.Init(m_position);
	}
	m_box.Init(CVector3(m_objdata->s_x, m_objdata->s_y, m_objdata->s_z));
	OffScreen* offScreen = FindGO<OffScreen>();
	ObjModelDataFactory* factory = &GetObjModelDataFactory();
	offScreen->SetSkinModel(factory->GetSkinModel(m_modeldata->s_hashKey));
	offScreen->SetObjData(m_objdata);

	//巻き込まれたら音出す
	CSoundSource* se = new CSoundSource();
	if (m_objdata->s_isDefalutSe != 0) {
		wchar_t name[256];
		swprintf_s(name, L"Assets/sound/mono/%ls.wav", m_objdata->seFileName);
		se->Init(name);
	}
	else {
		se->Init(L"Assets/sound/water.wav");
	}
	se->Play(false);
	se->SetVolume(seVolume);
}

void Obj::ClcMatrix()
{
	//ローカル行列とプレイヤーのワールド行列を乗算
	m_worldMatrix.Mul(m_localMatrix, m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
}

void Obj::Update()
{
	if (!m_draw) {
		return;
	}
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}
	//巻き込まれたら一回だけ実行する
	if (!m_isHit && m_staticobject.GetRigidBody()->GetBody()->GetisHit()) {
		m_isHit = true;
		m_player->GetCSkinModelRender().UpdateWorldMatrix();
		ClcLocalMatrix(m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
		m_player->AddVolume(m_objdata->s_volume);
	}
	//巻き込まれた時の処理
	if (m_movestate == enMove_MoveHit) {
		if (!m_gamedata->GetisPose() || m_gamedata->GetScene() == enScene_Result) {
			ClcMatrix();
			if (m_islinesegment) {
				ClcVertex();
				if (m_isclclinesegment) {
					m_linesegment.Execute(m_player->GetPosition(), m_linevector);
				}
			}
		}
		m_modeldata->s_skinmodel.UpdateInstancingData(m_worldMatrix, m_anim.GetPlayAnimationType());
	}
	//巻き込まれてない時の処理
	else {
		if (!m_gamedata->GetisPose() || m_gamedata->GetScene() == enScene_Result) {
			if (m_movestate != enMove_No) {
				m_position = m_move->Move();
				m_staticobject.SetPosition(m_position);
			}
			if (m_rotstate != enRot_No) {
				m_rotation = m_rot->Rot(m_move->GetMoveVector());
				m_staticobject.SetRotation(m_rotation);
			}
			if (m_movestate != enMove_No || m_rotstate != enRot_No) {
				ClcVertex();
			}
		}
		m_modeldata->s_skinmodel.UpdateInstancingData(m_position, m_rotation, CVector3::One(), m_anim.GetPlayAnimationType());
	}
	m_anim.PlayAnimation(m_movestate);
}

void Obj::PostRender()
{
	if (m_movestate != enMove_MoveHit) {
		return;
	}
	/*wchar_t output[256];
	swprintf_s(output, L"%f", m_bufferList[2].y - m_bufferList[5].y);
	m_font.DrawScreenPos(output,CVector2::Zero());*/
}