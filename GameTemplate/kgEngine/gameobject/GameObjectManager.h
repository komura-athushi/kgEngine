#pragma once

#include <unordered_map>


#include "GameObject.h"
#include "..///util/Util.h"
//GameObjectを管理するやつ
class CGameObjectManager :Noncopyable
{
private:
	//例によって初期化子で初期化
	CGameObjectManager(): 
		m_gameObjectPriorityMax(0)
	{
	}
	~CGameObjectManager()
	{
	}

	//ゲームオブジェクトの名前キーを作成、文字を数字に変えて
	//名前検索のときにそれを利用して高速化を狙うのですね
	static unsigned int MakeGameObjectNameKey(const wchar_t* objname)
	{
		//デフォルトのハッシュ値
		static const unsigned int defaultNameKey = Util::MakeHash("defaultname");
		unsigned int hash;
		//名前がnullptr、無いときはデフォルトのハッシュ値を設定するよ
		if (objname == nullptr) {
			hash = defaultNameKey;
		}
		else {
			hash = Util::MakeHash(objname);
		}
		return hash;
	}

public:
	//インスタンスを取得
	static CGameObjectManager& GetInstance()
	{
		static CGameObjectManager instance;
		return instance;
	}

	/*!
	*@brief	ゲームオブジェクトをゲームオブジェクトマネージャーのリストに追加する(GoリストやMapやらに)
	*@param[in]	prio			実行優先順位。
	*@param[in] gameobject		ゲームオブジェクト
	*@param[in] objectName		オブジェクト名
	*/
	void AddGameObject(GameObjectPrio prio, IGameObject* gameobject, const wchar_t* name = nullptr)
	{	//もしオブジェクトマネージャーに登録されていなかったら
		if (!gameobject->m_isRegist) {
			unsigned int hash = MakeGameObjectNameKey(name);
			//at()でstd::vectorの要素にアクセス、GOリストに入れる
			m_GogameobjectList.at(prio).push_back(gameobject);

			//登録した！
			gameobject->m_isRegist = true;
			gameobject->m_priority = prio;
			gameobject->m_isStart = false;
			gameobject->m_nameKey = hash;

		}
	}
	/*!
	 *@brief	ゲームオブジェクトのnew
	*@details
	* この関数を使用してnewしたオブジェクトは必ずDeleteGameObjectを実行することでdeleteされます。
	*@param[in]	prio		実行優先順位。
	*@param[in]	objectName	オブジェクト名。
	*/
	template<class T,class...TArgs>
	T* NewGameObject(GameObjectPrio prio, const wchar_t* name = nullptr)
	{
		T* newobject = new T();
		newobject->SetMarkNewFromGameObjectmanager();
	}
private:
	typedef std::list<IGameObject*> GameObjectList;						//ゲームオブジェクトリスト
	std::vector<GameObjectList> m_GogameobjectList;						//処理の優先度ごとにゲームオブジェクトリストが格納されている、スタートやらアプデやらを行うリスト
	std::vector<GameObjectList> m_DeletegameobjectList;					//処理の優先度ごとにゲームオブジェクトリストが格納されている、デリートを行うリスト

	GameObjectPrio				m_gameObjectPriorityMax;				//!<ゲームオブジェクトの優先度の最大数、NewGOの引数の整数の最大数ってところですか
};

