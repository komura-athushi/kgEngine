#include "KGstdafx.h"
#include "Box.h"

CBox::CBox()
{

}

CBox::~CBox()
{

}

void CBox::Init(const CVector3& halfSize)
{
	m_halfSize = halfSize;
	m_halfSize.y = halfSize.z;
	m_halfSize.z = halfSize.y;
}

void CBox::Update(const CMatrix& worldMatrix)
{
	//8頂点の座標を計算。
	m_vertexPosition[0] = CVector3::Zero();
	m_vertexPosition[0].x -= m_halfSize.x;
	m_vertexPosition[0].y -= m_halfSize.y;
	m_vertexPosition[0].z -= m_halfSize.z;

	m_vertexPosition[1] = CVector3::Zero();
	m_vertexPosition[1].x += m_halfSize.x;
	m_vertexPosition[1].y -= m_halfSize.y;
	m_vertexPosition[1].z -= m_halfSize.z;

	m_vertexPosition[2] = CVector3::Zero();
	m_vertexPosition[2].x -= m_halfSize.x;
	m_vertexPosition[2].y += m_halfSize.y;
	m_vertexPosition[2].z -= m_halfSize.z;

	m_vertexPosition[3] = CVector3::Zero();
	m_vertexPosition[3].x += m_halfSize.x;
	m_vertexPosition[3].y += m_halfSize.y;
	m_vertexPosition[3].z -= m_halfSize.z;

	m_vertexPosition[4] = CVector3::Zero();
	m_vertexPosition[4].x -= m_halfSize.x;
	m_vertexPosition[4].y -= m_halfSize.y;
	m_vertexPosition[4].z += m_halfSize.z;

	m_vertexPosition[5] = CVector3::Zero();
	m_vertexPosition[5].x += m_halfSize.x;
	m_vertexPosition[5].y -= m_halfSize.y;
	m_vertexPosition[5].z += m_halfSize.z;

	m_vertexPosition[6] = CVector3::Zero();
	m_vertexPosition[6].x -= m_halfSize.x;
	m_vertexPosition[6].y += m_halfSize.y;
	m_vertexPosition[6].z += m_halfSize.z;

	m_vertexPosition[7] = CVector3::Zero();
	m_vertexPosition[7].x += m_halfSize.x;
	m_vertexPosition[7].y += m_halfSize.y;
	m_vertexPosition[7].z += m_halfSize.z;

	//回転を加算
	for (auto& pos : m_vertexPosition) {
		worldMatrix.Apply(pos);
	}

	//右の面
	m_vertexPosition2[0] = (m_vertexPosition[1] + m_vertexPosition[3] + m_vertexPosition[5] + m_vertexPosition[7]) / 4;

	//左の面
	m_vertexPosition2[1] = (m_vertexPosition[0] + m_vertexPosition[2] + m_vertexPosition[4] + m_vertexPosition[6]) / 4;

	//一番上の面
	m_vertexPosition2[4] = (m_vertexPosition[2] + m_vertexPosition[3] + m_vertexPosition[6] + m_vertexPosition[7]) / 4;

	//一番下の面の座標
	m_vertexPosition2[5] = (m_vertexPosition[0] + m_vertexPosition[1] + m_vertexPosition[4] + m_vertexPosition[5]) / 4;

	//前方の面
	m_vertexPosition2[2] = (m_vertexPosition[4] + m_vertexPosition[5] + m_vertexPosition[6] + m_vertexPosition[7]) / 4;

	//後方の面
	m_vertexPosition2[3] = (m_vertexPosition[0] + m_vertexPosition[1] + m_vertexPosition[2] + m_vertexPosition[3]) / 4;
	int a = 0;
}

CVector3 CBox::SurfaceLineSegment(XYZ xyz) 
{
	if (xyz == enXYZ_X) {
		return m_vertexPosition2[0] - m_vertexPosition2[1];
	}
	else if (xyz == enXYZ_Y) {
		return m_vertexPosition2[2] - m_vertexPosition2[3];
	}
	else {
		if (m_vertexPosition2[4].y >= m_vertexPosition2[5].y) {
			return m_vertexPosition2[5];
		}
		else {
			return m_vertexPosition2[4];
		}
		/*if (m_vertexPosition2[4].y >= m_vertexPosition2[5].y) {
			return m_vertexPosition2[5];
		}
		else {
			return m_vertexPosition2[0];
		}*/
		//return m_vertexPosition2[4] - m_vertexPosition2[5];
	}
}