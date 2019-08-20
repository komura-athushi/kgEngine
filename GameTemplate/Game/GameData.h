#pragma once
static int NUMBEROFSTAGES = 3;
enum EnStageNumber {
	enState_Stage1 = 1,
	enState_Stage2 = 2,
	enState_Stage3 = 3,
	enState_FirstStage = enState_Stage1,
	enState_LastStage = enState_Stage3
};
class GameData
{
private:
	GameData() {}
	~GameData() {}
public:
	static GameData& GetInstance()
	{
		static GameData instance;
		return instance;
	}
	//ステージをクリアした！
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	//ステージをクリアしているかどうかを取得
	bool GetisStageClear(const EnStageNumber& number)
	{
		return m_stageClearList[number - 1];
	}
	//ステージの番号を設定
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	//選択したステージの番号を取得
	EnStageNumber GetStageNumber()
	{
		return m_stageNumber;
	}
	//選択したステージのプレイヤーの初期の大きさを取得
	float GetPlayerSize()
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	//ステージの制限時間を取得
	float GetStageLimitTime()
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	//プレイヤーの目標の大きさを取得
	float GetGoalPlayerSize()
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	//プレイヤーの最終の大きさを設定
	void SetReusltPlayerSsize(const int& size)
	{
		m_resultPlayerSize = size;
	}
    //プレイヤーの最終の大きさを取得
	float GetResultPlayerSize()
	{
		return m_resultPlayerSize;
	}
public:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 20.0f,40.0f,40.0f };
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 100.0f,200.0f,300.0f };
	float m_resultPlayerSize = 0.0f;
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 3.0f * 60.0f , 5.0f * 60.0f, 7.0f * 60.0f };
 	EnStageNumber m_stageNumber = enState_FirstStage;
};

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
