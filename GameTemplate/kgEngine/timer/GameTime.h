#pragma once

#include <queue>
#include <list>
#include <minwindef.h>
#include "graphics/2D/CFont.h"
//�Q�[�����ԁA1�t���[�����Ƃ̌o�ߎ���(�b)�𑪂�����Ƃ�
class CGameTime {
private:
	CGameTime(){}
	~CGameTime(){}
public:
	//�C���X�^���X���擾
	static CGameTime& GetInstance()
	{
		static CGameTime instance;
		return instance;
	}
	//1�t���[�����Ƃ̌o�ߎ��Ԃ��擾(�b)
	const float GetFrameDeltaTime() const
	{
		return m_frameDeltaTime;
	}
	//1�t���[�����Ƃ̌o�ߎ���(�b)���v�Z
	void PushFrameDeltaTime(float deltaTime)
	{
		//���Ԃ��L������
		m_frameDeltaTimeQue.push_back(deltaTime);
		//�z��̒�����30.0f�ȏ�Ȃ玞�Ԃ��o�߂���
		if (m_frameDeltaTimeQue.size() > 30.0f) {
			float totalTime = 0.0f;
			//�z��̎��Ԃ̍��v���v�Z����
			for (auto time : m_frameDeltaTimeQue) {
				totalTime += time;
			}
			//���ϒl���Ƃ�A�z��̍��v���Ԃ�z��̒����Ŋ���
			m_frameDeltaTime = totalTime / m_frameDeltaTimeQue.size();
			//�z��̈�ԍŏ��̗v�f���폜����
			m_frameDeltaTimeQue.pop_front();

		}
	}
	void Draw()
	{
#if BUILD_LEVEL != BUILD_LEVEL_MASTER
		wchar_t output[256];
		swprintf_s(output, L"FPS %f", 1.0f / m_frameDeltaTime);
		m_font.DrawScreenPos(output, CVector2::Zero());
#endif
	}
private:
	std::list<float> m_frameDeltaTimeQue;
	float m_frameDeltaTime = 1.0f / 60.0f;			//1�t���[�����Ƃ̌o�ߎ��ԁA�f�t�H���g�ł�60�t���[����
	CFont m_font;
};

static inline CGameTime& GameTime()
{
	return CGameTime::GetInstance();
}