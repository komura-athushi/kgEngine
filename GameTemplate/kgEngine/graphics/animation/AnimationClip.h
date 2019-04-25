/*!
 *@brief	�A�j���[�V�����N���b�v�B
 */

#pragma once

//////////////////////////////////////////////////////////////////////
// �e��\����
//////////////////////////////////////////////////////////////////////
/*!
*@brief	�A�j���[�V�����N���b�v�̃w�b�_�[�B
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<�L�[�t���[���̐��B
	std::uint32_t numAnimationEvent;	//!<�A�j���[�V�����C�x���g�̐��B
};
/*!
*@brief	�A�j���[�V�����C�x���g
*@�A�j���[�V�����C�x���g�͖��Ή��B
* ��肽�������玩���Ŏ�������悤�ɁB
*/
struct AnimationEventData {
	float	invokeTime;					//!<�A�j���[�V�����C�x���g���������鎞��(�P��:�b)
	std::uint32_t eventNameLength;		//!<�C�x���g���̒����B
};
/*!
*@brief	�L�[�t���[���B
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<�{�[���C���f�b�N�X�B
	float time;					//!<���ԁB
	CMatrix transform;			//!<�g�����X�t�H�[���B
};
/*!
*@brief	�L�[�t���[���B
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<�{�[���C���f�b�N�X�B
	float time;					//!<���ԁB
	CVector3 transform[4];		//!<�g�����X�t�H�[���B
};

/*!
*@brief	�A�j���[�V�����N���b�v�B
*/
class AnimationClip  {
public:
	//�^�C�v�ʂ��������炵���āA�������̂�std::vector<KeyFrame*>�̕ʖ���`�B
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	�R���X�g���N�^
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	�f�X�g���N�^�B
	*/
	~AnimationClip();
	/*!
	*@brief	�A�j���[�V�����N���b�v�����[�h�B
	*@param[in]	filePath	�t�@�C���p�X�B
	*/
	void Load(const wchar_t* filePath);

	/*!
	*@brief	���[�v����H
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	���[�v�t���O��ݒ肷��B
	*/
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/*!
	*@brief
	*/
	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}
private:
	
	bool m_isLoop = false;									//!<���[�v�t���O�B
	std::vector<Keyframe*> m_keyframes;						//�S�ẴL�[�t���[���B
	std::vector<keyFramePtrList> m_keyFramePtrListArray;	//�{�[�����Ƃ̃L�[�t���[���̃��X�g���Ǘ����邽�߂̔z��B
															//�Ⴆ�΁Am_keyFramePtrListArray[0]��0�Ԗڂ̃{�[���̃L�[�t���[���̃��X�g�A
															//m_keyFramePtrListArray[1]��1�Ԗڂ̃{�[���̃L�[�t���[���̃��X�g�Ƃ����������B
	keyFramePtrList* m_topBoneKeyFramList = nullptr;
};

