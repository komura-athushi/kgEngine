#pragma once

#include <unordered_map>
#include <list>

#include "GameObject.h"
#include "..///util/Util.h"
#include "..//Noncopyable.h"
//GameObject���Ǘ�������
class CGameObjectManager :Noncopyable
{
private:
	//��ɂ���ď������q�ŏ�����
	CGameObjectManager(): 
		m_gameObjectPriorityMax(0)
	{
	}
	~CGameObjectManager()
	{
	}

public:
	/*!
	*@brief	�������B
	*@param[in]	gameObjectPrioMax	�Q�[���I�u�W�F�N�g�̗D��x�̍ő�l�B(255�܂�)
	*/
	void Init(int gameObjectPrioMax);
	//�Q�[���I�u�W�F�N�g�̖��O�L�[���쐬�A�����𐔎��ɕς���
	//���O�����̂Ƃ��ɂ���𗘗p���č�������_���̂ł���
	static unsigned int MakeGameObjectNameKey(const wchar_t* objname)
	{
		//�f�t�H���g�̃n�b�V���l
		static const unsigned int defaultNameKey = Util::MakeHash("defaultname");
		unsigned int hash;
		//���O��nullptr�A�����Ƃ��̓f�t�H���g�̃n�b�V���l��ݒ肷���
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
	//�폜���Ă�����
	void Delete(); 
public:
	//�C���X�^���X���擾
	static CGameObjectManager& GetInstance()
	{
		static CGameObjectManager instance;
		return instance;
	}

	/*!
	*@brief	�Q�[���I�u�W�F�N�g���Q�[���I�u�W�F�N�g�}�l�[�W���[�̃��X�g�ɒǉ�����
	*@param[in]	prio			���s�D�揇�ʁB
	*@param[in] gameobject		�Q�[���I�u�W�F�N�g
	*@param[in] objectName		�I�u�W�F�N�g��
	*/
	void AddGameObject(GameObjectPrio prio, const wchar_t* name = nullptr, IGameObject* gameobject = nullptr)
	{	//�����I�u�W�F�N�g�}�l�[�W���[�ɓo�^����Ă��Ȃ�������
		if (!gameobject->m_isRegist) {
			//at()��std::vector�̗v�f�ɃA�N�Z�X�AGO���X�g�ɓ����
			m_GogameobjectList.at(prio).push_back(gameobject);

			//�o�^�����I
			gameobject->m_isRegist = true;
			gameobject->m_priority = prio;
			gameobject->m_isStart = false;
			unsigned int hash = MakeGameObjectNameKey(name);
			gameobject->m_nameKey = hash;

		}
	}
	/*!
	 *@brief	�Q�[���I�u�W�F�N�g��new
	*@details
	* ���̊֐����g�p����new�����I�u�W�F�N�g�͕K��DeleteGameObject�����s���邱�Ƃ�delete����܂��B
	*@param[in]	prio		���s�D�揇�ʁB
	*@param[in]	objectName	�I�u�W�F�N�g���B
	*/
	template<class T,class...TArgs>
	T* NewGameObject(GameObjectPrio prio, const wchar_t* name = nullptr)
	{
		T* newobject = new T();
		//NewGO���ꂽ�I
		newobject->SetMarkNewFromGameObjectManager();
		/*m_GogameobjectList.at(prio).push_back(newobject);
		newobject->m_isRegist = true;
		newobject->m_priority = prio;*/
		unsigned int hash = MakeGameObjectNameKey(name);
		newobject->m_nameKey = hash;
		return newobject;
	}
	//�Q�[���I�u�W�F�N�g��FindGO�A�w�肳�ꂽ�Q�[���I�u�W�F�N�g����������
	template<class T>
	T* FindGameObject(const wchar_t* objectName = nullptr) {
		unsigned int nameKey = MakeGameObjectNameKey(objectName);
		for (auto goList : m_GogameobjectList) {
			for (auto go : goList) {
				if (!go->m_isDead && go->m_isRegist) {
					//���O�������Ȃ�
					if (go->m_nameKey == nameKey) {
						//dynamic_cast�Ō^�ϊ�(��{�N���X��h���N���X�̌^�ɕϊ�����)
						//��Ƀ|�C���^��Ԃ�
						T* p = dynamic_cast<T*>(go);
						if (p != nullptr) {
							return p;
						}
					}
				}
			}
		}
		//������Ȃ�������nullptr��Ԃ�
		return nullptr;
	}
	/*!
	*@brief	�Q�[���I�u�W�F�N�g�̌����B
	*@details
	* �d����I
	*@param[in]	objectName		�I�u�W�F�N�g���B
	*/
	template<class T>
	void FindGameObjects(const wchar_t* objectName, std::function<bool(T* go)> func)
	{
		unsigned int nameKey = MakeGameObjectNameKey(objectName);
		for (auto goList : m_GogameobjectList) {
			for (auto go : goList) {
				//���O�������Ȃ�
				if (go->m_nameKey == nameKey) {
					//�^�ϊ�
					T* p = dynamic_cast<T*>(go);
					if (p != nullptr) {
						//�����ɐݒ肵�Ă���֐����s����false�Ȃ�N�G�����f
						if (func(p) == false) {
							//�N�G�����f
							return;
						}
					}
				}
			}
		}
	}
	//�Q�[���I�u�W�F�N�g�����X�g����폜����
	void DeleteList(IGameObject* GO) {
		GameObjectPrio prio = GO->GetPriority();
		GameObjectList& goExecList = m_GogameobjectList.at(prio);
		//�Q�[���I�u�W�F�N�g���X�g����Y���̃I�u�W�F�N�g�̉ӏ���T����
		auto it = std::find(goExecList.begin(), goExecList.end(), GO);
		//�Q�[���I�u�W�F�N�g�}�l�[�W���[��New����Ă�����delete����
		if (GO->IsNewFromgameObjectManager()) {
			//�폜
			delete (*it);
		}
		//�Q�[���I�u�W�F�N�g���X�g����폜
		goExecList.erase(it);
	}
	//�Q�[���I�u�W�F�N�g�̖������t���O�𗧂Ă�ANewGO�ō쐬�����C���X�^���X�͂���ō폜����悤��
	//�����ōs���͎̂��S���肾���A�Q�[���I�u�W�F�N�g�̃A�b�v�f�[�g���I�������ɂ܂Ƃ߂č폜����܂�
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
	typedef std::list<IGameObject*> GameObjectList;						//�Q�[���I�u�W�F�N�g���X�g
	std::vector<GameObjectList> m_GogameobjectList;						//�����̗D��x���ƂɃQ�[���I�u�W�F�N�g���X�g���i�[����Ă���A�X�^�[�g���A�v�f�����s�����X�g
	std::vector<IGameObject*> m_DeletegameobjectList;					//�f���[�g���s�����X�g
	GameObjectPrio				m_gameObjectPriorityMax;				//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�ANewGO�̈����̐����̍ő吔���ĂƂ���ł���
};


static inline CGameObjectManager& GameObjectManager()
{
	return CGameObjectManager::GetInstance();
}

/*!
*@brief	�Q�[���I�u�W�F�N�g���Q�[���I�u�W�F�N�g�}�l�[�W���[�ɒǉ����邽�߂̊֐�
*@param[in]	priority	�v���C�I���e�B�A�����̗D��x
*@param[in]	objectName	�I�u�W�F�N�g���B(NULL�̎w��j
*@details
*/
template<class T>
static inline void AddGO(int priority = 0, const wchar_t* objectName = nullptr, IGameObject* go = nullptr)
{
	GameObjectManager().AddGameObject((GameObjectPrio)priority, objectName, go);
}

/*!
*@brief	�Q�[���I�u�W�F�N�g�����̃w���p�[�֐��B
*@param[in]	priority	�v���C�I���e�B�A�����̗D��x
*@param[in]	objectName	�I�u�W�F�N�g���B(NULL�̎w��j
*@details
*/
template<class T>
static inline T* NewGO(int priority, const wchar_t* objectName = nullptr , typename T::IGameObjectIsBase* = nullptr)
{
	return GameObjectManager().NewGameObject<T>((GameObjectPrio)priority, objectName);
}
/*!
*@brief	�Q�[���I�u�W�F�N�g�̌����̃w���p�[�֐��B
*@details
* ���O�̌��������邽�ߒx���ł��B
*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�B
*@return ���������C���X�^���X�̃A�h���X�B������Ȃ������ꍇ��nullptr��Ԃ��B
*/
template<class T>
static inline T* FindGO(const wchar_t* objectName = nullptr)
{
	return GameObjectManager().FindGameObject<T>(objectName);
}

/*!
	*@brief	�Q�[���I�u�W�F�N�g�̌����̃w���p�[�֐��B
	*@details
	* �����̃Q�[���I�u�W�F�N�g�ɑS�Ăɑ΂��āA�N�G�����s�������ꍇ�Ɏg�p���Ă��������B
	*@param[in]	objectName	�Q�[���I�u�W�F�N�g�̖��O�A���O����Ɏw�肵�Ȃ��ꍇ��nullptr���w�肵�Ă�������
	*@param[in]	func		�Q�[���I�u�W�F�N�g�����������Ƃ��ɌĂ΂��R�[���o�b�N�֐��B
	*/
template<class T>
static inline void QueryGOs(const wchar_t* objectName, std::function<bool(T* go)> func)
{
	return GameObjectManager().FindGameObjects<T>(objectName, func);
}

/*!
 *@brief	�Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐��B
 * NewGO���g�p���č쐬�����I�u�W�F�N�g�͕K��DeleteGO�����s����悤�ɁB
 *@param[in]	go		�폜����Q�[���I�u�W�F�N�g�B
 */
static inline void DeleteGO(IGameObject* go)
{
	GameObjectManager().DeleteGameObject(go);
}

/*!
 *@brief	�Q�[���I�u�W�F�N�g�����X�g����폜����w���p�[�֐�
 * ���X�g����폜���邾��
 *@param[in]	go		�폜����Q�[���I�u�W�F�N�g�B
 */
static inline void DeleteList(IGameObject* go)
{
	GameObjectManager().DeleteList(go);
}