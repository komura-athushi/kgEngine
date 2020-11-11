#include "stdafx.h"
#include "Obj.h"
#include "Move/MoveLR.h"
#include "Move\MoveFB.h"
#include "Move/MoveUp.h"
#include "Move/MovePath.h"
#include "Rotation/IRot.h"
#include "Rotation/RotSelf.h"
#include "Rotation/RotDirection.h"
#include "Player.h"
#include "GameData.h"
#include "OffScreen.h"
#include "sound/SoundSource.h"
#include "Move\MoveNone.h"
#include "Rotation\RotNone.h"

namespace {
	const float seVolume = 0.25f;		//巻き込まれたときのSEのボリューム
}

ObjModelDataFactory::ObjModelDataFactory()
{

}

bool ObjModelDataFactory::Start()
{
	return true;
}

void ObjModelDataFactory::PreUpdate()
{
	//インスタンシング描画のための前処理
	BeginUpdateInstancingData();
}

ObjModelData* ObjModelDataFactory::Load(const wchar_t* path) 
{
	//名前からハッシュタグを生成
	int key = Util::MakeHash(path);
	//
	if (m_modelMap.count(key) == 0) {
		wchar_t filepath[256];
		swprintf_s(filepath, L"Resource/modelData/%ls.cmo", path);
		//std::make_unique  スマートポインタ生成のヘルパー関数
		//newが不要になる、()に初期値
		m_modelMap.emplace(key, std::make_unique<ObjModelData>());
		//モデルをロード
		m_modelMap[key].get()->s_skinModel.Init(filepath, nullptr, 0, enFbxUpAxisZ, true);
		SetPriorityGO(&m_modelMap[key].get()->s_skinModel, 2);
		m_modelMap[key].get()->s_hashKey = key;
		auto skinModel = std::make_unique<SkinModel>();
		skinModel->Init(filepath);
		//所有権をムーブする。
		m_skinModelMap.emplace(key, std::move(skinModel));
	}
	m_modelMap[key].get()->s_maxInstance += 1;
	m_modelMap[key].get()->s_skinModel.SetInstanceNumber(m_modelMap[key].get()->s_maxInstance);
	//getでマッピングされている値
	return m_modelMap[key].get();
}

void ObjModelDataFactory::InitInstancingData()
{
	//各モデルのインスタンシングデータを初期化
	if (m_modelMap.size() != 0) {
		for (auto itr = m_modelMap.begin(); itr != m_modelMap.end(); ++itr) {
			itr->second.get()->s_skinModel.InitInstancing();
		}
	}
}

void ObjModelDataFactory::BeginUpdateInstancingData()
{
	//インスタンシングを開始するぞい
	if (m_modelMap.size() != 0) {
		for (auto itr = m_modelMap.begin(); itr != m_modelMap.end(); ++itr) {
			itr->second.get()->s_skinModel.BeginUpdateInstancingData();
		}
	}
}

void ObjModelDataFactory::DeleteAllData()
{
	//マップクリアー
	m_modelMap.clear();
}

Obj::Obj()
{

}

Obj::~Obj()
{
	//ステージクリアしてたら
	if (m_gameData->GetScene() == enScene_Result && m_gameData->GetisGameClear()) {
		//図鑑で表示するように設定する
		GetObjectData().SetisHit(m_objData->s_volume);
	}
}

void Obj::SetFilePath(const wchar_t* path)
{
	//モデルをロード
	m_modelData = GetObjModelDataFactory().Load(path);
	//アニメーションが設定されていたら
	if (m_objData->s_isAnimation == 1) {
		//アニメーションを読み込む
		m_anim.Init(m_objData->s_name, &m_modelData->s_skinModel);
	}
}

bool Obj::Start()
{
	m_size = (m_objData->s_x + m_objData->s_y + m_objData->s_z) / 3;
	//球体である
	if (m_objData->s_isSphere == 1) {
		m_isSphere = true;
		m_size = m_objData->s_x;
		m_lenght = m_size * 2;
		m_radius = m_objData->s_x;
		m_staticObject.CreateSphereObject(m_size, m_position, m_rotation);
	}
	else {
		//メッシュコライダーじゃない？
		if (m_objData->s_isMeshCollider == 0) {
			m_staticObject.CreateBoxObject(m_position, m_rotation, { m_objData->s_x * 2,m_objData->s_y * 2,m_objData->s_z * 2 });
		}
		//メッシュコライダー
		else {
			m_staticObject.CreateMeshObject(&m_modelData->s_skinModel, m_position, m_rotation);
		}
		m_lenght = (m_objData->s_x + m_objData->s_y + m_objData->s_z) * 2;
		m_radius = pow(m_objData->s_volume, 1.0f / 3.0f) / 2.0f;
	}
	//巻き込まれたときにガタガタ処理をする？
	if (m_objData->s_isLineSegment == 1) {
		m_isLineSegment = true;
		m_lineVector = m_objData->s_lineVector;
	}
	//鏡面反射する？
	if (m_objData->s_isSpec == 1) {
		TextureData textureData;
		wchar_t output[256];
		swprintf_s(output, L"Resource/sprite/%ls.dds", m_objData->s_specName);
		textureData.specFilePath = output;
		//スペキュラマップを読み込む
		m_modelData->s_skinModel.InitTexture(&textureData);
	}
	m_box.Init(CVector3(m_objData->s_x,m_objData->s_y,m_objData->s_z));
	ClcVertex();
	m_modelData->s_skinModel.UpdateInstancingData(m_position, m_rotation,CVector3::One(),m_anim.GetPlayAnimationType());
	m_gameData = &GetGameData();
	m_staticObject.SetSize(m_radius);
	m_staticObject.GetRigidBody()->GetBody()->setUserIndex(3);

	m_modelData->s_skinModel.GetSkinModel().SetisDithering(true);
	return true;
}

void Obj::ReadMovePath(const int& number)
{
	auto mp = std::make_unique<MovePath>();
	wchar_t aiueo[256];
	if (GameData::GetInstance().GetisBattle()) {
		//バトルモードはステージ1
		swprintf_s(aiueo, L"Assets/path/stage1/%ls%d.tks", m_objData->s_name, number);
		
	}
	else {
		swprintf_s(aiueo, L"Assets/path/stage%d/%ls%d.tks", GetGameData().GetStageNumber(), m_objData->s_name, number);
	}
	//パスを読み込む
	mp->ReadPath(aiueo);
	m_move = std::move(mp);
}

void Obj::InitMove(EnMove state, const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	//移動用のクラスのインスタンスを生成
	switch (state)
	{
	case enMove_Lr:
		m_move = std::make_unique<MoveLR>();
		break;
	case enMove_Fb:
		m_move = std::make_unique <MoveFB>();
		break;
	case enMove_Up:
		m_move = std::make_unique<MoveUp>();
		break;
		case enMove_Path:
		//ここでnewしない
		break;
	default:
		m_move = std::make_unique<MoveNone>();
		break;
	}
	
	//初期化
	m_move->Init(pos, move, movelimit, rot);
	m_move->SetMoveState();

	m_moveState = state;
	m_position = pos;
	m_rotation = rot;
}

void Obj::InitRot(EnRotate state, const float speed)
{
	//回転用のクラスのインスタンスを生成
	switch (state)
	{
	case enRot_Rot:
		m_rot = std::make_unique<RotSelf>();
		break;
	case enRot_DirectionRot:
		m_rot = std::make_unique<RotDirection>();
		break;
	case enRot_No:
		m_rot = std::make_unique<RotNone>();
		break;
	}
	//初期化
	m_rot->Init(m_rotation, speed);
	m_rotState = state;
}

void Obj::ClcVertex()
{
	//巻き込まれてない
	if (m_moveState != enMove_MoveHit) {
		m_box.Update(m_position,m_rotation);
	}
	//巻き込まれた
	else {
		m_box.Update(m_worldMatrix);
		if (m_box.GetisLowPositionY(m_player->GetPosition(),m_objData->s_state)) {
			m_isClcLineSegment = false;
		}
		else {
			m_lineVector = m_box.SurfaceLineSegment(m_objData->s_state) - m_player->GetPosition();
			m_isClcLineSegment = true;
		}
	}
}

void Obj::ClcLocalMatrix(const CMatrix& worldMatrix)
{
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
	m_moveState = enMove_MoveHit;
	m_staticObject.Release();
	if (m_isLineSegment) {
		//ガタガタ処理のための初期化
		m_lineSegment.Init(m_position);
		m_lineSegment.GetRigidBody()->GetBody()->setUserIndex(4);
		m_lineSegment.SetPlayer(m_player);
	}
	m_box.Init(CVector3(m_objData->s_x, m_objData->s_y, m_objData->s_z));
	OffScreen* offScreen = FindGO<OffScreen>();
	ObjModelDataFactory* factory = &GetObjModelDataFactory();
	offScreen->SetSkinModel(factory->GetSkinModel(m_modelData->s_hashKey));
	offScreen->SetObjData(m_objData);

	//巻き込まれたら音出す
	CSoundSource* se = new CSoundSource();
	if (m_objData->s_isDefalutSe != 0) {
		wchar_t name[256];
		swprintf_s(name, L"Assets/sound/mono/%ls.wav", m_objData->s_seFileName);
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

void Obj::MoveRotation()
{
	//ポーズ中かあるいはリザルト画面じゃなかったら
	if (!m_gameData->GetisPose() || m_gameData->GetScene() == enScene_Result) {
		//巻き込まれてる
		if (m_moveState == enMove_MoveHit) {
			ClcMatrix();
			if (m_isLineSegment) {
				ClcVertex();
				if (m_isClcLineSegment) {
					m_lineSegment.Execute(m_player->GetPosition(), m_lineVector);
				}
			}
		}
		//巻き込まれていない
		else {
			//移動
			m_position = m_move->Move();
			m_staticObject.SetPosition(m_position);

			//回転
			m_rotation = m_rot->Rot(m_move->GetMoveVector());
			m_staticObject.SetRotation(m_rotation);

			//当たり判定処理、動いたり回転してなかったりしたら更新しない
			if (m_moveState != enMove_No || m_rotState != enRot_No) {
				ClcVertex();
			}
		}
	}
}

void Obj::Update()
{
	if (!m_draw) {
		return;
	}
	//巻き込まれたら一回だけ実行する
	if (!m_isHit && m_staticObject.GetRigidBody()->GetBody()->GetisHit()) {
		QueryGOs<Player>(nullptr, [&](Player* player) {
			if (player->GetPlayerNumber() == m_staticObject.GetRigidBody()->GetBody()->GetPlayerNumber()) {
				m_player = player;
				return false;
			}
			else {
				return true;
			}
		});
		m_isHit = true;
		m_player->GetCSkinModelRender().UpdateWorldMatrix();
		ClcLocalMatrix(m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
		m_player->AddVolume(m_objData->s_volume);
	}

	//移動回転処理
	MoveRotation();

	//インスタンシング処理
	if (m_moveState == enMove_MoveHit) {
		m_modelData->s_skinModel.UpdateInstancingData(m_worldMatrix, m_anim.GetPlayAnimationType());
	}
	else {
		m_modelData->s_skinModel.UpdateInstancingData(m_position, m_rotation, CVector3::One(), m_anim.GetPlayAnimationType());
	}
	//アニメーション
	m_anim.PlayAnimation(m_moveState);
}

