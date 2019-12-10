#include "stdafx.h"
#include "CollectionBook.h"
#include "StageSelectGround.h"
#include "GameData.h"
CollectionBook::CollectionBook()
{

}

CollectionBook::~CollectionBook()
{

}

bool CollectionBook::Start()
{

	m_model.Init(L"Resource/modelData/book.cmo");

	m_model.SetPosition(m_position);
	m_model.SetRotation(m_rotation);
	m_model.SetScale(m_scale);
	m_model.UpdateWorldMatrix();
	ClcLocalMatrix();
	m_worldMatrix.Mul(m_localMatrix, m_stageSelectGround->GetMatrix());
	m_model.SetWorldMatrix(m_worldMatrix);
	return true;
}

void  CollectionBook::ClcLocalMatrix()
{
	m_stageSelectGround = FindGO<StageSelectGround>();
	CMatrix worldMatrix = m_stageSelectGround->GetMatrix();
	//プレイヤーの逆行列を求める
	CMatrix ReverseMatrix;
	ReverseMatrix.Inverse(worldMatrix);
	//オブジェクトのワールド行列とプレイヤーの逆行列を乗算して、
	//プレイヤーを基準としたオブジェクトのローカル行列を求める
	//3dsMaxと軸を合わせるためのバイアス。
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix objworldMatrix, transMatrix, rotMatrix, scaleMatrix;
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
}

void  CollectionBook::Update()
{
	m_worldMatrix.Mul(m_localMatrix, m_stageSelectGround->GetMatrix());
	m_model.SetWorldMatrix(m_worldMatrix);
	//m_model.UpdateWorldMatrix();

	m_position.x = m_model.GetSkinModel().GetWorldMatrix().m[3][0];
	m_position.y = m_model.GetSkinModel().GetWorldMatrix().m[3][1];
	m_position.z = m_model.GetSkinModel().GetWorldMatrix().m[3][2];
}