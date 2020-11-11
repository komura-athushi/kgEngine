/*****************************************************************//**
 * \file   GameData.h
 * \brief  GameData�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

enum EnStageNumber {
	enState_Stage1 = 1,
	enState_Stage2 = 2,
	enState_Stage3 = 3,
	enState_FirstStage = enState_Stage1,
	enState_LastStage = enState_Stage3
};
enum EnScene {
	enScene_Title,
	enScene_StageSelect,
	enScene_Stage,
	enScene_Result
};

/**
 * \brief �Q�[�����̐F�X�ȃf�[�^���Ǘ�.
 */
class GameData
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	GameData() {}
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~GameData() {}
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static GameData& GetInstance()
	{
		static GameData instance;
		return instance;
	}
	/**
	 * \brief �X�e�[�W�̃N���A�󋵂����[�h����.
	 * 
	 */
	void LoadDataStageClear();
	/**
	 * \brief �X�e�[�W�̃N���A�󋵂��Z�[�u����.
	 * 
	 */
	void SaveDataStageClear();
	/**
	 * \brief �X�e�[�W���N���A����.
	 * 
	 */
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	/**
	 * \brief �Y���̃X�e�[�W���N���A���Ă邩�ǂ���.
	 * 
	 * \param number �X�e�[�W�̔ԍ�
	 * \return �X�e�[�W�N���A���Ă�����true
	 */
	bool GetisStageClear(const EnStageNumber& number) const
	{
		return m_stageClearList[number - 1];
	}
	/**
	 * \brief �v���C����X�e�[�W�̔ԍ���ݒ�.
	 * 
	 * \param number �v���C����X�e�[�W�̔ԍ�
	 */
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	/**
	 * \brief �v���C���Ă���X�e�[�W�̔ԍ����擾.
	 * 
	 * \return �v���C���Ă���X�e�[�W�̔ԍ�
	 */
	const EnStageNumber GetStageNumber() const
	{
		return m_stageNumber;
	}
	/**
	 * \brief �v���C����X�e�[�W�̃v���C���[�̍ŏ��̑傫�����擾.
	 * 
	 * \return �v���C����X�e�[�W�̃v���C���[�̍ŏ��̑傫�����擾
	 */
	const float GetPlayerSize() const
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	/**
	 * \brief �X�e�[�W1�̃v���C���[�̍ŏ��̑傫�����擾.
	 * 
	 * \return �X�e�[�W1�̃v���C���[�̍ŏ��̑傫��
	 */
	const float GetFirstPlayerSize() const
	{
		return m_firstPlayerSize[0];
	}
	/**
	 * \brief �v���C����X�e�[�W�̐������Ԃ��擾.
	 * 
	 * \return �v���C����X�e�[�W�̐�������
	 */
	const float GetStageLimitTime() const
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	/**
	 * \brief �v���C����X�e�[�W�̃v���C���[�̖ڕW�̑傫�����擾.
	 * 
	 * \return �v���C����X�e�[�W�̃v���C���[�̖ڕW�̑傫��
	 */
	const float GetGoalPlayerSize() const
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	/**
	 * \brief �v���C���[�̍ŏI�̑傫����ݒ�.
	 * 
	 * \param size �v���C���[�̍ŏI�̑傫��
	 * \param number �v���C���[�̔ԍ�(0��1P)
	 */
	void SetReusltPlayerSsize(const float size, const int number = 0)
	{
		m_resultPlayerSize[number] = size;
	}
	/**
	 * \brief �v���C���[�̍ŏI�̑傫�����擾.
	 * 
	 * \param number �v���C���[�̔ԍ�(0��1P)
	 * \return �v���C���[�̍ŏI�̑傫��
	 */
	const int GetResultPlayerSize(const int number = 0) const
	{
		return  int(m_resultPlayerSize[number] * 2);
	}
	/**
	 * \brief �v���C���[�̑傫�����ڕW�̑傫�����傫�����ǂ������擾.
	 * 
	 * \param number �v���C���[�̔ԍ�(0��1P)
	 * \return �v���C���[�̍ŏI�̑傫�����X�e�[�W�̖ڕW�̑傫�����傫�����true
	 */
	const bool GetisGameClear(const int number = 0) const
	{
		return int(m_resultPlayerSize[number] * 2) >= int(m_goalPlayerSize[m_stageNumber - 1]);
	}
	/**
	 * \brief 1P�A2P�ǂ������傫�����擾.
	 * 
	 * \return 1P�̂ق����傫���Ȃ�0,���������Ȃ�-1
	 */
	const int GetWinner()
	{
		if (int(m_resultPlayerSize[0] * 2) > int(m_resultPlayerSize[1] * 2)) {
			return 0;
		}
		else if (int(m_resultPlayerSize[1] * 2) > int(m_resultPlayerSize[0] * 2)) {
			return 1;
		}
		else {
			-1;
		}
	}
	/**
	 * \brief �|�[�Y�����ǂ���.
	 * 
	 * \return �|�[�Y���Ȃ�true
	 */
	const bool GetisPose() const
	{
		return m_isPose;
	}
	/**
	 * \brief �|�[�Y����.
	 * 
	 */
	void SetPose()
	{
		m_isPose = true;
	}
	/**
	 * \brief �|�[�Y����.
	 * 
	 */
	void SetPoseCancel()
	{
		m_isPose = false;
	}
	/**
	 * \brief �ǂ̃V�[�����ݒ�.
	 * 
	 * \param scene
	 */
	void SetScene(const EnScene scene)
	{
		m_scene = scene;
	}
	/**
	 * \brief �ǂ̃V�[�������擾.
	 * 
	 * \return �V�[���̎��
	 */
	const EnScene GetScene() const
	{
		return m_scene;
	}
	/**
	 * \brief �o�g�����邩�ǂ�����ݒ�.
	 * 
	 * \param flag �o�g�����邩�ǂ����̃t���O
	 */
	void SetisBattle(const bool flag)
	{
		m_isBattle = flag;
	}
	/**
	 * \brief �o�g�����邩�ǂ������擾.
	 * 
	 * \return �o�g������ݒ肾������true
	 */
	const bool GetisBattle() const
	{
		return m_isBattle;
	}
	/**
	 * \brief �o�g�����̐������Ԃ��擾.
	 * 
	 * \return �o�g�����̐�������
	 */
	const float GetBattleLimitTime() const
	{
		return m_battleLimitTIme;
	}
	/**
	 * \brief �Q�[�����X�^�[�g�������ǂ������擾.
	 * 
	 * \return �Q�[�����X�^�[�g���Ă����true
	 */
	const bool GetisStart() const
	{
		return m_isStart;
	}
	/**
	 * \brief �Q�[�����X�^�[�g�������ǂ�����ݒ�.
	 * 
	 * \param flag �Q�[�����X�^�[�g�������ǂ���
	 */
	void SetisStart(const bool flag)
	{
		m_isStart = flag;
	}
private:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };								//�e�X�e�[�W���N���A�������ǂ���
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 6.0f,10.0f,40.0f };								//�e�X�e�[�W�̃v���C���[�̍ŏ��̑傫��
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 25.0f,35.0f,200.0f };							//�e�X�e�[�W�̃v���C���[�̃N���A�ڕW�̑傫��
	float m_resultPlayerSize[2] = { 12.0f , 12.0f};							//25.0f													//�X�e�[�W�I�����̃v���C���[�̑傫��
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 4.6f * 60.0f , 4.5f * 60.0f, 10.0f * 60.0f };		//�e�X�e�[�W�̐�������
	float m_battleLimitTIme = 3.5f * 60.0f;							//4.6f		//4.5f
 	EnStageNumber m_stageNumber = enState_FirstStage;															//�X�e�[�W�̔ԍ�
	bool m_isPose = false;																							//�|�[�Y�����ǂ���
	EnScene m_scene = enScene_Title;																				//�V�[��
	bool m_isBattle = true;
	bool m_isStart = true;
};

/**
 * \brief �C���X�^���X���擾.
 * 
 * \return �C���X�^���X
 */
static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
