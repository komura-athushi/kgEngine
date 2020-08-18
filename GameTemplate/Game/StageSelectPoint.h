#pragma once

class StageSelectGround;

enum enPoint {
	enPoint_nun,
	enPoint_Stage1 = 1,
	enPoint_Stage2 = 2,
	enPoint_Collection,
	enPoint_Battle
};

class StageSelectPoint:public IGameObject
{
public:
	StageSelectPoint();
	~StageSelectPoint();
	bool Start() override;
	void Update() override;
	//���W��ݒ�
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	//���W���擾
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//��]��ݒ�
	void SetRotation(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	//�傫����ݒ�
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
	}
	//������ݒ�
	void SetPoint(enPoint point)
	{
		m_enPoint = point;
	}
	//�������擾
	enPoint GetPoint()
	{
		return m_enPoint;
	}
private:
	//���[�J���s����v�Z
	void ClcLocalMatrix();
private:
	CSkinModelRender m_model;								//�X�L�����f�������_�[
	CVector3 m_position;									//���W		
	CQuaternion m_rotation;									//��]
	CVector3 m_scale;										//�傫��
	CMatrix m_localMatrix;									//���[�J���s��
	CMatrix m_worldMatrix;									//���[���h�s��
	StageSelectGround* m_stageSelectGround = nullptr;		//������
	bool m_isFind = false;									//���������������H
	enPoint m_enPoint = enPoint_nun;
};

