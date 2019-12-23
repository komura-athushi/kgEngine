#include "stdafx.h"
#include "StagePoint.h"
#include "StageSelectGround.h"
#include "GameData.h"
StagePoint::StagePoint()
{

}

StagePoint::~StagePoint()
{

}

bool StagePoint::Start()
{
	auto& gameData = GameData::GetInstance();
	if (gameData.GetisStageClear(EnStageNumber(m_number))) {
		m_model.Init(L"Resource/modelData/pointclear.cmo");
	}
	else {
		m_model.Init(L"Resource/modelData/pointnoclear.cmo");
	}
	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.UpdateWorldMatrix();
	//ClcLocalMatrix();
	//m_worldMatrix.Mul(m_localMatrix, m_stageSelectGround->GetModel().GetSkinModel().GetWorldMatrix());
	//m_model.SetWorldMatrix(m_worldMatrix);
	return true;
}

void StagePoint::ClcLocalMatrix()
{
	
	CMatrix worldMatrix = m_stageSelectGround->GetModel().GetSkinModel().GetWorldMatrix();
	//�v���C���[�̋t�s������߂�
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//�I�u�W�F�N�g�̃��[���h�s��ƃv���C���[�̋t�s�����Z���āA
	//�v���C���[����Ƃ����I�u�W�F�N�g�̃��[�J���s������߂�
	//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix objworldMatrix, transMatrix, rotMatrix, scaleMatrix;
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
}

void StagePoint::Update()
{
	
	if (m_stageSelectGround == nullptr) {
		m_stageSelectGround = FindGO<StageSelectGround>();
		if (m_stageSelectGround != nullptr) {
			m_isFind = true;
			return;
		}
		
	}
	else if(m_isFind){
		ClcLocalMatrix();
		m_isFind = false;
	}



	m_worldMatrix.Mul(m_localMatrix, m_stageSelectGround->GetModel().GetSkinModel().GetWorldMatrix());
	m_model.SetWorldMatrix(m_worldMatrix);
	//m_model.UpdateWorldMatrix();

	m_position.x = m_model.GetSkinModel().GetWorldMatrix().m[3][0];
	m_position.y = m_model.GetSkinModel().GetWorldMatrix().m[3][1];
	m_position.z = m_model.GetSkinModel().GetWorldMatrix().m[3][2];
}