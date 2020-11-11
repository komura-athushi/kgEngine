#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Object/Obj.h"
#include "math/kgBox.h"
#include "GameData.h"
#include "sound\SoundSource.h"
#include "graphics\CEffektEngine.h"
#include "PlayerConst.h"

//
using namespace nsPlayerCnst;


Player::Player()
{
	
}


Player::~Player()
{
	
}

bool Player::Start()
{
	m_gameData = &GetGameData();
	

	wchar_t filePath[256];
	if (m_isTitle || m_gameData->GetisBattle()) {
		m_radius = m_gameData->GetFirstPlayerSize();
		m_protRadius = m_radius;
		swprintf_s(filePath, L"Resource/modelData/sphere1.cmo");
	}
	else {
		m_radius = m_gameData->GetPlayerSize();
		m_protRadius = m_gameData->GetPlayerSize();
		swprintf_s(filePath, L"Resource/modelData/sphere%d.cmo", (int)m_gameData->GetStageNumber());
	}
	//cmoファイルの読み込み、ステージの番号によって読み込むファイルを設定する
	

	m_skinModelRender.Init(filePath);
	m_skinModelRender.SetShadowCaster(true);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetScale(m_scale);
	//キャラコン、スフィアコライダーを使う
	m_charaCon.Init(
		m_radius,			//半径。 
		m_position - CVector3::AxisY() * m_radius			//初期位置。
	);
	m_charaCon.SetPlayerNumber(m_playerNumber);

	m_beforePosition = m_position;
	//体積を求める
	m_volume = CMath::PI * pow(m_radius, 3.0f) * 4 / 3;

	//人型のモデル
	m_skinModelRender2.Init(L"Resource/modelData/zunko.cmo");
	m_skinModelRender2.SetActive(true);
	m_skinModelRender2.SetScale(CVector3::One() * 0.5f);
	m_skinModelRender2.SetShadowCaster(true);
	m_skinModelRender2.SetShadowReceiver(true);

	m_skinModelRender.SetPosition(m_position);
	CVector3 pos = MainCamera(m_playerNumber).GetFront();
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	m_skinModelRender2.SetRotation(rot);
	pos = pos * m_radius * 1.2f;
	pos = m_position - pos;
	pos.y -= m_radius;
	m_skinModelRender2.SetPosition(pos);

	m_hitEffect = CEffektEngine::GetInstance().CreateEffekseerEffect(L"Assets/effect/hit.efk");
	//m_smokeEffect = CEffektEngine::GetInstance().CreateEffekseerEffect(L"Assets/effect/hit.efk");
	return true;
}

void Player::Update()
{
	//リザルト画面ではキャラクターは非表示
	if (m_gameData->GetScene() == enScene_Result) {
		m_skinModelRender2.SetisActive(false);
	}
	if (m_gameData->GetisPose() && m_gameData->GetisStart()) {
		return;
	}
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_isStopTime) {
		m_stopTimer += GameTime().GetFrameDeltaTime();
		if (m_stopTimer >= 1.5f) {
			m_isStopTime = false;
			m_stopTimer = 0.0f;
		}
		return;
	}
	//Judgment();
	Move();
	Turn();
	ScreenPosition();
	m_charaCon.SetPosition(m_position);
	m_beforePosition = m_position;
	m_skinModelRender.SetPosition(m_position + CVector3::AxisY() * m_radius);
	m_skinModelRender.SetRotation(m_rotation);
	CVector3 pos = MainCamera(m_playerNumber).GetFront();
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	m_skinModelRender2.SetRotation(rot);
	pos = pos * m_radius * 1.2f;
	pos = m_position - pos;
	m_skinModelRender2.SetPosition(pos);
	//シャドウマップのカメラをプレイヤーの座標を元に設定する
	Engine().GetGraphicsEngine().SetLightCameraPosition(CVector3(m_position.x + 300.0f,m_position.y + 300.0f,m_position.z + 300.0f),m_playerNumber);
	Engine().GetGraphicsEngine().SetLightCameraTarget(m_position,m_playerNumber);
	m_skinModelRender.UpdateWorldMatrix();
	m_skinModelRender2.UpdateWorldMatrix();
}

void Player::AddVolume(const float volume)
{
	const float volumeMultiply = 0.9f;

	m_volume += volume * volumeMultiply;
	m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
	m_charaCon.SetRadius(m_radius);
}

void Player::Move()
{

	//移動速度
	m_moveSpeedMultiply = 5.0f * (m_radius / m_standardSize) * 0.9f;

	CVector3 Stick = CVector3::Zero();
	m_skinModelRender.SetColor(CVector4(1.0f, 1.0f, 1.0f, 0.5f));
	//はまったりしたときの
	//B押したら初期位置に移動する
	if (GetPad(m_playerNumber).IsPress(enButtonB)) {
		m_respawnTimer += GameTime().GetFrameDeltaTime();
	
		if (m_respawnTimer >= 3.0f) {
			m_position = m_firstPosition;
			m_position.y += 50.0f;
			m_charaCon.SetPosition(m_firstPosition);
			m_respawnTimer = 0.0f;
		}
	}
	else {
		m_respawnTimer = 0.0f;
	}

	//両方のスティックが入力されていたら
	if (m_gameCamera->GetStateStick() == enStick_EnterStickBoth) {
		CVector3 stickL;
		stickL.x = GetPad(m_playerNumber).GetLStickXF();
		stickL.y = GetPad(m_playerNumber).GetLStickYF();
		stickL.z = 0.0f;
		CVector3 stickR;
		stickR.x = GetPad(m_playerNumber).GetRStickXF();
		stickR.y = GetPad(m_playerNumber).GetRStickYF();
		stickR.z = 0.0f;
		Stick = stickL + stickR;

	}
	//両方のスティックが逆の方向に入力されていたら、左が前方向
	else if (m_gameCamera->GetStateStick() == enStick_EnterStickBothOppositeLeft) {
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
	else if (m_gameCamera->GetStateStick() == enStick_EnterStickBothOppositeRight) {
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
	CVector3 frontxz = MainCamera(m_playerNumber).GetFront();
	CVector3 rightxz = MainCamera(m_playerNumber).GetRight();
	if (m_count >= CountLimit) {
		if (m_isDush) {
			m_moveSpeed += frontxz * m_moveSpeedMultiply * DushSpeed;
		}
		//ダッシュ時の最初はある程度の移動速度を設定する
		else {
			for (int i = 0; i < 30; i++) {
				m_moveSpeed += frontxz * m_moveSpeedMultiply * DushSpeed;
				m_moveSpeed *= MoveSpeedAtten;
			}
			m_isDush = true;
		}
	}
	else {
		frontxz *= Stick.y;
		rightxz *= Stick.x;
		CVector3 addMoveSpeed = (frontxz + rightxz);
		CVector3 moveSpeed = m_moveSpeed;
		moveSpeed.y = 0.0f;
		addMoveSpeed.y = 0.0f;
		//ブレーキしてないかつ、移動速度がある程度あったら
		if (!m_isBrake &&
			moveSpeed.LengthSq() >= (m_moveSpeedMultiply * 55.0f) * (m_moveSpeedMultiply * 55.0f) &&
			addMoveSpeed.LengthSq() >= 0.8f) {

			CVector3 addMove = addMoveSpeed;
			CVector3 move = moveSpeed;
			addMove.Normalize();
			move.Normalize();
			float angle = addMove.Dot(move);
			//移動方向とスティックの入力の角度がある程度大きかったらブレーキ
			if (fabs(acosf(angle)) >= CMath::PI * 0.5f && m_collisionTimer >= CollisionTime) {
				m_moveSpeed *= MoveSpeedAtten2;
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/brake.wav");
				se->Play(false);
				se->SetVolume(BrakeVolume);
				m_isBrake = true;
				/*auto effectEngine = CEffektEngine::GetInstance();
				m_smokeEffectHandle = effectEngine.Play(m_smokeEffect);
				effectEngine.SetPosition(m_smokeEffectHandle, m_position);
				effectEngine.SetScale(m_smokeEffectHandle, CVector3::One() * (m_radius / m_gamedata->GetFirstPlayerSize()) * 4.0f);
				effectEngine.SetRotation(m_smokeEffectHandle, 0.0f, 0.0f, 00.0f);*/

			}
		}
		else {
			m_isBrake = false;
		}
		m_moveSpeed += addMoveSpeed * m_moveSpeedMultiply;
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
		m_moveSpeed.y -= GravityMoveSpeed * GameTime().GetFrameDeltaTime();
	//}
	/*if (m_movespeed.y <= LimitBoundMoveSpeed) {
		m_isbound = true;
		MoveSpeedY = m_movespeed.y;
	}
	else {
		m_isbound = false;
	}*/
	if (!m_gameData->GetisStart()) {
		m_moveSpeed.x = 0.0f;
		m_moveSpeed.z = 0.0f;
	}
	m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
	//衝突したら
	if (m_charaCon.GetisCollision()) {
		if (m_count2 >= 2 && m_collisionTimer >= CollisionTime) {
			frontxz *= Stick.y;
			rightxz *= Stick.x;
			CVector3 addMoveSpeed = (frontxz + rightxz);
			CVector3 moveSpeed = m_moveSpeed;
			moveSpeed.y = 0.0f;
			addMoveSpeed.y = 0.0f;
			CVector3 Normal = m_charaCon.GetWallNormalVector();
			Normal.y = 0.0f;
			Normal.Normalize();
			float t = Normal.Dot(m_moveSpeed);
			CVector3 vt = Normal * t;
			CVector3 InversionSpeed = CVector3(-m_moveSpeed.x, 0.0f, -m_moveSpeed.z);
			CVector3 va = InversionSpeed + vt * 2;
			m_moveSpeed.x = -va.x * CollisionAtten;
			m_moveSpeed.z = -va.z * CollisionAtten;
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/syoutotu.wav");
			se->Play(false);
			se->SetVolume(CollisionVolume);
			m_collisionTimer = 0.0f;
			auto effectEngine = CEffektEngine::GetInstance();
			m_playEffectHandle = effectEngine.Play(m_hitEffect);
			effectEngine.SetPosition(m_playEffectHandle,m_charaCon.GetHitPos());
			effectEngine.SetScale(m_playEffectHandle, CVector3::One()* (m_radius / m_gameData->GetFirstPlayerSize()) * 4.0f);
			//キャラクタ属性のコリジョンと衝突したら
			if (m_charaCon.GetisHitCharacter()) {
				//ある程度早さがあったら
				if (!m_isBrake &&
					moveSpeed.LengthSq() >= (m_moveSpeedMultiply * 55.0f) * (m_moveSpeedMultiply * 55.0f) &&
					addMoveSpeed.LengthSq() >= 0.8f) {
					moveSpeed.Normalize();
					CVector3 hitVector = m_charaCon.GetHitPos() - m_position;
					hitVector.y = 0.0f;
					hitVector.Normalize();
					float angle = acosf(hitVector.Dot(moveSpeed));
					if (fabsf(angle) < CMath::PI * 0.40f) {
						m_player->SetStopTime();
					}
				}

			}

		}
		
		m_count2++;
	}
	else {
		m_count2 = 0;
		m_collisionTimer += GameTime().GetFrameDeltaTime();
	}
	if (m_charaCon.IsOnGround()) {
		/*if (m_isbound) {
			m_movespeed.y = -MoveSpeedY * BoundMultiply;
		}
		else {*/
			m_moveSpeed.y = 0.0f;
			//if (GetPad(0).IsTrigger(enButtonB)) {
				//m_movespeed.y = JumpMoveSpeed;
			//}
		//}
		//m_movespeed.y = 0.0f;
	}
	m_moveSpeed.x *= MoveSpeedAtten;
	m_moveSpeed.z *= MoveSpeedAtten;
}

void Player::Turn()
{
	const float RotationSpeed = 1.0f * (m_protMoveSpeedMultiply / m_moveSpeedMultiply); //-((1.0f * (m_radius / m_protradius)) - 1.0f);

	CVector3 movespeedXZ = m_position - m_beforePosition;
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

void Player::ScreenPosition()
{
    //m_skinModelRender.GetSkinModel().GetWorldMatrix().Mul(worldPos);
	//CVector3 worldPos = CVector3(m_skinModelRender.GetSkinModel().GetWorldMatrix().m[3][0], m_skinModelRender.GetSkinModel().GetWorldMatrix().m[3][1], m_skinModelRender.GetSkinModel().GetWorldMatrix().m[3][2]);
	CVector3 worldPos = m_position + CVector3::AxisY() * m_radius;
	MainCamera(m_playerNumber).GetViewMatrix().Mul(worldPos);
	CVector4 tmp = worldPos;
	MainCamera(m_playerNumber).GetProjectionMatrix().Mul(tmp);
	m_katamariVector.x = tmp.x / tmp.w;
	m_katamariVector.y = tmp.y / tmp.w;
	m_katamariVector.z = tmp.z / tmp.w;
}

void Player::PostRender()
{
	//ステージなら塊の大きさを表示する
	if (m_gameData->GetScene() == enScene_Stage) {
		wchar_t output[256];
		int playerSize = int(m_radius * 2.0f);
		int goalSize = int(m_gameData->GetGoalPlayerSize());
		swprintf_s(output, L"%d\n", playerSize);
		//swprintf_s(output, L"%f\n", m_position.y);
		//swprintf_s(output, L"x %f y %f z %f\n", m_position.x , m_position.y ,m_position.z);
		CVector4 color;
		if (playerSize >= goalSize && !m_gameData->GetisBattle()) {
			color = CVector4::Blue();
		}
		else {
			color = CVector4::Red();
		}

		float y = 0.0f;
		if (m_playerNumber == 1) {
			y = FRAME_BUFFER_H / 2.0f;
		}
	
		//m_font.DrawScreenPos(L"cm\n", CVector2(78.0f, 38.0f), color, { 0.8f,0.8f });

		if (m_gameData->GetisBattle()) {
			if (m_playerNumber == 0) {
				color = CVector4::Red();
			}
			else if (m_playerNumber == 1) {
				color = CVector4::Blue();
			}
			wchar_t output[256];
			swprintf_s(output, L"%dP\n", m_playerNumber + 1);
			m_font.DrawScreenPos(output, CVector2(0.0f, 10.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 1.6f,1.6f });
			m_font.DrawScreenPos(output, CVector2(0.0f, 10.0f + y), color, { 1.5f,1.5f });
		}
		else {
			m_font.DrawScreenPos(output, CVector2(0.0f, 10.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 1.6f,1.6f });
			m_font.DrawScreenPos(L"cm\n", CVector2(82.0f, 38.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.86f,0.86f });
			m_font.DrawScreenPos(output, CVector2(0.0f, 10.0f + y), color, { 1.5f,1.5f });
			m_font.DrawScreenPos(L"cm\n", CVector2(82.0f, 38.0f + y), color, { 0.8f,0.8f });

			m_font.DrawScreenPos(L"イマ", CVector2(50.0f, 80.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.86f,0.86f });
			m_font.DrawScreenPos(L"イマ", CVector2(50.0f, 80.0f + y), CVector4::White(), { 0.8f,0.8f });
			wchar_t output2[256];
			swprintf_s(output2, L"%d\n", goalSize);
			m_font.DrawScreenPos(output2, CVector2(160.0f, 65.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 1.07f,1.07f });
			m_font.DrawScreenPos(L"cm\n", CVector2(217.0f, 85.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.55f,0.55f });
			m_font.DrawScreenPos(output2, CVector2(160.0f, 65.0f + y), CVector4::White(), { 1.0f,1.0f });
			m_font.DrawScreenPos(L"cm\n", CVector2(217.0f, 85.0f + y), CVector4::White(), { 0.5f,0.5f });

			m_font.DrawScreenPos(L"モクヒョ", CVector2(175.0f, 40.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.55f,0.55f });
			m_font.DrawScreenPos(L"モクヒョ", CVector2(175.0f, 40.0f + y), CVector4::White(), { 0.5f,0.5f });

			m_font.DrawScreenPos(L"/\n", CVector2(112.0f, 30.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 2.15f,2.15f });
			m_font.DrawScreenPos(L"/\n", CVector2(112.0f, 30.0f + y), CVector4::White(), { 2.0f,2.0f });
		}
	}
}