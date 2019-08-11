#pragma once

enum Enxyz {
	enState_X,
	enState_Y,
	enState_Z,
	enState_No
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
	void Update(const CVector3& pos,const CQuaternion& rot,const CVector3& scale = CVector3::One());
	//�Y���̖ʂ̒��S��y���W���v���C���[�̍��W���Ⴂ���ǂ���
	bool GetisLowPositionY(const CVector3& position, Enxyz xyz);
	//�����Ɏg�����߂̊Y���̖ʂ̒��S�̍��W���擾
	CVector3 SurfaceLineSegment(Enxyz xyz);
	//�Y���̖ʂ̒��S���W���擾
	CVector3 SurfaceVector(const int& number)
	{
		return m_vertexPosition2[number];
	}
	//�z��̑傫��
	static const int m_SurfaceVectorNumber = 6;
private:
	CVector3 m_halfSize = CVector3::Zero();					//�x�N�g���̊e�v�f�̍ő�l�̔����̒l
	CVector3 m_vertexPosition[8];							//�o�E���f�B���O�{�b�N�X�̊e���_���W
	CVector3 m_vertexPosition2[6];							//�e�ʂ̒��S���W
};

