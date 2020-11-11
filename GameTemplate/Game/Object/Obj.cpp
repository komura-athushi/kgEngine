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
	const float seVolume = 0.25f;		//�������܂ꂽ�Ƃ���SE�̃{�����[��
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
	//�C���X�^���V���O�`��̂��߂̑O����
	BeginUpdateInstancingData();
}

ObjModelData* ObjModelDataFactory::Load(const wchar_t* path) 
{
	//���O����n�b�V���^�O�𐶐�
	int key = Util::MakeHash(path);
	//
	if (m_modelMap.count(key) == 0) {
		wchar_t filepath[256];
		swprintf_s(filepath, L"Resource/modelData/%ls.cmo", path);
		//std::make_unique  �X�}�[�g�|�C���^�����̃w���p�[�֐�
		//new���s�v�ɂȂ�A()�ɏ����l
		m_modelMap.emplace(key, std::make_unique<ObjModelData>());
		//���f�������[�h
		m_modelMap[key].get()->s_skinModel.Init(filepath, nullptr, 0, enFbxUpAxisZ, true);
		SetPriorityGO(&m_modelMap[key].get()->s_skinModel, 2);
		m_modelMap[key].get()->s_hashKey = key;
		auto skinModel = std::make_unique<SkinModel>();
		skinModel->Init(filepath);
		//���L�������[�u����B
		m_skinModelMap.emplace(key, std::move(skinModel));
	}
	m_modelMap[key].get()->s_maxInstance += 1;
	m_modelMap[key].get()->s_skinModel.SetInstanceNumber(m_modelMap[key].get()->s_maxInstance);
	//get�Ń}�b�s���O����Ă���l
	return m_modelMap[key].get();
}

void ObjModelDataFactory::InitInstancingData()
{
	//�e���f���̃C���X�^���V���O�f�[�^��������
	if (m_modelMap.size() != 0) {
		for (auto itr = m_modelMap.begin(); itr != m_modelMap.end(); ++itr) {
			itr->second.get()->s_skinModel.InitInstancing();
		}
	}
}

void ObjModelDataFactory::BeginUpdateInstancingData()
{
	//�C���X�^���V���O���J�n���邼��
	if (m_modelMap.size() != 0) {
		for (auto itr = m_modelMap.begin(); itr != m_modelMap.end(); ++itr) {
			itr->second.get()->s_skinModel.BeginUpdateInstancingData();
		}
	}
}

void ObjModelDataFactory::DeleteAllData()
{
	//�}�b�v�N���A�[
	m_modelMap.clear();
}

Obj::Obj()
{

}

Obj::~Obj()
{
	//�X�e�[�W�N���A���Ă���
	if (m_gameData->GetScene() == enScene_Result && m_gameData->GetisGameClear()) {
		//�}�ӂŕ\������悤�ɐݒ肷��
		GetObjectData().SetisHit(m_objData->s_volume);
	}
}

void Obj::SetFilePath(const wchar_t* path)
{
	//���f�������[�h
	m_modelData = GetObjModelDataFactory().Load(path);
	//�A�j���[�V�������ݒ肳��Ă�����
	if (m_objData->s_isAnimation == 1) {
		//�A�j���[�V������ǂݍ���
		m_anim.Init(m_objData->s_name, &m_modelData->s_skinModel);
	}
}

bool Obj::Start()
{
	m_size = (m_objData->s_x + m_objData->s_y + m_objData->s_z) / 3;
	//���̂ł���
	if (m_objData->s_isSphere == 1) {
		m_isSphere = true;
		m_size = m_objData->s_x;
		m_lenght = m_size * 2;
		m_radius = m_objData->s_x;
		m_staticObject.CreateSphereObject(m_size, m_position, m_rotation);
	}
	else {
		//���b�V���R���C�_�[����Ȃ��H
		if (m_objData->s_isMeshCollider == 0) {
			m_staticObject.CreateBoxObject(m_position, m_rotation, { m_objData->s_x * 2,m_objData->s_y * 2,m_objData->s_z * 2 });
		}
		//���b�V���R���C�_�[
		else {
			m_staticObject.CreateMeshObject(&m_modelData->s_skinModel, m_position, m_rotation);
		}
		m_lenght = (m_objData->s_x + m_objData->s_y + m_objData->s_z) * 2;
		m_radius = pow(m_objData->s_volume, 1.0f / 3.0f) / 2.0f;
	}
	//�������܂ꂽ�Ƃ��ɃK�^�K�^����������H
	if (m_objData->s_isLineSegment == 1) {
		m_isLineSegment = true;
		m_lineVector = m_objData->s_lineVector;
	}
	//���ʔ��˂���H
	if (m_objData->s_isSpec == 1) {
		TextureData textureData;
		wchar_t output[256];
		swprintf_s(output, L"Resource/sprite/%ls.dds", m_objData->s_specName);
		textureData.specFilePath = output;
		//�X�y�L�����}�b�v��ǂݍ���
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
		//�o�g�����[�h�̓X�e�[�W1
		swprintf_s(aiueo, L"Assets/path/stage1/%ls%d.tks", m_objData->s_name, number);
		
	}
	else {
		swprintf_s(aiueo, L"Assets/path/stage%d/%ls%d.tks", GetGameData().GetStageNumber(), m_objData->s_name, number);
	}
	//�p�X��ǂݍ���
	mp->ReadPath(aiueo);
	m_move = std::move(mp);
}

void Obj::InitMove(EnMove state, const CVector3& pos, const float move, const float movelimit, const CQuaternion& rot)
{
	//�ړ��p�̃N���X�̃C���X�^���X�𐶐�
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
		//������new���Ȃ�
		break;
	default:
		m_move = std::make_unique<MoveNone>();
		break;
	}
	
	//������
	m_move->Init(pos, move, movelimit, rot);
	m_move->SetMoveState();

	m_moveState = state;
	m_position = pos;
	m_rotation = rot;
}

void Obj::InitRot(EnRotate state, const float speed)
{
	//��]�p�̃N���X�̃C���X�^���X�𐶐�
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
	//������
	m_rot->Init(m_rotation, speed);
	m_rotState = state;
}

void Obj::ClcVertex()
{
	//�������܂�ĂȂ�
	if (m_moveState != enMove_MoveHit) {
		m_box.Update(m_position,m_rotation);
	}
	//�������܂ꂽ
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
	//�v���C���[�̋t�s������߂�
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//�I�u�W�F�N�g�̃��[���h�s��ƃv���C���[�̋t�s�����Z���āA
	//�v���C���[����Ƃ����I�u�W�F�N�g�̃��[�J���s������߂�
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix objworldMatrix,transMatrix, rotMatrix, scaleMatrix;
	//���s�ړ��s����쐬����B
	transMatrix.MakeTranslation(m_position);
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	//�g��s����쐬����B
	scaleMatrix.MakeScaling(CVector3::One());
	//���[���h�s����쐬����B
	//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
	//���Ԃ��ԈႦ���猋�ʂ��ς���B
	objworldMatrix.Mul(scaleMatrix, rotMatrix);
	objworldMatrix.Mul(objworldMatrix, transMatrix);
	m_localMatrix.Mul(objworldMatrix, ReverseMatrix);
	m_moveState = enMove_MoveHit;
	m_staticObject.Release();
	if (m_isLineSegment) {
		//�K�^�K�^�����̂��߂̏�����
		m_lineSegment.Init(m_position);
		m_lineSegment.GetRigidBody()->GetBody()->setUserIndex(4);
		m_lineSegment.SetPlayer(m_player);
	}
	m_box.Init(CVector3(m_objData->s_x, m_objData->s_y, m_objData->s_z));
	OffScreen* offScreen = FindGO<OffScreen>();
	ObjModelDataFactory* factory = &GetObjModelDataFactory();
	offScreen->SetSkinModel(factory->GetSkinModel(m_modelData->s_hashKey));
	offScreen->SetObjData(m_objData);

	//�������܂ꂽ�特�o��
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
	//���[�J���s��ƃv���C���[�̃��[���h�s�����Z
	m_worldMatrix.Mul(m_localMatrix, m_player->GetCSkinModelRender().GetSkinModel().GetWorldMatrix());
}

void Obj::MoveRotation()
{
	//�|�[�Y�������邢�̓��U���g��ʂ���Ȃ�������
	if (!m_gameData->GetisPose() || m_gameData->GetScene() == enScene_Result) {
		//�������܂�Ă�
		if (m_moveState == enMove_MoveHit) {
			ClcMatrix();
			if (m_isLineSegment) {
				ClcVertex();
				if (m_isClcLineSegment) {
					m_lineSegment.Execute(m_player->GetPosition(), m_lineVector);
				}
			}
		}
		//�������܂�Ă��Ȃ�
		else {
			//�ړ�
			m_position = m_move->Move();
			m_staticObject.SetPosition(m_position);

			//��]
			m_rotation = m_rot->Rot(m_move->GetMoveVector());
			m_staticObject.SetRotation(m_rotation);

			//�����蔻�菈���A���������]���ĂȂ������肵����X�V���Ȃ�
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
	//�������܂ꂽ���񂾂����s����
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

	//�ړ���]����
	MoveRotation();

	//�C���X�^���V���O����
	if (m_moveState == enMove_MoveHit) {
		m_modelData->s_skinModel.UpdateInstancingData(m_worldMatrix, m_anim.GetPlayAnimationType());
	}
	else {
		m_modelData->s_skinModel.UpdateInstancingData(m_position, m_rotation, CVector3::One(), m_anim.GetPlayAnimationType());
	}
	//�A�j���[�V����
	m_anim.PlayAnimation(m_moveState);
}

