/*****************************************************************//**
 * \file   GameData.h
 * \brief  GameDataクラス
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
 * \brief ゲーム中の色々なデータを管理.
 */
class GameData
{
private:
	/**
	 * \brief コンストラクタ.
	 * 
	 */
	GameData() {}
	/**
	 * \brief デストラクタ.
	 * 
	 */
	~GameData() {}
public:
	/**
	 * \brief インスタンスを取得.
	 * 
	 * \return インスタンス
	 */
	static GameData& GetInstance()
	{
		static GameData instance;
		return instance;
	}
	/**
	 * \brief ステージのクリア状況をロードする.
	 * 
	 */
	void LoadDataStageClear();
	/**
	 * \brief ステージのクリア状況をセーブする.
	 * 
	 */
	void SaveDataStageClear();
	/**
	 * \brief ステージをクリアした.
	 * 
	 */
	void SetStageClear()
	{
		m_stageClearList[m_stageNumber - 1] = true;
	}
	/**
	 * \brief 該当のステージをクリアしてるかどうか.
	 * 
	 * \param number ステージの番号
	 * \return ステージクリアしていたらtrue
	 */
	bool GetisStageClear(const EnStageNumber& number) const
	{
		return m_stageClearList[number - 1];
	}
	/**
	 * \brief プレイするステージの番号を設定.
	 * 
	 * \param number プレイするステージの番号
	 */
	void SetStageNumber(const EnStageNumber& number)
	{
		m_stageNumber = number;
	}
	/**
	 * \brief プレイしているステージの番号を取得.
	 * 
	 * \return プレイしているステージの番号
	 */
	const EnStageNumber GetStageNumber() const
	{
		return m_stageNumber;
	}
	/**
	 * \brief プレイするステージのプレイヤーの最初の大きさを取得.
	 * 
	 * \return プレイするステージのプレイヤーの最初の大きさを取得
	 */
	const float GetPlayerSize() const
	{
		return m_firstPlayerSize[m_stageNumber - 1];
	}
	/**
	 * \brief ステージ1のプレイヤーの最初の大きさを取得.
	 * 
	 * \return ステージ1のプレイヤーの最初の大きさ
	 */
	const float GetFirstPlayerSize() const
	{
		return m_firstPlayerSize[0];
	}
	/**
	 * \brief プレイするステージの制限時間を取得.
	 * 
	 * \return プレイするステージの制限時間
	 */
	const float GetStageLimitTime() const
	{
		return m_stageLimitTime[m_stageNumber - 1];
	}
	/**
	 * \brief プレイするステージのプレイヤーの目標の大きさを取得.
	 * 
	 * \return プレイするステージのプレイヤーの目標の大きさ
	 */
	const float GetGoalPlayerSize() const
	{
		return m_goalPlayerSize[m_stageNumber - 1];
	}
	/**
	 * \brief プレイヤーの最終の大きさを設定.
	 * 
	 * \param size プレイヤーの最終の大きさ
	 * \param number プレイヤーの番号(0で1P)
	 */
	void SetReusltPlayerSsize(const float size, const int number = 0)
	{
		m_resultPlayerSize[number] = size;
	}
	/**
	 * \brief プレイヤーの最終の大きさを取得.
	 * 
	 * \param number プレイヤーの番号(0で1P)
	 * \return プレイヤーの最終の大きさ
	 */
	const int GetResultPlayerSize(const int number = 0) const
	{
		return  int(m_resultPlayerSize[number] * 2);
	}
	/**
	 * \brief プレイヤーの大きさが目標の大きさより大きいかどうかを取得.
	 * 
	 * \param number プレイヤーの番号(0で1P)
	 * \return プレイヤーの最終の大きさがステージの目標の大きさより大きければtrue
	 */
	const bool GetisGameClear(const int number = 0) const
	{
		return int(m_resultPlayerSize[number] * 2) >= int(m_goalPlayerSize[m_stageNumber - 1]);
	}
	/**
	 * \brief 1P、2Pどっちが大きいか取得.
	 * 
	 * \return 1Pのほうが大きいなら0,引き分けなら-1
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
	 * \brief ポーズ中かどうか.
	 * 
	 * \return ポーズ中ならtrue
	 */
	const bool GetisPose() const
	{
		return m_isPose;
	}
	/**
	 * \brief ポーズする.
	 * 
	 */
	void SetPose()
	{
		m_isPose = true;
	}
	/**
	 * \brief ポーズ解除.
	 * 
	 */
	void SetPoseCancel()
	{
		m_isPose = false;
	}
	/**
	 * \brief どのシーンか設定.
	 * 
	 * \param scene
	 */
	void SetScene(const EnScene scene)
	{
		m_scene = scene;
	}
	/**
	 * \brief どのシーン中か取得.
	 * 
	 * \return シーンの種類
	 */
	const EnScene GetScene() const
	{
		return m_scene;
	}
	/**
	 * \brief バトルするかどうかを設定.
	 * 
	 * \param flag バトルするかどうかのフラグ
	 */
	void SetisBattle(const bool flag)
	{
		m_isBattle = flag;
	}
	/**
	 * \brief バトルするかどうかを取得.
	 * 
	 * \return バトルする設定だったらtrue
	 */
	const bool GetisBattle() const
	{
		return m_isBattle;
	}
	/**
	 * \brief バトル中の制限時間を取得.
	 * 
	 * \return バトル中の制限時間
	 */
	const float GetBattleLimitTime() const
	{
		return m_battleLimitTIme;
	}
	/**
	 * \brief ゲームがスタートしたかどうかを取得.
	 * 
	 * \return ゲームがスタートしていればtrue
	 */
	const bool GetisStart() const
	{
		return m_isStart;
	}
	/**
	 * \brief ゲームがスタートしたかどうかを設定.
	 * 
	 * \param flag ゲームがスタートしたかどうか
	 */
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

/**
 * \brief インスタンスを取得.
 * 
 * \return インスタンス
 */
static inline  GameData& GetGameData()
{
	return GameData::GetInstance();
}
