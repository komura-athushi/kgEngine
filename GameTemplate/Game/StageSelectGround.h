#pragma once
class StageSelectGround : public IGameObject
{
public:
	StageSelectGround();
	~StageSelectGround();
	bool Start() override;
	void Update() override;
	//���W��ݒ�
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	//�ړ����x�����߂�
	const CVector3& GetMoveSpeed() const
	{
		return m_currentPosition - m_beforePosition;
	}
	//���f�����擾
	CSkinModelRender& GetModel()
	{
		return  m_skinModelRender;
	}
private:
	//��]
	void Turn();
	//�ړ�
	void Move();
private:
	CSkinModelRender m_skinModelRender;									//�������̃��f��
	CVector3 m_position;												//���W
	CQuaternion m_rotation = CQuaternion::Identity();					//��]
	CVector3 m_currentPosition, m_beforePosition = CVector3::Zero();	//�ړ�����O�ƌ�̍��W

};

