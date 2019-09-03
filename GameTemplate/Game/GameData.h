#pragma once
static int NUMBEROFSTAGES = 3;
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
	//�X�e�[�W���N���A�����I
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	//�X�e�[�W���N���A���Ă��邩�ǂ������擾
	bool GetisStageClear(const EnStageNumber& number)
	{
		return m_stageClearList[number - 1];
	}
	//�X�e�[�W�̔ԍ���ݒ�
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	//�I�������X�e�[�W�̔ԍ����擾
	EnStageNumber GetStageNumber()
	{
		return m_stageNumber;
	}
	//�I�������X�e�[�W�̃v���C���[�̏����̑傫�����擾
	float GetPlayerSize()
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	//�X�e�[�W�̐������Ԃ��擾
	float GetStageLimitTime()
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	//�v���C���[�̖ڕW�̑傫�����擾
	float GetGoalPlayerSize()
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	//�v���C���[�̍ŏI�̑傫����ݒ�
	void SetReusltPlayerSsize(const int& size)
	{
		m_resultPlayerSize = size;
	}
    //�v���C���[�̍ŏI�̑傫�����擾
	float GetResultPlayerSize()
	{
		return m_resultPlayerSize;
	}
	//�|�[�Y�����ǂ���
	bool GetisPose()
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
public:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };								//�e�X�e�[�W���N���A�������ǂ���
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 20.0f,40.0f,40.0f };								//�e�X�e�[�W�̃v���C���[�̍ŏ��̑傫��
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 100.0f,200.0f,300.0f };							//�e�X�e�[�W�̃v���C���[�̃N���A�ڕW�̑傫��
	float m_resultPlayerSize = 0.0f;																				//�X�e�[�W�I�����̃v���C���[�̑傫��
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 0.1f * 60.0f , 5.0f * 60.0f, 7.0f * 60.0f };		//�e�X�e�[�W�̐�������
 	EnStageNumber m_stageNumber = enState_FirstStage;																//�X�e�[�W�̔ԍ�
	bool m_isPose = false;																							//�|�[�Y�����ǂ���
	EnScene m_scene = enScene_Title;
};

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}