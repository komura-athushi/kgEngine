#include "stdafx.h"
#include "LineSegment.h"
#include "Physics/CollisionAttr.h"
#include "Player.h"

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�n�ʗp)
struct SweepResultGround : public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;									//�Փ˃t���O�B
	CVector3 hitPos = CVector3(0.0f, -FLT_MAX, 0.0f);	//�Փ˓_�B
	CVector3 startPos = CVector3::Zero();				//���C�̎n�_�B
	CVector3 hitNormal = CVector3::Zero();				//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;					//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B
	float dist = FLT_MAX;								//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B

														//�Փ˂����Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_User) {
			//�����ɏՓ˂����Bor �L�����N�^�����̃R���W�����ƏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		CVector3 hitNormalTmp = *(CVector3*)& convexResult.m_hitNormalLocal;
		//������Ɩ@���̂Ȃ��p�x�����߂�B
		float angle = hitNormalTmp.Dot(CVector3::Up());
		angle = fabsf(acosf(angle));
		if (angle < CMath::PI * 0.3f		//�n�ʂ̌X�΂�54�x��菬�����̂Œn�ʂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Ground //�������̓R���W�����������n�ʂƎw�肳��Ă���B
			) {
			//�Փ˂��Ă���B
			isHit = true;
			CVector3 hitPosTmp = *(CVector3*)& convexResult.m_hitPointLocal;
			//�Փ˓_�̋��������߂�B�B
			CVector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			float distTmp = vDist.Length();
			if (dist > distTmp) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				hitNormal = *(CVector3*)& convexResult.m_hitNormalLocal;
				dist = distTmp;
			}
		}
		return 0.0f;
	}
};

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B
	CVector3 hitPos = CVector3::Zero();		//�Փ˓_�B
	CVector3 startPos = CVector3::Zero();		//���C�̎n�_�B
	float dist = FLT_MAX;					//�Փ˓_�܂ł̋����B��ԋ߂��Փ˓_�����߂邽�߁BFLT_MAX�͒P���x�̕��������_����肤��ő�̒l�B
	CVector3 hitNormal = CVector3::Zero();	//�Փ˓_�̖@���B
	btCollisionObject* me = nullptr;		//�������g�B�������g�Ƃ̏Փ˂����O���邽�߂̃����o�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == me || convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_User) {
			//�����ɏՓ˂����Bor �n�ʂɏՓ˂����B
			return 0.0f;
		}
		//�Փ˓_�̖@�������������Ă���B
		CVector3 hitNormalTmp;
		hitNormalTmp.Set(convexResult.m_hitNormalLocal);
		//������ƏՓ˓_�̖@���̂Ȃ��p�x�����߂�B
		float angle = fabsf(acosf(hitNormalTmp.Dot(CVector3::Up())));
		if (angle >= CMath::PI * 0.3f		//�n�ʂ̌X�΂�54�x�ȏ�Ȃ̂ŕǂƂ݂Ȃ��B
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character	//�������̓R���W�����������L�����N�^�Ȃ̂ŕǂƂ݂Ȃ��B
			) {
			isHit = true;
			CVector3 hitPosTmp;
			hitPosTmp.Set(convexResult.m_hitPointLocal);
			//��_�Ƃ̋����𒲂ׂ�B
			CVector3 vDist;
			vDist.Subtract(hitPosTmp, startPos);
			vDist.y = 0.0f;
			float distTmp = vDist.Length();
			if (distTmp < dist) {
				//���̏Փ˓_�̕����߂��̂ŁA�ŋߖT�̏Փ˓_���X�V����B
				hitPos = hitPosTmp;
				dist = distTmp;
				hitNormal = hitNormalTmp;
			}
		}
		return 0.0f;
	}
};

void LineSegment::Init(const CVector3& position)
{
	m_position = position;
	m_collider.Create(m_radius);

	//���̂�������
	RigidBodyInfo rbInfo;
	rbInfo.collider = &m_collider;
	rbInfo.mass = 0.0f;
	m_rigidBody.Create(rbInfo);
	btTransform& trans = m_rigidBody.GetBody()->getWorldTransform();
	//���̂̈ʒu���X�V
	trans.setOrigin(btVector3(position.x, position.z, position.z));
	m_rigidBody.GetBody()->setUserIndex(enCollisionAttr_User);
	m_rigidBody.GetBody()->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	Engine().GetPhysicsEngine().AddRigidBody(m_rigidBody);
}

void LineSegment::Execute(const CVector3& position, const CVector3& linesegment)
{
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
	}
	m_position = position;
	//���̈ړ���ƂȂ���W���v�Z����
	CVector3 nextPosition = m_position;
	//���x���炱�̃t���[���ł̈ړ��ʂ����߂�A�I�C���[�ϕ�
	CVector3 addPos = linesegment;
	nextPosition += addPos;
	CVector3 nextPosition2 = nextPosition;

	CVector3 originalXZDir = addPos;
	originalXZDir.y = 0.0f;
	originalXZDir.Normalize();

	btRigidBody* btBody = m_rigidBody.GetBody();
	//���̂𓮂����B
	btBody->setActivationState(DISABLE_DEACTIVATION);
	btTransform& trans = btBody->getWorldTransform();
	//���̂̈ʒu���X�V�B
	trans.setOrigin(btVector3(position.x, position.y, position.z));

	//�������𒲂ׂ�B
	{
		CVector3 addPos;
		addPos.Subtract(nextPosition, m_position);

		m_position = nextPosition;	//�ړ��̉��m��B
									//���C���쐬����B
		btTransform start, end;
		start.setIdentity();
		end.setIdentity();
		//�n�_�̓J�v�Z���R���C�_�[�̒��S�B
		start.setOrigin(btVector3(position.x, position.y + m_radius, position.z));
		//�I�_�͒n�ʏ�ɂ��Ȃ��ꍇ��1m��������B
		//�n�ʏ�ɂ��Ȃ��ăW�����v�ŏ㏸���̏ꍇ�͏㏸�ʂ�0.01�{��������B
		//�n�ʏ�ɂ��Ȃ��č~�����̏ꍇ�͂��̂܂ܗ�����𒲂ׂ�B
		CVector3 endPos;
		endPos.Set(start.getOrigin());
		//�������Ă���ꍇ�͂��̂܂܉��𒲂ׂ�B
		endPos.y += addPos.y;
		end.setOrigin(btVector3(endPos.x, endPos.y, endPos.z));
		SweepResultGround callback;
		callback.me = m_rigidBody.GetBody();
		callback.startPos.Set(start.getOrigin());
		//�Փˌ��o�B
		if (fabsf(endPos.y - callback.startPos.y) > FLT_EPSILON) {
			Engine().GetPhysicsEngine().ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), start, end, callback);
			if (callback.isHit) {
				//���������B
				addPos.y = 0.0f;
				m_isJump = false;
				m_isOnGround = true;
				nextPosition.y = callback.hitPos.y;
			}
		}
	}
	//@todo ���Ή��B trans.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z));
	CVector3 vector = nextPosition2 - nextPosition;
	CVector3 pos = m_player->GetPosition();
	if (fabsf(pos.y) >= 0.1f) {
		m_player->SetMoveSpeedYZero();
	}
	pos.y -= vector.y;
	//pos += vector;
	m_player->SetPosition(pos - CVector3::AxisY() * m_player->GetRadius());
}