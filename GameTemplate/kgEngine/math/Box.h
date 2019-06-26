#pragma once
enum XYZ {
	enXYZ_X,
	enXYZ_Y,
	enXYZ_Z
};
//�o�E���f�B���O�{�b�N�X
class CBox {
public:
	CBox();
	~CBox();
	//������
	void Init(const CVector3& halfSize);
	//�X�V
	void Update(const CMatrix& worldMatrix);
	CVector3 SurfaceLineSegment(XYZ xyz);
private:
	CVector3 m_halfSize = CVector3::Zero();					//�x�N�g���̊e�v�f�̍ő�l�̔����̒l
	CVector3 m_vertexPosition[8];							//�o�E���f�B���O�{�b�N�X�̊e���_���W
	CVector3 m_vertexPosition2[6];							//�e�ʂ̒��S���W
};

