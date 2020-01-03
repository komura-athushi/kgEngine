#pragma once
class StageSelectGround;
//�{
class CollectionBook : public IGameObject
{
public:
	CollectionBook();
	~CollectionBook();
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
private:
	//���[�J���s����v�Z
	void ClcLocalMatrix();
private:
	CSkinModelRender m_model;								//�X�L�����f�������_�[
	CVector3 m_position;									//���W
	CQuaternion m_rotation;									//��]
	CVector3 m_scale = CVector3::One();						//�傫��
	CMatrix m_localMatrix;									//���[�J���s��
	CMatrix m_worldMatrix;									//���[���h�s��
	StageSelectGround* m_stageSelectGround = nullptr;		//������
	bool m_isFind = false;									//�������������H
};

