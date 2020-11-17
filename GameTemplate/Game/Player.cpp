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

	//cmo�t�@�C���̓ǂݍ��݁A�X�e�[�W�̔ԍ��ɂ���ēǂݍ��ރt�@�C����ݒ肷��
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
	m_skinModelRender.Init(filePath);
	m_skinModelRender.SetShadowCaster(true);
	m_skinModelRender.SetShadowReceiver(true);
	m_skinModelRender.SetScale(m_scale);
	//�L�����R���A�X�t�B�A�R���C�_�[���g��
	m_charaCon.Init(
		m_radius,			//���a�B 
		m_position - CVector3::AxisY() * m_radius			//�����ʒu�B
	);
	m_charaCon.SetPlayerNumber(m_playerNumber);

	m_beforePosition = m_position;
	//�̐ς����߂�
	m_volume = CMath::PI * pow(m_radius, 3.0f) * 4 / 3;

	//�l�^�̃��f��
	m_skinModelRender2.Init(L"Resource/modelData/zunko.cmo");
	m_skinModelRender2.SetActive(true);
	m_skinModelRender2.SetScale(CVector3::One() * 0.5f);
	m_skinModelRender2.SetShadowCaster(true);
	m_skinModelRender2.SetShadowReceiver(true);

	//��
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
	return true;
}

void Player::Update()
{
	//���U���g��ʂł̓L�����N�^�[�͔�\��
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
	//��~
	if (m_isStopTime) {
		m_stopTimer += GameTime().GetFrameDeltaTime();
		if (m_stopTimer >= stopTime) {
			m_isStopTime = false;
			m_stopTimer = 0.0f;
		}
		return;
	}
	m_beforePosition = m_position;
	//�ړ�
	Move();

	//��]
	Turn();
	ScreenPosition();
	//�V���h�E�}�b�v�̃J�������v���C���[�̍��W�����ɐݒ肷��
	Engine().GetGraphicsEngine().SetLightCameraPosition(CVector3(m_position.x + 300.0f,m_position.y + 300.0f,m_position.z + 300.0f),m_playerNumber);
	Engine().GetGraphicsEngine().SetLightCameraTarget(m_position,m_playerNumber);
	m_skinModelRender.UpdateWorldMatrix();
	m_skinModelRender2.UpdateWorldMatrix();
}

void Player::AddVolume(const float volume)
{
	//�I�u�W�F�N�g�̑̐ς���ɉ��Z����
	m_volume += volume * volumeMultiply;
	m_radius = pow(3 * m_volume / (4 * CMath::PI), 1.0 / 3.0f);
	m_charaCon.SetRadius(m_radius);
}

void Player::Stick()
{
	//�����̃X�e�B�b�N�����͂���Ă�����
	if (m_gameCamera->GetStateStick() == enStick_EnterStickBoth) {
		CVector3 stickL;
		stickL.x = GetPad(m_playerNumber).GetLStickXF();
		stickL.y = GetPad(m_playerNumber).GetLStickYF();
		stickL.z = 0.0f;
		CVector3 stickR;
		stickR.x = GetPad(m_playerNumber).GetRStickXF();
		stickR.y = GetPad(m_playerNumber).GetRStickYF();
		stickR.z = 0.0f;
		m_stick = stickL + stickR;

	}
	//�����̃X�e�B�b�N���t�̕����ɓ��͂���Ă�����A�����O����
	else if (m_gameCamera->GetStateStick() == enStick_EnterStickBothOppositeLeft) {
		if (m_count == 0) {
			m_timer = 0.0f;
			m_isRight = true;
		}
		if (m_timer <= timeLimit && m_isRight) {
			m_isRight = false;
			m_count++;
			m_timer = 0;
			m_timer = 0.0f;
		}

	}
	//�����̃X�e�B�b�N���t�̕����ɓ��͂���Ă�����A�E���O����
	else if (m_gameCamera->GetStateStick() == enStick_EnterStickBothOppositeRight) {
		if (m_count == 0) {
			m_timer = 0.0f;
			m_isRight = false;
		}
		if (m_timer <= timeLimit && !m_isRight) {
			m_count++;
			m_isRight = true;
			m_timer = 0.0f;
		}
	}
}

void Player::Collision()
{
	if (m_count2 >= 2 && m_collisionTimer >= collisionTime) {
		m_frontXZ *= m_stick.y;
		m_rightXZ *= m_stick.x;
		CVector3 addMoveSpeed = (m_frontXZ + m_rightXZ);
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
		m_moveSpeed.x = -va.x * collisionAtten;
		m_moveSpeed.z = -va.z * collisionAtten;
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/syoutotu.wav");
		se->Play(false);
		se->SetVolume(collisionVolume);
		m_collisionTimer = 0.0f;
		auto effectEngine = CEffektEngine::GetInstance();
		m_playEffectHandle = effectEngine.Play(m_hitEffect);
		effectEngine.SetPosition(m_playEffectHandle, m_charaCon.GetHitPos());
		effectEngine.SetScale(m_playEffectHandle, CVector3::One() * (m_radius / m_gameData->GetFirstPlayerSize()) * effectScale);
		//�L�����N�^�����̃R���W�����ƏՓ˂�����
		if (m_charaCon.GetisHitCharacter()) {
			//������x��������������
			if (!m_isBrake &&
				moveSpeed.LengthSq() >= (m_moveSpeedMultiply * brakeSpeed) * (m_moveSpeedMultiply * brakeSpeed) &&
				addMoveSpeed.LengthSq() >= brakeSpeed2) {
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

void Player::Brake()
{
	//�u���[�L���ĂȂ����A�ړ����x��������x��������
	if (!m_isBrake &&
		m_moveSpeed.LengthSq() >= (m_moveSpeedMultiply * brakeSpeed) * (m_moveSpeedMultiply * brakeSpeed) &&
		m_addMoveSpeed.LengthSq() >= brakeSpeed2) {

		CVector3 addMove = m_addMoveSpeed;
		CVector3 move = m_moveSpeed;
		addMove.Normalize();
		move.Normalize();
		float angle = addMove.Dot(move);
		//�ړ������ƃX�e�B�b�N�̓��͂̊p�x��������x�傫��������u���[�L
		if (fabs(acosf(angle)) >= CMath::PI * brakeAngle && m_collisionTimer >= collisionTime) {
			m_moveSpeed *= moveSpeedAtten2;
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/brake.wav");
			se->Play(false);
			se->SetVolume(brakeVolume);
			m_isBrake = true;
		}
	}
	else {
		m_isBrake = false;
	}
}

void Player::Move()
{
	//�ړ����x
	m_moveSpeedMultiply = speed * (m_radius / m_standardSize);

	m_stick = CVector3::Zero();
	//�͂܂����肵���Ƃ���
	//B�������珉���ʒu�Ɉړ�����
	if (GetPad(m_playerNumber).IsPress(enButtonB)) {
		m_respawnTimer += GameTime().GetFrameDeltaTime();
		if (m_respawnTimer >= respawnTime) {
			m_position = m_firstPosition;
			m_position.y += respawnHeight;
			m_charaCon.SetPosition(m_firstPosition);
			m_respawnTimer = 0.0f;
		}
	}
	else {
		m_respawnTimer = 0.0f;
	}

	Stick();

	//�J�E���g����萔��������_�b�V������
	if (m_timer >= timeLimit) {
		m_count = 0;
		m_isDush = false;
	}
	m_timer += GameTime().GetFrameDeltaTime();
	m_frontXZ = MainCamera(m_playerNumber).GetFront();
	m_rightXZ = MainCamera(m_playerNumber).GetRight();
	//�_�b�V����
	if (m_count >= countLimit) {
		if (m_isDush) {
			m_moveSpeed += m_frontXZ * m_moveSpeedMultiply * dushSpeed;
		}
		//�_�b�V�����̍ŏ��͂�����x�̈ړ����x��ݒ肷��
		else {
			for (int i = 0; i < roopDush; i++) {
				m_moveSpeed += m_frontXZ * m_moveSpeedMultiply * dushSpeed;
				m_moveSpeed *= moveSpeedAtten;
			}
			m_isDush = true;
		}
	}
	//��_�b�V����
	else {
		m_frontXZ *= m_stick.y;
		m_rightXZ *= m_stick.x;
		m_addMoveSpeed = (m_frontXZ + m_rightXZ);
		m_addMoveSpeed.y = 0.0f;

		Brake();

		m_moveSpeed += m_addMoveSpeed * m_moveSpeedMultiply;
		m_moveSpeed.y -= gravityMoveSpeed * GameTime().GetFrameDeltaTime();

		if (!m_gameData->GetisStart()) {
			m_moveSpeed.x = 0.0f;
			m_moveSpeed.z = 0.0f;
		}
		m_position = m_charaCon.Execute(GameTime().GetFrameDeltaTime(), m_moveSpeed);
		//�Փ˂�����
		if (m_charaCon.GetisCollision()) {
			Collision();
		}
		else {
			m_count2 = 0;
			m_collisionTimer += GameTime().GetFrameDeltaTime();
		}
		if (m_charaCon.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
	}
	m_moveSpeed.x *= moveSpeedAtten;
	m_moveSpeed.z *= moveSpeedAtten;
	m_charaCon.SetPosition(m_position);

	m_skinModelRender.SetPosition(m_position + CVector3::AxisY() * m_radius);
	CVector3 pos = MainCamera(m_playerNumber).GetFront();
	pos = pos * m_radius * 1.2f;
	pos = m_position - pos;
	m_skinModelRender2.SetPosition(pos);
	m_skinModelRender.SetRotation(m_rotation);
	CVector3 pos2 = MainCamera(m_playerNumber).GetFront();
	CQuaternion rot2;
	rot2.SetRotation(CVector3::AxisY(), atan2f(pos2.x, pos2.z));
	m_skinModelRender2.SetRotation(rot2);
}

void Player::Turn()
{
	const float rotationSpeed = 1.0f * (m_protMoveSpeedMultiply / m_moveSpeedMultiply); //-((1.0f * (m_radius / m_protradius)) - 1.0f);

	CVector3 movespeedXZ = m_position - m_beforePosition;
	movespeedXZ.y = 0.0f;
	if (movespeedXZ.LengthSq() <= 0.01f) {
		return;
	}
	float lengh = movespeedXZ.Length();
	//�m�[�}���C�Y���Ĉړ������̕����x�N�g�������߂�
	movespeedXZ /= lengh;
	//Y���ƕ����x�N�g���̊O�σx�N�g�������߂�
	CVector3 pos;
	pos.Cross(CVector3::AxisY(), movespeedXZ);
	//�O�σx�N�g�������ɉ�]������N�H�[�^�j�I�������߂�
	CQuaternion rot;
	rot.SetRotationDeg(pos, lengh * rotationSpeed);
	//���߂��N�H�[�^�j�I������Z����
	m_rotation.Multiply(rot,m_rotation);

}

void Player::ScreenPosition()
{
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
	if (m_gameData->GetScene() == enScene_Stage) {
		wchar_t output[256];
		int playerSize = int(m_radius * 2.0f);
		int goalSize = int(m_gameData->GetGoalPlayerSize());
		swprintf_s(output, L"%d\n", playerSize);
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