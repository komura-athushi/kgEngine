#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "math/kgBox.h"
#include "GameData.h"
#include "sound\SoundSource.h"

Player::Player()
{
	
}


Player::~Player()
{
	
}

bool Player::Start()
{
	GameData* gameData = &GetGameData();
	m_protradius = gameData->GetPlayerSize();
	if (m_isTitle) {
		m_radius = gameData->GetFirstPlayerSize();
	}
	else {
		m_radius = gameData->GetPlayerSize();
	}
	//cmoファイルの読み込み、ステージの番号によって読み込むファイルを設定する
	wchar_t filePath[256];
	swprintf_s(filePath, L"Resource/modelData/sphere%d.cmo", (int)gameData->GetStageNumber());
	m_skinModelRender.Init(filePath);
	m_skinModelRender.SetShadowCaster(true);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetScale(m_scale);
	//キャラコン、スフィアコライダーを使う
	m_characon.Init(
		m_radius,			//半径。 
		m_position - CVector3::AxisY() * m_radius			//初期位置。
	);
	m_beforeposition = m_position;
	//体積を求める
	m_volume = CMath::PI * pow(m_radius, 3.0f) * 4 / 3;

	//人型のモデル
	m_skinModelRender2.Init(L"Resource/modelData/zunko.cmo");
	m_skinModelRender2.SetActive(true);
	m_skinModelRender2.SetScale(CVector3::One() * 0.5f);
	m_skinModelRender2.SetShadowCaster(true);
	m_skinModelRender2.SetShadowReceiver(true);
	m_gamedata = &GetGameData();
	return true;
}

void Player::Update()
{
	//リザルト画面ではキャラクターは非表示
	if (m_gamedata->GetScene() == enScene_Result) {
		m_skinModelRender2.SetisActive(false);
	}
	if (m_gamedata->GetisPose()) {
		return;
	}
	if (m_gamecamera == nullptr) {
		m_gamecamera = FindGO<GameCamera>();
	}
	//Judgment();
	Move();
	Turn();
	m_characon.SetPosition(m_position);
	m_beforeposition = m_position;
	m_skinModelRender.SetPosition(m_position + CVector3::AxisY() * m_radius);
	m_skinModelRender.SetRotation(m_rotation);
	CVector3 pos = MainCamera().GetFront();
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	m_skinModelRender2.SetRotation(rot);
	pos = pos * m_radius * 1.2f;
	pos = m_position - pos;
	m_skinModelRender2.SetPosition(pos);
	//シャドウマップのカメラをプレイヤーの座標を元に設定する
	Engine().GetGraphicsEngine().SetLightCameraPosition(CVector3(m_position.x + 300.0f,m_position.y + 300.0f,m_position.z + 300.0f));
	Engine().GetGraphicsEngine().SetLightCameraTarget(m_position);
	m_skinModelRender.UpdateWorldMatrix();
	m_skinModelRender2.UpdateWorldMatrix();
}

void Player::AddVolume(const float volume)
{
	const float volumeMultiply = 0.9f;

	m_volume += volume * volumeMultiply;
	m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
	m_characon.SetRadius(m_radius);
}

void Player::Judgment()
{
	const float LenghtMultiply = 1.0f;
	const float SizeMultiply = 0.9f;

	QueryGOs<Obj>(nullptr, [&](Obj* object) {
		if (object->GetisStickPlayer()) {
			return true;
		}
		CVector3 diff = object->GetPosition() - m_position - CVector3::AxisY() * m_radius;
		if (diff.LengthSq() >= pow(object->GetLenght() + m_radius * LenghtMultiply, 2.0f)) {
			return true;
		}
		if (m_radius >= object->GetRadius() * 3.0f) {
			if (object->GetisSphere()) {
				CVector3 diff = object->GetPosition() - m_position - CVector3::AxisY() * m_radius;
				if (diff.LengthSq() <= pow(m_radius + object->GetSize(), 2.0f) * LenghtMultiply) {
					object->ClcLocalMatrix(m_skinModelRender.GetSkinModel().GetWorldMatrix());
					m_volume += object->GetObjData().s_volume * SizeMultiply;
					m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
					m_characon.SetRadius(m_radius);
					return true;
				}
			}
			else {
				for (int i = 0; i < CBox::m_SurfaceVectorNumber; i++) {
					if (pow(m_radius * LenghtMultiply, 2.0f) >= (object->GetBox()->GetSurfaceVector(i)- m_position - CVector3::AxisY() * m_radius).LengthSq()) {
						object->ClcLocalMatrix(m_skinModelRender.GetSkinModel().GetWorldMatrix());
						m_volume += object->GetObjData().s_volume * SizeMultiply;
						m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
						m_characon.SetRadius(m_radius);
						return true;
					}
				}
				for (int i = 0; i < CBox::m_vertexNumber; i++) {
					if (pow(m_radius * LenghtMultiply, 2.0f) >= (object->GetBox()->GetVertexVector(i) - m_position - CVector3::AxisY() * m_radius).LengthSq()) {
						object->ClcLocalMatrix(m_skinModelRender.GetSkinModel().GetWorldMatrix());
						m_volume += object->GetObjData().s_volume * SizeMultiply;
						m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
						m_characon.SetRadius(m_radius);
						return true;
					}
				}

			}
		}
		return true;
	});
}

void Player::Move()
{
	const float StandardSize = 40.0f;
	//移動速度
	m_movespeedmultiply = 5.0f * (m_radius / m_standardSize) * 0.9f;
	//移動速度減衰
	const float MoveSpeedAtten = 0.98f;
	const float MoveSpeedAtten2 = 0.3f;
	//重力
	const float GravityMoveSpeed = 800.0f;
	//ジャンプ速度
	const float JumpMoveSpeed = 700.0f;
	//一定以上のyの速度があったらバウンドする〜
	const float LimitBoundMoveSpeed = -1000.0f;
	const float BoundMultiply = 0.7f;
	//地面と衝突する前のyベクトルを記憶する
	float MoveSpeedY = 0.0f;
	const float TimeLimit = 0.4f;
	const int CountLimit = 5;
	
	CVector3 Stick = CVector3::Zero();
	//両方のスティックが入力されていたら
	if (m_gamecamera->GetStateStick() == enStick_EnterStickBoth) {
		CVector3 stickL;
		stickL.x = GetPad(0).GetLStickXF();
		stickL.y = GetPad(0).GetLStickYF();
		stickL.z = 0.0f;
		CVector3 stickR;
		stickR.x = GetPad(0).GetRStickXF();
		stickR.y = GetPad(0).GetRStickYF();
		stickR.z = 0.0f;
		Stick = stickL + stickR;

	}
	//両方のスティックが逆の方向に入力されていたら、左が前方向
	else if (m_gamecamera->GetStateStick() == enStick_EnterStickBothOppositeLeft) {
		if (m_count == 0) {
			m_timer = 0.0f;
			m_isRight = true;
		}
		if (m_timer <= TimeLimit && m_isRight) {
			m_isRight = false;
			m_count++;
			m_timer = 0;
			m_timer = 0.0f;
		}
		
	}
	//両方のスティックが逆の方向に入力されていたら、右が前方向
	else if (m_gamecamera->GetStateStick() == enStick_EnterStickBothOppositeRight) {
		if (m_count == 0) {
			m_timer = 0.0f;
			m_isRight = false;
		}
		if (m_timer <= TimeLimit && !m_isRight) {
			m_count++;
			m_isRight = true;
			m_timer = 0.0f;
		}
	}
	//カウントが一定数だったらダッシュする
	if (m_timer >= TimeLimit) {
		m_count = 0;
		m_isDush = false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	CVector3 frontxz = MainCamera().GetFront();
	CVector3 rightxz = MainCamera().GetRight();
	if (m_count >= CountLimit) {
		if (m_isDush) {
			m_movespeed += frontxz * m_movespeedmultiply * 3;
		}
		//ダッシュ時の最初はある程度の移動速度を設定する
		else {
			for (int i = 0; i < 30; i++) {
				m_movespeed += frontxz * m_movespeedmultiply * 3;
				m_movespeed *= MoveSpeedAtten;
			}
			m_isDush = true;
		}
	}
	else {
		frontxz *= Stick.y;
		rightxz *= Stick.x;
		CVector3 addMoveSpeed = (frontxz + rightxz);
		CVector3 moveSpeed = m_movespeed;
		moveSpeed.y = 0.0f;
		addMoveSpeed.y = 0.0f;
		//ブレーキしてないかつ、移動速度がある程度あったら
 		if (!m_isBrake && 
			moveSpeed.LengthSq() >= ( m_movespeedmultiply * 40.0f ) * (m_movespeedmultiply * 40.0f) &&
			addMoveSpeed.LengthSq() >= 0.6f) {

			CVector3 addMove = addMoveSpeed;
			CVector3 move = moveSpeed;
			addMove.Normalize();
			move.Normalize();
			float angle = addMove.Dot(move);
			//移動方向とスティックの入力の角度がある程度大きかったらブレーキ
			if (fabs(acosf(angle)) >= CMath::PI * 0.5f) {
				m_movespeed *= MoveSpeedAtten2;
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/brake.wav");
				se->Play(false);
				se->SetVolume(0.7f);
				m_isBrake = true;
			}
		}
		else {
			m_isBrake = false;
		}
		m_movespeed += addMoveSpeed * m_movespeedmultiply;
	}
	//m_movespeed.y -= GravityMoveSpeed * GameTime().GetFrameDeltaTime();
	//if (m_characon.IsOnGround()) {
		/*CVector3 Normal = m_characon.GetGroundNormalVector();
		CVector3 Gravity = CVector3(0.0f, -GravityMoveSpeed, 0.0f);
		float t = Normal.Dot(Gravity);
		CVector3 vt = Normal * t * 1.2f;
		CVector3 va = Gravity - vt;
		m_movespeed += va * GameTime().GetFrameDeltaTime();*/
		
	//}
	//else {
		m_movespeed.y -= GravityMoveSpeed * GameTime().GetFrameDeltaTime();
	//}
	/*if (m_movespeed.y <= LimitBoundMoveSpeed) {
		m_isbound = true;
		MoveSpeedY = m_movespeed.y;
	}
	else {
		m_isbound = false;
	}*/
	
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_movespeed);
	//衝突したら
	if (m_characon.GetisCollision()) {
		if (m_count2 >= 1) {
			CVector3 Normal = m_characon.GetWallNormalVector();
			Normal.y = 0.0f;
			Normal.Normalize();
			float t = Normal.Dot(m_movespeed);
			CVector3 vt = Normal * t;
			CVector3 InversionSpeed = CVector3(-m_movespeed.x, 0.0f, -m_movespeed.z);
			CVector3 va = InversionSpeed + vt * 2;
			m_movespeed.x = -va.x * 0.5f;
			m_movespeed.z = -va.z * 0.5f;
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/syoutotu.wav");
			se->Play(false);
		}
		
		m_count2++;
	}
	else {
		m_count2 = 0;
	}
	if (m_characon.IsOnGround()) {
		/*if (m_isbound) {
			m_movespeed.y = -MoveSpeedY * BoundMultiply;
		}
		else {*/
			m_movespeed.y = 0.0f;
			if (GetPad(0).IsTrigger(enButtonB)) {
				m_movespeed.y = JumpMoveSpeed;
			}
		//}
		//m_movespeed.y = 0.0f;
	}
	m_movespeed.x *= MoveSpeedAtten;
	m_movespeed.z *= MoveSpeedAtten;
}

void Player::Turn()
{
	const float RotationSpeed = 1.0f * (m_protmovespeedmultiply / m_movespeedmultiply); //-((1.0f * (m_radius / m_protradius)) - 1.0f);

	CVector3 movespeedXZ = m_position - m_beforeposition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.01f) {
		return;
	}
	float Lengh = movespeedXZ.Length();
	//ノーマライズして移動方向の方向ベクトルを求める
	movespeedXZ /= Lengh;
	//Y軸と方向ベクトルの外積ベクトルを求める
	CVector3 pos;
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//外積ベクトルを元に回転させるクォータニオンを求める
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * RotationSpeed);
	//求めたクォータニオンを乗算する
	m_rotation.Multiply(rot,m_rotation);
}

void Player::PostRender()
{
	//ステージなら塊の大きさを表示する
	if (m_gamedata->GetScene() == enScene_Stage) {
		wchar_t output[256];
		swprintf_s(output, L"%.f\n", m_radius * 2.0f);
		//swprintf_s(output, L"%f\n", m_position.y);
		//swprintf_s(output, L"x %f y %f z %f\n", m_position.x , m_position.y ,m_position.z);
		m_font.DrawScreenPos(output, CVector2(0.0f, 50.0f), CVector4::White(), { 1.5f,1.5f });
		m_font.DrawScreenPos(L"cm\n", CVector2(70.0f, 80.0f), CVector4::White(), { 0.8f,0.8f });
	}
}