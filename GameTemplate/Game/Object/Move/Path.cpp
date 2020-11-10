#include "stdafx.h"
#include "Path.h"

Path::Path()
{

}

Path::~Path()
{

}

void Path::Load(const wchar_t* filePath)
{
	Skeleton skl;
	if (!skl.Load(filePath)) {
		//パスを読み込めなかった
		return;
	}
	m_pointList.resize(skl.GetNumBones() - 1);
	for (int i = 1; i < skl.GetNumBones(); i++) {
		Bone* bone = skl.GetBone(i);
		CMatrix ma = bone->GetBindPoseMatrix();
		CVector3 pos;
		pos.x = ma.m[3][0];
		pos.y = ma.m[3][2];
		pos.z = -ma.m[3][1];
		//文字列をint型に変換
		int pointNm = _wtoi(bone->GetName());
		Point point;
		point.s_vector = pos;
		point.s_number = pointNm;
		m_pointList[pointNm - 1] = point;
	}
}

Point* Path::GetNearPoint(const CVector3& pos)
{
	Point* point = &m_pointList[0];
	CVector3 diff = m_pointList[0].s_vector - pos;
	//今の場所から一番近いポイント探す
	for (int i = 1; i < m_pointList.size(); i++) {
		CVector3 diff2 = m_pointList[i].s_vector - pos;
		if (diff.LengthSq() > diff2.LengthSq()) {
			diff = diff2;
			point = &m_pointList[i];
		}
	}
	return point;
}

Point* Path::GetPoint(const int& number)
{
	//番号が最後のポイントだったら最初のポイント
	if (number == m_pointList.size()) {
		return &m_pointList[0];
	}
	else {
		return &m_pointList[number];
	}
}