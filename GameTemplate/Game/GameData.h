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
//なんか色々なデータを管理
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
	//データをロードする
	void LoadDataStageClear();
	//データをセーブする
	void SaveDataStageClear();
	//ステージをクリアした！
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	//各ステージをクリアしているかどうかを取得
	bool GetisStageClear(const EnStageNumber& number) const
	{
		return m_stageClearList[number - 1];
	}
	//ステージの番号を設定
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	//選択したステージの番号を取得
	EnStageNumber GetStageNumber() const
	{
		return m_stageNumber;
	}
	//選択したステージのプレイヤーの初期の大きさを取得
	float GetPlayerSize() const
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	//プレイヤーの1ステージ目の大きさを取得
	float GetFirstPlayerSize() const
	{
		return m_firstPlayerSize[0];
	}
	//ステージの制限時間を取得
	float GetStageLimitTime() const
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	//プレイヤーの目標の大きさを取得
	float GetGoalPlayerSize() const
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	//プレイヤーの最終の大きさを設定
	void SetReusltPlayerSsize(const float size)
	{
		m_resultPlayerSize = size;
	}
    //プレイヤーの最終の大きさを取得
	int GetResultPlayerSize() const
	{
		return  int(m_resultPlayerSize * 2);
	}
	//プレイヤーの大きさが目標の大きさより大きいかどうかを取得
	bool GetisGameClear() const
	{
		return int(m_resultPlayerSize * 2) >= int(m_goalPlayerSize[m_stageNumber - 1]);
	}
	//ポーズ中かどうか
	bool GetisPose() const
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
private:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };								//各ステージをクリアしたかどうか
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 6.0f,10.0f,40.0f };								//各ステージのプレイヤーの最初の大きさ
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 25.0f,40.0f,200.0f };							//各ステージのプレイヤーのクリア目標の大きさ
	float m_resultPlayerSize = 12.0f;							//25.0f													//ステージ終了時のプレイヤーの大きさ
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 4.6f * 60.0f , 5.0f * 60.0f, 10.0f * 60.0f };		//各ステージの制限時間
 	EnStageNumber m_stageNumber = enState_FirstStage;			//4.6f												//ステージの番号
	bool m_isPose = false;																							//ポーズ中かどうか
	EnScene m_scene = enScene_Title;																				//シーン
};

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
