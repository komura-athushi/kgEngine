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
	m_gamedata = &GetGameData();
	

	wchar_t filePath[256];
	if (m_isTitle || m_gamedata->GetisBattle()) {
		m_radius = m_gamedata->GetFirstPlayerSize();
		m_protradius = m_radius;
		swprintf_s(filePath, L"Resource/modelData/sphere1.cmo");
	}
	else {
		m_radius = m_gamedata->GetPlayerSize();
		m_protradius = m_gamedata->GetPlayerSize();
		swprintf_s(filePath, L"Resource/modelData/sphere%d.cmo", (int)m_gamedata->GetStageNumber());
	}
	//cmo�t�@�C���̓ǂݍ��݁A�X�e�[�W�̔ԍ��ɂ���ēǂݍ��ރt�@�C����ݒ肷��
	

	m_skinModelRender.Init(filePath);
	m_skinModelRender.SetShadowCaster(true);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetScale(m_scale);
	//�L�����R���A�X�t�B�A�R���C�_�[���g��
	m_characon.Init(
		m_radius,			//���a�B 
		m_position - CVector3::AxisY() * m_radius			//�����ʒu�B
	);
	m_characon.SetPlayerNumber(m_playerNumber);

	m_beforeposition = m_position;
	//�̐ς����߂�
	m_volume = CMath::PI * pow(m_radius, 3.0f) * 4 / 3;

	//�l�^�̃��f��
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

	return true;
}

void Player::Update()
{
	//���U���g��ʂł̓L�����N�^�[�͔�\��
	if (m_gamedata->GetScene() == enScene_Result) {
		m_skinModelRender2.SetisActive(false);
	}
	if (m_gamedata->GetisPose() && m_gamedata->GetisStart()) {
		return;
	}
	if (m_gamecamera == nullptr) {
		m_gamecamera = FindGO<GameCamera>();
		return;
	}
	//Judgment();
	Move();
	Turn();
	ScreenPosition();
	m_characon.SetPosition(m_position);
	m_beforeposition = m_position;
	m_skinModelRender.SetPosition(m_position + CVector3::AxisY() * m_radius);
	m_skinModelRender.SetRotation(m_rotation);
	CVector3 pos = MainCamera(m_playerNumber).GetFront();
	CQuaternion rot;
	rot.SetRotation(CVector3::AxisY(), atan2f(pos.x, pos.z));
	m_skinModelRender2.SetRotation(rot);
	pos = pos * m_radius * 1.2f;
	pos = m_position - pos;
	m_skinModelRender2.SetPosition(pos);
	//�V���h�E�}�b�v�̃J�������v���C���[�̍��W�����ɐݒ肷��
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
	//�ړ����x
	m_movespeedmultiply = 5.0f * (m_radius / m_standardSize) * 0.9f;
	//�ړ����x����
	const float MoveSpeedAtten = 0.98f;
	const float MoveSpeedAtten2 = 0.3f;
	//�d��
	const float GravityMoveSpeed = 800.0f;
	//�W�����v���x
	const float JumpMoveSpeed = 700.0f;
	//���ȏ��y�̑��x����������o�E���h����`
	const float LimitBoundMoveSpeed = -1000.0f;
	const float BoundMultiply = 0.7f;
	//�n�ʂƏՓ˂���O��y�x�N�g�����L������
	float MoveSpeedY = 0.0f;
	const float TimeLimit = 0.3f;
	const int CountLimit = 5;
	const float DushSpeed = 5.0f;
	//�A���ŏՓˉ����o���Ȃ����߂�
	const float CollisionTime = 0.2f;
	const float BrakeVolume = 0.7f;
	const float CollisionVolume = 1.8;
	const float CollisionAtten = 0.5f;
	
	CVector3 Stick = CVector3::Zero();

	//�͂܂����肵���Ƃ���
	//B�������珉���ʒu�Ɉړ�����
	if (GetPad(m_playerNumber).IsTrigger(enButtonB)) {
		m_position = m_firstPosition;
		m_position.y += 50.0f;
		m_characon.SetPosition(m_firstPosition);
	}

	//�����̃X�e�B�b�N�����͂���Ă�����
	if (m_gamecamera->GetStateStick() == enStick_EnterStickBoth) {
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
	//�����̃X�e�B�b�N���t�̕����ɓ��͂���Ă�����A�����O����
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
	//�����̃X�e�B�b�N���t�̕����ɓ��͂���Ă�����A�E���O����
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
	//�J�E���g����萔��������_�b�V������
	if (m_timer >= TimeLimit) {
		m_count = 0;
		m_isDush = false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	CVector3 frontxz = MainCamera(m_playerNumber).GetFront();
	CVector3 rightxz = MainCamera(m_playerNumber).GetRight();
	if (m_count >= CountLimit) {
		if (m_isDush) {
			m_movespeed += frontxz * m_movespeedmultiply * DushSpeed;
		}
		//�_�b�V�����̍ŏ��͂�����x�̈ړ����x��ݒ肷��
		else {
			for (int i = 0; i < 30; i++) {
				m_movespeed += frontxz * m_movespeedmultiply * DushSpeed;
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
		//�u���[�L���ĂȂ����A�ړ����x��������x��������
		if (!m_isBrake &&
			moveSpeed.LengthSq() >= (m_movespeedmultiply * 40.0f) * (m_movespeedmultiply * 40.0f) &&
			addMoveSpeed.LengthSq() >= 0.8f) {

			CVector3 addMove = addMoveSpeed;
			CVector3 move = moveSpeed;
			addMove.Normalize();
			move.Normalize();
			float angle = addMove.Dot(move);
			//�ړ������ƃX�e�B�b�N�̓��͂̊p�x��������x�傫��������u���[�L
			if (fabs(acosf(angle)) >= CMath::PI * 0.5f && m_collisionTimer >= CollisionTime) {
				m_movespeed *= MoveSpeedAtten2;
				CSoundSource* se = new CSoundSource();
				se->Init(L"Assets/sound/brake.wav");
				se->Play(false);
				se->SetVolume(BrakeVolume);
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
	if (!m_gamedata->GetisStart()) {
		m_movespeed.x = 0.0f;
		m_movespeed.z = 0.0f;
	}
	m_position = m_characon.Execute(GameTime().GetFrameDeltaTime(), m_movespeed);
	//�Փ˂�����
	if (m_characon.GetisCollision()) {
		if (m_count2 >= 1 && m_collisionTimer >= CollisionTime) {
			CVector3 Normal = m_characon.GetWallNormalVector();
			Normal.y = 0.0f;
			Normal.Normalize();
			float t = Normal.Dot(m_movespeed);
			CVector3 vt = Normal * t;
			CVector3 InversionSpeed = CVector3(-m_movespeed.x, 0.0f, -m_movespeed.z);
			CVector3 va = InversionSpeed + vt * 2;
			m_movespeed.x = -va.x * CollisionAtten;
			m_movespeed.z = -va.z * CollisionAtten;
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/syoutotu.wav");
			se->Play(false);
			se->SetVolume(CollisionVolume);
			m_collisionTimer = 0.0f;
		}
		
		m_count2++;
	}
	else {
		m_count2 = 0;
		m_collisionTimer += GameTime().GetFrameDeltaTime();
	}
	if (m_characon.IsOnGround()) {
		/*if (m_isbound) {
			m_movespeed.y = -MoveSpeedY * BoundMultiply;
		}
		else {*/
			m_movespeed.y = 0.0f;
			//if (GetPad(0).IsTrigger(enButtonB)) {
				//m_movespeed.y = JumpMoveSpeed;
			//}
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
	//�m�[�}���C�Y���Ĉړ������̕����x�N�g�������߂�
	movespeedXZ /= Lengh;
	//Y���ƕ����x�N�g���̊O�σx�N�g�������߂�
	CVector3 pos;
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	CQuaternion rot;
	rot.SetRotationDeg(pos, Lengh * RotationSpeed);
	//���߂��N�H�[�^�j�I������Z����
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
	//�X�e�[�W�Ȃ��̑傫����\������
	if (m_gamedata->GetScene() == enScene_Stage) {
		wchar_t output[256];
		int playerSize = int(m_radius * 2.0f);
		int goalSize = int(m_gamedata->GetGoalPlayerSize());
		swprintf_s(output, L"%d\n", playerSize);
		//swprintf_s(output, L"%f\n", m_position.y);
		//swprintf_s(output, L"x %f y %f z %f\n", m_position.x , m_position.y ,m_position.z);
		CVector4 color;
		if (playerSize >= goalSize && !m_gamedata->GetisBattle()) {
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

		if (m_gamedata->GetisBattle()) {
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

			m_font.DrawScreenPos(L"�C�}", CVector2(50.0f, 80.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.86f,0.86f });
			m_font.DrawScreenPos(L"�C�}", CVector2(50.0f, 80.0f + y), CVector4::White(), { 0.8f,0.8f });
			wchar_t output2[256];
			swprintf_s(output2, L"%d\n", goalSize);
			m_font.DrawScreenPos(output2, CVector2(160.0f, 65.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 1.07f,1.07f });
			m_font.DrawScreenPos(L"cm\n", CVector2(217.0f, 85.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.55f,0.55f });
			m_font.DrawScreenPos(output2, CVector2(160.0f, 65.0f + y), CVector4::White(), { 1.0f,1.0f });
			m_font.DrawScreenPos(L"cm\n", CVector2(217.0f, 85.0f + y), CVector4::White(), { 0.5f,0.5f });

			m_font.DrawScreenPos(L"���N�q��", CVector2(175.0f, 40.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 0.55f,0.55f });
			m_font.DrawScreenPos(L"���N�q��", CVector2(175.0f, 40.0f + y), CVector4::White(), { 0.5f,0.5f });

			m_font.DrawScreenPos(L"/\n", CVector2(112.0f, 30.0f + y), CVector4(0.0f, 0.0f, 0.0f, 1.0f), { 2.15f,2.15f });
			m_font.DrawScreenPos(L"/\n", CVector2(112.0f, 30.0f + y), CVector4::White(), { 2.0f,2.0f });
		}
	}
}