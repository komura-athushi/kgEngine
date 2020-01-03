#pragma once
#include "IMove.h"
#include "Path.h"
class MovePath:public IMove
{
public:
	MovePath();
	~MovePath() override;
	//移動後の座標を返す
	CVector3 Move() override;
	//ステートを設定、派生クラスで設定する用
	void SetMoveState() override
	{
		IMove::SetMoveStateBasic(enMove_Path);
	}
	/*!
	* @brief	初期化。
	*@param[in] pos				座標
	*@param[in]	move			移動速度
	*@param[in]	movelimit		移動範囲
	*@param[in]	rot				回転
	*/
	void Init(const CVector3& pos, const float& move, const float& movelimit, const CQuaternion& rot = CQuaternion::Identity()) override;
	//パスのデータのファイルを読み込む
	void ReadPath(const wchar_t* filePath);
	//移動ベクトルを取得
	virtual CVector3 GetMoveVector() const override
	{
		return m_movevector;
	}
private:
	CVector3 m_position = CVector3::Zero();					//座標
	float m_movespeed = 0.0f;								//移動速度
	CVector3 m_movevector = CVector3::Zero();				//移動ベクトル
	bool m_isaccessup = true;								//上側の座標に近づくかどうか
	bool m_isculcmovevector = false;						//移動ベクトルを計算したかどうか
	Path m_path;											//パス
	Point* m_point = nullptr;								//今向かってるポイント
	bool m_isstart = true;									//スタート
	bool m_isstop = false;									//ストップ？
	bool m_isadddegree = true;								//方向転換
	float m_time = 0.0f;									//方向転換の時間
	float m_timer = 0.0f;									//タイマー
};

