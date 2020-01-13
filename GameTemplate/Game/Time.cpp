#include "stdafx.h"
#include "Time.h"
#include "Game.h"
#include "GameData.h"

Time::Time()
{

}

Time::~Time()
{

}

bool Time::Start()
{
	//��
	m_sprite1.Init(L"Resource/sprite/clockkari.dds", false);
	//���F
	m_sprite2.Init(L"Resource/sprite/clockkari2.dds", true);
	//�������Ԃ���~�`�Q�[�W�̊p�x�����肷��
	m_degree = (m_time / 60.0f) * 360.0f / 60.0f;
	m_game = FindGO<Game>();
	m_gamedata = &GetGameData();
	return true;
}

void Time::PostRender()
{

	//�^�C�}�[��0�ɂȂ�΃X�e�[�W�I���t���O�����Ă�
	if (m_timer <= 0.0000001f) {
		m_timer = 0.0f;
		m_game->SetOwaOwari();
	}
	else {
		if (!m_gamedata->GetisPose()) {
			//�^�C�}�[�����炷
			m_timer -= GameTime().GetFrameDeltaTime();
		}
	}
	m_degree = (m_timer / m_time) * 180.0f;
	m_sprite1.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.5f),
		DirectX::SpriteEffects_None, 0.5f);
	m_sprite2.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.8f),
		DirectX::SpriteEffects_None, 0.5f, m_degree);
	int minutes = m_timer / 60;
	int seconds = m_timer - minutes * 60;
	wchar_t output[256];
	wchar_t output2[256];
	CVector2 pos;
	//�c�莞�Ԃ�1�����傫�������番�\���A������������b�\��
	if (minutes < 1) {
		swprintf_s(output, L"%d\n", seconds);
		swprintf_s(output2, L"�r���E");
		if (seconds > 9) {
			pos = CVector2(1070.0f, 50.0f);
		}
		else {
			pos = CVector2(1040.0f, 50.0f);
		}
	}
	else {
		swprintf_s(output, L"%d\n", minutes);
		swprintf_s(output2, L"�t��\n");
		pos = CVector2(1040.0f, 50.0f);
	}
	m_font.DrawScreenPos(L"�A�g\n", CVector2(960.0f, 50.0f), CVector4::Yellow(), CVector2(0.6f, 0.6f));
	m_font.DrawScreenPos(output, CVector2(1000.0f, 25.0f), CVector4::Yellow(), CVector2(1.2f, 1.2f));
	m_font.DrawScreenPos(output2, pos, CVector4::Yellow(), CVector2(0.6f, 0.6f));
}
