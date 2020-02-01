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
//�Ȃ񂩐F�X�ȃf�[�^���Ǘ�
class GameData
{
private:
	GameData() {}
	~GameData() {}
public:
	//�C���X�^���X���擾
	static GameData& GetInstance()
	{
		static GameData instance;
		return instance;
	}
	//�f�[�^�����[�h����
	void LoadDataStageClear();
	//�f�[�^���Z�[�u����
	void SaveDataStageClear();
	//�X�e�[�W���N���A�����I
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	//�e�X�e�[�W���N���A���Ă��邩�ǂ������擾
	bool GetisStageClear(const EnStageNumber& number) const
	{
		return m_stageClearList[number - 1];
	}
	//�X�e�[�W�̔ԍ���ݒ�
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	//�I�������X�e�[�W�̔ԍ����擾
	EnStageNumber GetStageNumber() const
	{
		return m_stageNumber;
	}
	//�I�������X�e�[�W�̃v���C���[�̏����̑傫�����擾
	float GetPlayerSize() const
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	//�v���C���[��1�X�e�[�W�ڂ̑傫�����擾
	float GetFirstPlayerSize() const
	{
		return m_firstPlayerSize[0];
	}
	//�X�e�[�W�̐������Ԃ��擾
	float GetStageLimitTime() const
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	//�v���C���[�̖ڕW�̑傫�����擾
	float GetGoalPlayerSize() const
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	//�v���C���[�̍ŏI�̑傫����ݒ�
	//0��1P
	void SetReusltPlayerSsize(const float size, const int number = 0)
	{
		m_resultPlayerSize[number] = size;
	}
    //�v���C���[�̍ŏI�̑傫�����擾
	//0��1P
	int GetResultPlayerSize(const int number = 0) const
	{
		return  int(m_resultPlayerSize[number] * 2);
	}
	//�v���C���[�̑傫�����ڕW�̑傫�����傫�����ǂ������擾
	//0��1P
	bool GetisGameClear(const int number = 0) const
	{
		return int(m_resultPlayerSize[number] * 2) >= int(m_goalPlayerSize[m_stageNumber - 1]);
	}
	//1P�A2P�ǂ������傫�����擾
	//0��1P
	//���������Ȃ�-1
	int GetWinner()
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
	//�|�[�Y�����ǂ���
	bool GetisPose() const
	{
		return m_isPose;
	}
	//�|�[�Y����
	void SetPose()
	{
		m_isPose = true;
	}
	//�|�[�Y����
	void SetPoseCancel()
	{
		m_isPose = false;
	}
	//�V�[����ݒ�
	void SetScene(const EnScene& scene)
	{
		m_scene = scene;
	}
	//�V�[�����擾
	EnScene GetScene()
	{
		return m_scene;
	}
	//�o�g�������ݒ�
	void SetisBattle(bool flag)
	{
		m_isBattle = flag;
	}
	//�o�g������H
	bool GetisBattle() const
	{
		return m_isBattle;
	}
	//�o�g���̐������Ԃ��擾
	float GetBattleLimitTime() const
	{
		return m_battleLimitTIme;
	}
	//�Q�[�����X�^�[�g�����H
	bool GetisStart() const
	{
		return m_isStart;
	}
	//�X�^�[�g�������ǂ�����ݒ�
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

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
