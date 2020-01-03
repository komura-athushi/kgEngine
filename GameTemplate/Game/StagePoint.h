#pragma once
class StageSelectGround;
class StagePoint : public IGameObject
{
public:
	StagePoint();
	~StagePoint();
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
	//�ԍ���ݒ�
	void SetNumber(int number)
	{
		m_number = number;
	}
	//�ԍ����擾
	int GetNumber() const
	{
		return m_number;
	}
	//�傫����ݒ�
	void SetScale(const CVector3& scale)
	{
		m_scale = scale;
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
	int m_number = 0;										//�|�C���g�̔ԍ�
	StageSelectGround* m_stageSelectGround = nullptr;		//������
	bool m_isFind = false;									//���������������H
};

