/*****************************************************************//**
 * \file   Ranking.h
 * \brief  Ranking�N���X
 * 
 * \author komura
 * \date   November 2020
 *********************************************************************/
#pragma once

class GameData;

/**
 * \brief �v���C���[�̑傫���̃����L���O�����܂�.
 */
class Ranking
{
private:
	/**
	 * \brief �R���X�g���N�^.
	 * 
	 */
	Ranking();
	/**
	 * \brief �f�X�g���N�^.
	 * 
	 */
	~Ranking();
public:
	/**
	 * \brief �C���X�^���X���擾.
	 * 
	 * \return �C���X�^���X
	 */
	static Ranking& GetInstance()
	{
		static Ranking instance;
		return instance;
	}
	/**
	 * \brief �����L���O�f�[�^�����[�h���܂�.
	 * 
	 */
	void Load();
	/**
	 * \brief �����L���O�f�[�^���Z�[�u���܂�.
	 * 
	 */
	void Save();
	/**
	 * \brief �����L���O���\�[�g���܂�.
	 * 
	 */
	void Sort();
	/**
	 * \brief �����L���O�f�[�^�̗v�f�����擾.
	 * 
	 * \return �����L���O�f�[�^�̗v�f��
	 */
	const int GetRankingDataSize() const
	{
		return m_rankingList.size();
	}
	/**
	 * \brief �v���C���[�������L���O�ŉ��ʂȂ̂��擾.
	 * 
	 * \return �����L���O�ɓ����ĂȂ�������0
	 */
	const int GetThisRanking() const
	{
		return m_number;
	}
	/**
	 * \brief �����L���O�̒l���擾.
	 * 
	 * \param number ����
	 * \return �����L���O�̒l
	 */
	const int GetRanking(const int& number) const
	{
		return m_rankingList[number - 1];
	}
	static const int m_rankingSize = 3;
private:
	/**
	 * \brief ���l��r.
	 * 
	 * \param number ���l
	 */
	void Comparison(const int number);
private:
	std::vector<int> m_rankingList;
	GameData* m_gameData = nullptr;
	int m_number = 0;
};

