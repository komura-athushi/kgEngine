/*****************************************************************//**
 * \file   Player.h
 * \brief  Player�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "character/CharacterController.h"
#include "graphics/2D/kgFont.h"
class GameCamera;
class GameData;
/**
 * \brief �v���C���[(��).
 */
class Player:public IGameObject
{
public:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Player();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Player() override final;
	/**
	 * \brief ����������.
	 * 
	 * \return true�ŏ���������
	 */
	bool Start() override final;
	/**
	 * \brief �X�V����.
	 * 
	 */
	void Update() override final;
	/**
	 * \brief �`�揈��.
	 * 
	 */
	void PostRender() override final;
	/**
	 * \brief �v���C���[��傫������.
	 * 
	 * \param volume �傫�����镪
	 */
	void AddVolume(const float volume);
	/**
	 * \brief ���W���擾.
	 * 
	 * \return ���W
	 */
	CVector3 GetPosition() const
	{
		return m_position + CVector3::AxisY() * m_radius;
	}
	/**
	 * \brief �L�����R���̍��W���擾.
	 * 
	 * \return �L�����R���̍��W
	 */
	const CVector3& GetCharaconPosition() const
	{
		return m_position;
	}
	/**
	 * \brief ���W��ݒ�.
	 * 
	 * \param pos ���W
	 */
	void SetPosition(const CVector3& pos)
	{
#ifdef _DEBUG
		if (isnan(pos.x) || isnan(pos.y) || isnan(pos.z)) {
			std::abort();
		}
#endif
		m_position = pos;
		m_charaCon.SetPosition(pos);
		m_skinModelRender.SetPosition(pos + CVector3::AxisY() * m_radius);
		if (m_isStart) {
			m_skinModelRender.UpdateWorldMatrix();
			m_skinModelRender2.UpdateWorldMatrix();
		}
	}
	/**
	 * \brief �X�e�[�W�̍ŏ�����ǂꂾ���傫���Ȃ��������擾.
	 * 
	 * \return �X�e�[�W�̍ŏ�����ǂꂾ���傫���Ȃ�����
	 */
	const float GetMag() const
	{
		return m_scale.y / 1.0f;
	}
	/**
	 * \brief ��̔��a���擾.
	 * 
	 * \return ��̔��a 
	 */
	const float GetRadius() const
	{
		return m_radius;
	}
	/**
	 * \brief �X�e�[�W�̍ŏ��̉�̑傫�����擾.
	 * 
	 * \return  �X�e�[�W�̍ŏ��̉�̑傫��
	 */
	const float GetProtRadius() const
	{
		return m_protRadius;
	}
	/**
	 * \brief ��̃��f�����擾.
	 * 
	 * \return ��̃��f��
	 */
	CSkinModelRender& GetCSkinModelRender()
	{
		return m_skinModelRender;
	}
	/**
	 * \brief �v���C���[��Y�̈ړ����x��0�ɂ���.
	 * 
	 */
	void SetMoveSpeedYZero()
	{
		m_moveSpeed.y = 0.0f;
	}
	/**
	 * \brief �J�E���g���擾.
	 * 
	 * \return �J�E���g
	 */
	const int GetCount() const
	{
		return m_count;
	}
	/**
	 * \brief ��̃v���C���[�̑傫�����擾.
	 * 
	 * \return ��̃v���C���[�̑傫�� 
	 */
	const float GetStandardSize() const
	{
		return m_standardSize;
	}
	/**
	 * \brief ��̑傫����\�����Ȃ�.
	 * 
	 */
	void SetisNotDisplay()
	{
		m_isDisplayFont = false;
	}
	/**
	 * \brief �^�C�g������.
	 * 
	 */
	void SetisTitle()
	{
		m_isTitle = true;
	}
	/**
	 * \brief ��̃X�N���[�����W�n���擾.
	 * 
	 * \return ��̃X�N���[�����W�n
	 */
	const CVector3& GetScreenPos() const
	{
		return m_katamariVector;
	}
	/**
	 * \brief 1P��2P����ݒ�(0��1P).
	 * 
	 * \param number 0��1P
	 */
	void SetPlayerNumber(const int number)
	{
		m_playerNumber = number;
	}
	/**
	 * \brief 1P��2P���擾.
	 * 
	 * \return 0��1P
	 */
	const int GetPlayerNumber() const
	{
		return m_playerNumber;
	}
	/**
	 * \brief �Q�[���J������ݒ�.
	 * 
	 * \param gameCamera �Q�[���J����
	 */
	void SetGameCamera(GameCamera* gameCamera)
	{
		m_gameCamera = gameCamera;
	}
	/**
	 * \brief �v���C���[�̍ŏ��̍��W��ݒ�.
	 * 
	 * \param position �v���C���[�̍ŏ��̍��W
	 */
	void SetFirstPosition(const CVector3& position)
	{
		m_firstPosition = position;
		m_position = position;
	}
	/**
	 * \brief �����Е��̃v���C���[��ݒ�.
	 * 
	 * \param player �����Е��̃v���C���[
	 */
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/**
	 * \brief �ꎞ��~������.
	 * 
	 */
	void SetStopTime()
	{
		m_isStopTime = true;
	}
	/**
	 * \brief �ꎞ��~���Ă邩�ǂ���.
	 * 
	 * \return �ꎞ��~���Ă���true
	 */
	const bool GetisStopTime() const
	{
		return m_isStopTime;
	}
private:
	/**
	 * \brief �ړ�.
	 * 
	 */
	void Move();
	/**
	 * \brief �X�e�B�b�N����.
	 * 
	 */
	void Stick();
	/**
	 * \brief �Փˏ���.
	 * 
	 */
	void Collision();
	/**
	 * \brief ��].
	 * 
	 */
	void Turn();
	/**
	 * \brief �X�N���[�����W�v�Z.
	 * 
	 */
	void ScreenPosition();
private:
	CharacterController m_charaCon;																//�L�����R��
	CVector3 m_position = { 0.0f,200.0f,0.0f }, m_beforePosition = CVector3::Zero();			//���W
	CVector3 m_moveSpeed = CVector3::Zero();													//�ړ��x�N�g��
	CVector3 m_scale = CVector3::One();															//�傫��
	CQuaternion m_rotation = CQuaternion::Identity();											//��]
	//�A�j���[�V����
	enum EnAnimationClip {
		enAnimationClip_idle,
		enAnimationClip_walk,
		enAnimationClip_run,
		enAnimationClip_jump,
		//enAnimationClip_damage,
		//enAnimationClip_KneelDown,
		//enAnimationClip_Clear,
		//enAnimationClip_Aria,
		enAnimationClip_num,
	};
	AnimationClip m_animClip[enAnimationClip_num];
	enum EnState {
		enState_idle,
		enState_walk,
		enState_jump,
		enState_num
	};
	EnState m_state = enState_idle;
	CSkinModelRender m_skinModelRender;															//�X�L�����f���B
	CSkinModelRender m_skinModelRender2;														//�l�̃��f��
	GameCamera* m_gameCamera = nullptr;															//�J����
	bool m_isJump = false;																		//�W�����v�����ǂ���
	float m_radius = 0.0f;																		//���̂̔��a
	float m_protRadius = 0.0f;																	//�ŏ��̋��̂̔��a	

	float m_volume = 0.0f;																		//��̑̐�																	
	const float m_protMoveSpeedMultiply = 5.0f;													//�ŏ��̈ړ����x
	float m_moveSpeedMultiply = 5.0f;															//�ړ����x
	bool m_isBound = false;																		//�o�E���h�����ǂ���
	CFont m_font;																				//����
	float m_timer = 0.0f;																		//�_�b�V���̔���Ɏg���^�C�}�[
	int m_count = 0;																			//�_�b�V���̔���Ɏg���J�E���g	
	bool m_isRight = false;																		//�X�e�B�b�N�̉E���O�����A�����������ɓ|���ꂽ��
	bool m_isDush = false;																		//�_�b�V��������
	GameData* m_gameData = nullptr;																//�Q�[���f�[�^
	/*CSprite m_sprite;
	CSprite m_sprite2;
	float m_degree = 0.0f;*/
	const float m_standardSize = 40.0f;															//��̉�̒��a
	CVector3 m_impulse = CVector3::Zero();														//���͎g���Ă��Ȃ�
	int m_count2 = 0;																			//�ǂɂԂ�����
	bool m_isDisplayFont = true;																//��̑傫����\������H
	bool m_isTitle = false;																		//�^�C�g���H
	bool m_isBrake = false;																		//�u���[�L�H
	float m_collisionTimer = 0.0f;																//�Փˉ���A���ŏo���Ȃ����߂̃^�C�}�[
	CVector3 m_katamariVector = CVector3::One();												//��̃X�N���[�����W�n
	int m_playerNumber = 0;
	CVector3 m_firstPosition = CVector3::Zero();
	Effekseer::Handle m_playEffectHandle = 1;
	//�G�t�F�N�g
	Effekseer::Effect* m_hitEffect = nullptr;
	Effekseer::Handle m_smokeEffectHandle = 1;
	//�G�t�F�N�g
	Effekseer::Effect* m_smokeEffect = nullptr;
	Player* m_player = nullptr;																	//������l�̃v���C���[
	bool m_isStopTime = false;
	float m_stopTimer = 0.0f;
	float m_respawnTimer = 0.0f;
	CVector3 m_stick = CVector3::Zero();
	CVector3 m_frontXZ = CVector3::Zero();
	CVector3 m_rightXZ = CVector3::Zero();

};

