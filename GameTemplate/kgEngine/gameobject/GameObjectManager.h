#pragma once

#include <unordered_map>


#include "GameObject.h"
#include "..///util/Util.h"
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

public:
	//�C���X�^���X���擾
	static CGameObjectManager& GetInstance()
	{
		static CGameObjectManager instance;
		return instance;
	}

	/*!
	*@brief	�Q�[���I�u�W�F�N�g���Q�[���I�u�W�F�N�g�}�l�[�W���[�̃��X�g�ɒǉ�����(Go���X�g��Map����)
	*@param[in]	prio			���s�D�揇�ʁB
	*@param[in] gameobject		�Q�[���I�u�W�F�N�g
	*@param[in] objectName		�I�u�W�F�N�g��
	*/
	void AddGameObject(GameObjectPrio prio, IGameObject* gameobject, const wchar_t* name = nullptr)
	{	//�����I�u�W�F�N�g�}�l�[�W���[�ɓo�^����Ă��Ȃ�������
		if (!gameobject->m_isRegist) {
			unsigned int hash = MakeGameObjectNameKey(name);
			//at()��std::vector�̗v�f�ɃA�N�Z�X�AGO���X�g�ɓ����
			m_GogameobjectList.at(prio).push_back(gameobject);

			//�o�^�����I
			gameobject->m_isRegist = true;
			gameobject->m_priority = prio;
			gameobject->m_isStart = false;
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
		newobject->SetMarkNewFromGameObjectmanager();
	}
private:
	typedef std::list<IGameObject*> GameObjectList;						//�Q�[���I�u�W�F�N�g���X�g
	std::vector<GameObjectList> m_GogameobjectList;						//�����̗D��x���ƂɃQ�[���I�u�W�F�N�g���X�g���i�[����Ă���A�X�^�[�g���A�v�f�����s�����X�g
	std::vector<GameObjectList> m_DeletegameobjectList;					//�����̗D��x���ƂɃQ�[���I�u�W�F�N�g���X�g���i�[����Ă���A�f���[�g���s�����X�g

	GameObjectPrio				m_gameObjectPriorityMax;				//!<�Q�[���I�u�W�F�N�g�̗D��x�̍ő吔�ANewGO�̈����̐����̍ő吔���ĂƂ���ł���
};

