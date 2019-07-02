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
		pos.z = ma.m[3][1];
		//文字列をint型に変換
		int pointNm = _wtoi(bone->GetName());
		Point point;
		point.s_vector = pos;
		point.s_number = pointNm;
		m_pointList[pointNm - 1] = point;
	}
}

Point* Path::GetPoint(const int& number)
{
	if (number == m_pointList.size()) {
		return &m_pointList[0];
	}
	else {
		return &m_pointList[number];
	}
}