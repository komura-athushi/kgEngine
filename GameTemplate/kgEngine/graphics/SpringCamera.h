#pragma once
//�o�l�J����
class SpringCamera
{
public:
	SpringCamera() {}
	~SpringCamera() {}
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="maxMoveSpeed">�ő呬�x</param>
	/// <param name="attenuationRate">������</param>
	void Init(
		float maxMoveSpeed,
		float attenuationRate,
		const CVector3& position,
		const CVector3& target
	);
	/// <summary>
	/// ���W���v�Z
	/// </summary>
	/// <param name="positionNow">���̍��W</param>
	/// <param name="positionTarget">�ڕW�̍��W</param>
	/// <param name="moveSpeed">�ړ����x</param>
	/// <returns></returns>
	CVector3 ClcSpringVector(
		const CVector3& positionNow,
		const CVector3& positionTarget,
		CVector3& moveSpeed
		);
	//�X�V
	void Update();
	//���W��ݒ�
	void SetPosition(const CVector3 position)
	{
		m_position = position;
	}
	//���W���擾
	CVector3 GetPosition() const
	{
		return m_position;
	}
	//�����_��ݒ�
	void SetTarget(const CVector3 target)
	{
		m_target = target;
	}
	//�����_���擾
	CVector3 GetTarget() const
	{
		return m_target;
	}
	//���t���b�V����ݒ肷��
	void SetRefresh(bool isRefresh)
	{
		m_isRefresh = isRefresh;
	}
	//�J�������X�g�b�v����
	void SetStop()
	{
		m_isStop = true;
	}
private:
	CVector3 m_position = CVector3::Zero();					//���W
	CVector3 m_target = CVector3::Zero();					//�����_
	float m_attenuationRate = 0.0f;							//������
	CVector3 m_positionMoveSpeed = CVector3::Zero();		//���W�̈ړ����x
	CVector3 m_targetMoveSpeed = CVector3::Zero();			//�����_�̈ړ����x
	float m_maxMoveSpeed = 0.0f;							//�ō��ړ����x
	bool m_isRefresh = false;								//�J�����̍��W�⒍���_����C�ɐ؂�ւ���
	bool m_isStop = false;
};

