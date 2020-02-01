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
	//0で1P
	void SetReusltPlayerSsize(const float size, const int number = 0)
	{
		m_resultPlayerSize[number] = size;
	}
    //プレイヤーの最終の大きさを取得
	//0で1P
	int GetResultPlayerSize(const int number = 0) const
	{
		return  int(m_resultPlayerSize[number] * 2);
	}
	//プレイヤーの大きさが目標の大きさより大きいかどうかを取得
	//0で1P
	bool GetisGameClear(const int number = 0) const
	{
		return int(m_resultPlayerSize[number] * 2) >= int(m_goalPlayerSize[m_stageNumber - 1]);
	}
	//1P、2Pどっちが大きいか取得
	//0が1P
	//引き分けなら-1
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
	//バトルするを設定
	void SetisBattle(bool flag)
	{
		m_isBattle = flag;
	}
	//バトルする？
	bool GetisBattle() const
	{
		return m_isBattle;
	}
	//バトルの制限時間を取得
	float GetBattleLimitTime() const
	{
		return m_battleLimitTIme;
	}
	//ゲームがスタートした？
	bool GetisStart() const
	{
		return m_isStart;
	}
	//スタートしたかどうかを設定
	void SetisStart(const bool flag)
	{
		m_isStart = flag;
	}
private:
	bool m_stageClearList[EnStageNumber::enState_LastStage] = { false,false,false };								//各ステージをクリアしたかどうか
	float m_firstPlayerSize[EnStageNumber::enState_LastStage] = { 6.0f,10.0f,40.0f };								//各ステージのプレイヤーの最初の大きさ
	float m_goalPlayerSize[EnStageNumber::enState_LastStage] = { 25.0f,35.0f,200.0f };							//各ステージのプレイヤーのクリア目標の大きさ
	float m_resultPlayerSize[2] = { 12.0f , 12.0f};							//25.0f													//ステージ終了時のプレイヤーの大きさ
	float m_stageLimitTime[EnStageNumber::enState_LastStage] = { 4.6f * 60.0f , 4.5f * 60.0f, 10.0f * 60.0f };		//各ステージの制限時間
	float m_battleLimitTIme = 3.5f * 60.0f;							//4.6f		//4.5f
 	EnStageNumber m_stageNumber = enState_FirstStage;															//ステージの番号
	bool m_isPose = false;																							//ポーズ中かどうか
	EnScene m_scene = enScene_Title;																				//シーン
	bool m_isBattle = true;
	bool m_isStart = true;
};

static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
