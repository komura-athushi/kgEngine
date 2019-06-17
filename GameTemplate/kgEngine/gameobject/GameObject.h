#pragma once
#include "..//Noncopyable.h"

//typedef �����̌^��V�����^�ɂ�����Ă����A���O���ς�邾������
typedef unsigned char GameObjectPrio;
//GameObject Update�֐��Ƃ��g�����������炱����p��
class IGameObject :Noncopyable
{
public:
	using IGameObjectIsBase = IGameObject;
	//�R���X�g���N�^�A�������q�ŏ����������
	IGameObject();
	//�f�X�g���N�^
	virtual ~IGameObject();

public:
	//Update���O�ɌĂ΂��J�n����
	virtual bool Start() { return true; }

	//Update
	virtual void  Update() {}

	//PreUpdate
	virtual void PreUpdate() {}
	//���s�D��x���擾
	GameObjectPrio GetPriority() const
	{
		return m_priority;
	}

	//�`��
	virtual void Render() {}

	//3D���f���`��A������
	virtual void Draw() {}

	//�폜�̒��O�ɌĂ΂��֐�
	virtual void OnDestroy() {}
public :
	//�|�X�g�G�t�F�N�g�̌�Ŏ��s�����A�|�X�g�G�t�F�N�g�̉e�����󂯂����Ȃ��ꍇ�͂�����
	virtual void PostRender() {}

	//���S�t���O�����Ă�
	void SetDead()
	{
		m_isDead = true;
	}

	//���S����
	bool isDead() const
	{
		return m_isDead;
	}

	//�X�^�[�g�֐��������������ǂ���
	virtual bool isStart() const
	{
		return m_isStart;
	}

	//�A�N�e�B�u���ǂ����𔻒�
	bool isActive() const
	{
		return m_isActive;
	}

	//�A�N�e�B�u�t���O��ݒ�
	void SetActive(bool flag)
	{
		m_isActive = flag;
	}

public:
	//�����𖞂�����PostRender�֐������s����
	void PostRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PostRender();
		}
	}

	//�����𖞂�����Update�֐������s����
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Update();
		}
	}

	void DrawWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Draw();
		}
	}

	void PreUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PreUpdate();
		}
	}

	//�����𖞂����Έ�x����Start�֐������s����
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead && !m_isRegistDeadList) {
			if (Start()) {
				m_isStart = true;
			}
		}
	}

	//GameObjectManager��NewGO���ꂽ�̂�ݒ肷��
	void SetMarkNewFromGameObjectManager()
	{
		m_isNewFromGameObjectManager = true;
	}

	//GameObjectManager��NewGO���ꂽ���ǂ������擾����
	bool IsNewFromgameObjectManager() const
	{
		return m_isNewFromGameObjectManager;
	}
	//friend�錾�A�t�����h�N���X�͎w�肳�ꂽ�N���X��private��protected�����o�ɃA�N�Z�X���邱�Ƃ��ł���
	friend class CGameObjectManager;
protected:
	GameObjectPrio m_priority;				//���s�D��x
	bool m_isDead = false;					//���S����
	bool m_isRegistDeadList = false;		//���S���X�g�ɓo�^����Ă��邩�ǂ���
	bool m_isStart = false;					//�X�^�[�g�֐��������������ǂ���
	bool m_isNewFromGameObjectManager;		//�Q�[���I�u�W�F�N�g�}�l�[�W���[��NewGO���ꂽ���ǂ���
	bool m_isRegist = false;				//GameObjectManager�ɓo�^����Ă��邩�ǂ���
	bool m_isActive = true;					//�A�N�e�B�u���ǂ���(Update)����΂�邩�ǂ������Ă��Ƃł�����
	unsigned int m_nameKey = 0;				//���O�L�[�A���O�̒������ˁH
};

