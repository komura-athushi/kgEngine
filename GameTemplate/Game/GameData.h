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
	//インスタンスを取得
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
	//ポーズ中かどうか
	bool GetisPose()
	{
		return m_isPose;
	}
	//ポーズする
	void SetPose()
	{
		m_isPose = true;
	}
	//ポーズ解除
	void SetPoseCancel()
	{
		m_isPose = false;
	}
	//シーンを設定
	void SetScene(const EnScene& scene)
	{
		m_scene = scene;
	}
	//シーンを取得
	EnScene GetScene()
	{
		return m_scene;
	}
public:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };								//各ステージをクリアしたかどうか
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 20.0f,40.0f,40.0f };								//各ステージのプレイヤーの最初の大きさ
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 100.0f,200.0f,300.0f };							//各ステージのプレイヤーのクリア目標の大きさ
	float m_resultPlayerSize = 0.0f;																				//ステージ終了時のプレイヤーの大きさ
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 0.1f * 60.0f , 5.0f * 60.0f, 7.0f * 60.0f };		//各ステージの制限時間
 	EnStageNumber m_stageNumber = enState_FirstStage;																//ステージの番号
	bool m_isPose = false;																							//ポーズ中かどうか
	EnScene m_scene = enScene_Title;
};

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
