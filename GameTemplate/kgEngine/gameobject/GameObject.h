#pragma once
#include "..//Noncopyable.h"

//typedef 既存の型を新しい型にするっていう、名前が変わるだけかな
typedef unsigned char GameObjectPrio;
//GameObject Update関数とか使いたかったらこれを継承
class IGameObject :Noncopyable
{
public:
	using IGameObjectIsBase = IGameObject;
	//コンストラクタ、初期化子で初期化するべ
	IGameObject();
	//デストラクタ
	virtual ~IGameObject();

public:
	//Update直前に呼ばれる開始処理
	virtual bool Start() { return true; }

	//Update
	virtual void  Update() {}

	//PreUpdate
	virtual void PreUpdate() {}
	//実行優先度を取得
	GameObjectPrio GetPriority() const
	{
		return m_priority;
	}

	//描画
	virtual void Render() {}

	//3Dモデル描画、今だけ
	virtual void Draw() {}

	//削除の直前に呼ばれる関数
	virtual void OnDestroy() {}
public :
	//ポストエフェクトの後で実行するやつ、ポストエフェクトの影響を受けたくない場合はこっち
	virtual void PostRender() {}

	//死亡フラグをたてる
	void SetDead()
	{
		m_isDead = true;
	}

	//死亡判定
	bool isDead() const
	{
		return m_isDead;
	}

	//スタート関数が完了したかどうか
	virtual bool isStart() const
	{
		return m_isStart;
	}

	//アクティブかどうかを判定
	bool isActive() const
	{
		return m_isActive;
	}

	//アクティブフラグを設定
	void SetActive(bool flag)
	{
		m_isActive = flag;
	}

public:
	//条件を満たせばPostRender関数を実行する
	void PostRenderWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PostRender();
		}
	}

	//条件を満たせばUpdate関数を実行する
	void UpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Update();
		}
	}

	void DrawWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			Draw();
		}
	}

	void PreUpdateWrapper()
	{
		if (m_isActive && m_isStart && !m_isDead && !m_isRegistDeadList) {
			PreUpdate();
		}
	}

	//条件を満たせば一度だけStart関数を実行する
	void StartWrapper()
	{
		if (m_isActive && !m_isStart && !m_isDead && !m_isRegistDeadList) {
			if (Start()) {
				m_isStart = true;
			}
		}
	}

	//GameObjectManagerでNewGOされたのを設定する
	void SetMarkNewFromGameObjectManager()
	{
		m_isNewFromGameObjectManager = true;
	}

	//GameObjectManagerでNewGOされたかどうかを取得する
	bool IsNewFromgameObjectManager() const
	{
		return m_isNewFromGameObjectManager;
	}
	//friend宣言、フレンドクラスは指定されたクラスのprivateやprotectedメンバにアクセスすることができる
	friend class CGameObjectManager;
protected:
	GameObjectPrio m_priority;				//実行優先度
	bool m_isDead = false;					//死亡判定
	bool m_isRegistDeadList = false;		//死亡リストに登録されているかどうか
	bool m_isStart = false;					//スタート関数が完了したかどうか
	bool m_isNewFromGameObjectManager;		//ゲームオブジェクトマネージャーでNewGOされたかどうか
	bool m_isRegist = false;				//GameObjectManagerに登録されているかどうか
	bool m_isActive = true;					//アクティブかどうか(Update)がよばれるかどうかってことですかね
	unsigned int m_nameKey = 0;				//名前キー、名前の長さかね？
};

