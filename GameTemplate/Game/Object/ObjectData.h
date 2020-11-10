/*****************************************************************//**
 * \file   ObjectData.h
 * \brief  ObjectData�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once
#include "math/kgBox.h"

//�I�u�W�F�N�g�̃f�[�^
struct StructObjectData {
	wchar_t* s_name;						//�I�u�W�F�N�g�̖��O
	char* s_jName;							//���{��̖��O
	float s_x;								//��
	float s_y;								//����
	float s_z;								//���s
	float s_volume;							//�̐�
	int s_isSphere;							//���̂��ǂ���
	int s_isLineSegment;					//�����𐶐����邩�ǂ���
	int s_isAnimation;						//�A�j���[�V���������邩�ǂ���
	int s_isMeshCollider;					//���b�V���R���C�_�[�ɂ��邩�ǂ���
	int s_isSpec = 0;						//���ʔ��˂��邩�ǂ���
	wchar_t* s_specName;					//�������܂ꂽ�Ƃ���SE�̓f�t�H���g��
	int s_isDefalutSe = 0;						//�������܂ꂽ���Ƀf�t�H���g��SE�g�����H
	wchar_t* s_seFileName;						//SE�t�@�C���̖��O
	CVector3 s_lineVector = CVector3::Zero();	//�����̃x�N�g��
	Enxyz s_state = enState_No;					//�������ǂ̕�����

};

struct SkinModel_ObjData {
	SkinModel s_skinModel;
	bool s_isHit = false;
	int s_hashKey = 0;
	float s_x, s_y, s_z = 0.0f;
	char* s_jName;
	int s_number = 0;
};
using SkinModel_ObjDataPtr = std::unique_ptr< SkinModel_ObjData>;

/**
 * \brief �I�u�W�F�N�g�̃f�[�^���O���t�@�C������ǂݍ���.
 */
class ObjectData
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	ObjectData();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~ObjectData() {}
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static ObjectData& GetInstance()
	{
		static ObjectData instance;
		return instance;
	}
public:
	/**
	 * \brief �I�u�W�F�N�g�����擾.
	 * 
	 * \return �I�u�W�F�N�g�̐�
	 */
	int GetListSize() const
	{
		return m_objectDataList.size();
	}
	/**
	 * \brief �Y���̃I�u�W�F�N�g�̃f�[�^���擾.
	 * 
	 * \param number ���X�g�̔ԍ�
	 * \return �I�u�W�F�N�g�f�[�^
	 */
	StructObjectData* GetObjectData(const int& number)
	{
		return &m_objectDataList[number];
	}
	/**
	 * �}�ӂŕ\��������t���O��ݒ�.
	 * 
	 * \param volume �I�u�W�F�N�g�̑傫��
	 */
	void SetisHit(float volume)
	{
		m_modelObjDataList[volume]->s_isHit = true;
	}
	/**
	 * \brief �z��擾.
	 * 
	 * \return �z�� 
	 */
	const std::map<float, SkinModel_ObjDataPtr>& GetList()
	{
		return m_modelObjDataList;
	}
	/**
	 * \brief �Z�[�u����.
	 * 
	 */
	void SaveData();
private:
	/**
	 * \brief �Z�[�u�f�[�^�����[�h����.
	 * 
	 */
	void LoadSaveData();
private:
	std::vector<StructObjectData> m_objectDataList;						//�I�u�W�F�N�g�̃f�[�^
	std::map<float, SkinModel_ObjDataPtr> m_modelObjDataList;			//�}�ӂŕ\�����邽�߂̃I�u�W�F�N�g�̃f�[�^
};

/**
 * \brief �C���X�^���X���擾.
 * 
 * \return �C���X�^���X
 */
static inline ObjectData& GetObjectData()
{
	return ObjectData::GetInstance();
}


