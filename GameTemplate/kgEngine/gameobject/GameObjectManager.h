#pragma once

#include <unordered_map>
#include <list>

#include "GameObject.h"
#include "..///util/Util.h"
#include "..//Noncopyable.h"
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

public:
	/*!
	*@brief	初期化。
	*@param[in]	gameObjectPrioMax	ゲームオブジェクトの優先度の最大値。(255まで)
	*/
	void Init(int gameObjectPrioMax);
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


	void Start();
	void Update();
	void PostRender();
	void Draw();
	void PreUpdate();
	//削除していくぅ
	void Delete(); 
public:
	//インスタンスを取得
	static CGameObjectManager& GetInstance()
	{
		static CGameObjectManager instance;
		return instance;
	}

	/*!
	*@brief	ゲームオブジェクトをゲームオブジェクトマネージャーのリストに追加する
	*@param[in]	prio			実行優先順位。
	*@param[in] gameobject		ゲームオブジェクト
	*@param[in] objectName		オブジェクト名
	*/
	void AddGameObject(GameObjectPrio prio, const wchar_t* name = nullptr, IGameObject* gameobject = nullptr)
	{	//もしオブジェクトマネージャーに登録されていなかったら
		if (!gameobject->m_isRegist) {
			//at()でstd::vectorの要素にアクセス、GOリストに入れる
			m_GogameobjectList.at(prio).push_back(gameobject);

			//登録した！
			gameobject->m_isRegist = true;
			gameobject->m_priority = prio;
			gameobject->m_isStart = false;
			unsigned int hash = MakeGameObjectNameKey(name);
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
		//NewGOされた！
		newobject->SetMarkNewFromGameObjectManager();
		/*m_GogameobjectList.at(prio).push_back(newobject);
		newobject->m_isRegist = true;
		newobject->m_priority = prio;*/
		unsigned int hash = MakeGameObjectNameKey(name);
		newobject->m_nameKey = hash;
		return newobject;
	}
	//ゲームオブジェクトのFindGO、指定されたゲームオブジェクトを検索する
	template<class T>
	T* FindGameObject(const wchar_t* objectName = nullptr) {
		unsigned int nameKey = MakeGameObjectNameKey(objectName);
		for (auto goList : m_GogameobjectList) {
			for (auto go : goList) {
				if (!go->m_isDead && go->m_isRegist) {
					//名前が同じなら
					if (go->m_nameKey == nameKey) {
						//dynamic_castで型変換(基本クラスを派生クラスの型に変換する)
						//後にポインタを返す
						T* p = dynamic_cast<T*>(go);
						if (p != nullptr) {
							return p;
						}
					}
				}
			}
		}
		//見つからなかったらnullptrを返す
		return nullptr;
	}
	/*!
	*@brief	ゲームオブジェクトの検索。
	*@details
	* 重いよ！
	*@param[in]	objectName		オブジェクト名。
	*/
	template<class T>
	void FindGameObjects(const wchar_t* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = MakeGameObjectNameKey(objectName);
		for (auto goList : m_GogameobjectList) {
			for (auto go : goList) {
				//名前が同じなら
				if (go->m_nameKey == nameKey) {
					//型変換
					T* p = dynamic_cast<T*>(go);
					if (p != nullptr) {
						//引数に設定してある関数実行してfalseならクエリ中断
						if (func(p) == false) {
							//クエリ中断
							return;
						}
					}
				}
			}
		}
	}
	//ゲームオブジェクトをリストから削除する
	void DeleteList(IGameObject* GO) {
		GameObjectPrio prio = GO->GetPriority();
		GameObjectList& goExecList = m_GogameobjectList.at(prio);
		//ゲームオブジェクトリストから該当のオブジェクトの箇所を探して
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//ゲームオブジェクトマネージャーでNewされていたらdeleteする
		if (GO->IsNewFromgameObjectManager()) {
			//削除
			delete (*it);
		}
		//ゲームオブジェクトリストから削除
		goExecList.erase(it);
	}
	//ゲームオブジェクトの無効化フラグを立てる、NewGOで作成したインスタンスはこれで削除するように
	//ここで行うのは死亡判定だけ、ゲームオブジェクトのアップデートが終わった後にまとめて削除されます
	void DeleteGameObject(IGameObject* gameobject) {
		if (gameobject == nullptr) {
			return;
		}
		if (gameobject->IsNewFromgameObjectManager()) {
			if (!gameobject->m_isDead) {
				gameobject->m_isDead = true;
				gameobject->m_isRegist = false;
				gameobject->m_isRegistDeadList = true;
				m_DeletegameobjectList.emplace_back(gameobject);
				gameobject->OnDestroy();
				return;
			}
		}
		return;
	}
private:
	typedef std::list<IGameObject*> GameObjectList;						//ゲームオブジェクトリスト
	std::vector<GameObjectList> m_GogameobjectList;						//処理の優先度ごとにゲームオブジェクトリストが格納されている、スタートやらアプデやらを行うリスト
	std::vector<IGameObject*> m_DeletegameobjectList;					//デリートを行うリスト
	GameObjectPrio				m_gameObjectPriorityMax;				//!<ゲームオブジェクトの優先度の最大数、NewGOの引数の整数の最大数ってところですか
};


static inline CGameObjectManager& GameObjectManager()
{
	return CGameObjectManager::GetInstance();
}

/*!
*@brief	ゲームオブジェクトをゲームオブジェクトマネージャーに追加するための関数
*@param[in]	priority	プライオリティ、処理の優先度
*@param[in]	objectName	オブジェクト名。(NULLの指定可）
*@details
*/
template<class T>
static inline void AddGO(int priority = 0, const wchar_t* objectName = nullptr, IGameObject* go = nullptr)
{
	GameObjectManager().AddGameObject((GameObjectPrio)priority, objectName, go);
}

/*!
*@brief	ゲームオブジェクト生成のヘルパー関数。
*@param[in]	priority	プライオリティ、処理の優先度
*@param[in]	objectName	オブジェクト名。(NULLの指定可）
*@details
*/
template<class T>
static inline T* NewGO(int priority, const wchar_t* objectName = nullptr , typename T::IGameObjectIsBase* = nullptr)
{
	return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName);
}
/*!
*@brief	ゲームオブジェクトの検索のヘルパー関数。
*@details
* 名前の検索が入るため遅いです。
*@param[in]	objectName	ゲームオブジェクトの名前。
*@return 見つかったインスタンスのアドレス。見つからなかった場合はnullptrを返す。
*/
template<class T>
static inline T* FindGO(const wchar_t* objectName = nullptr)
{
	return GameObjectManager().FindGameObject<T>(objectName);
}

/*!
	*@brief	ゲームオブジェクトの検索のヘルパー関数。
	*@details
	* 同名のゲームオブジェクトに全てに対して、クエリを行いたい場合に使用してください。
	*@param[in]	objectName	ゲームオブジェクトの名前、名前を特に指定しない場合はnullptrを指定してください
	*@param[in]	func		ゲームオブジェクトが見つかったときに呼ばれるコールバック関数。
	*/
template<class T>
static inline void QueryGOs(const wchar_t* objectName, std::function<bool(T* go)> func)
{
	return GameObjectManager().FindGameObjects<T>(objectName, func);
}

/*!
 *@brief	ゲームオブジェクト削除のヘルパー関数。
 * NewGOを使用して作成したオブジェクトは必ずDeleteGOを実行するように。
 *@param[in]	go		削除するゲームオブジェクト。
 */
static inline void DeleteGO(IGameObject* go)
{
	GameObjectManager().DeleteGameObject(go);
}

/*!
 *@brief	ゲームオブジェクトをリストから削除するヘルパー関数
 * リストから削除するだけ
 *@param[in]	go		削除するゲームオブジェクト。
 */
static inline void DeleteList(IGameObject* go)
{
	GameObjectManager().DeleteList(go);
}