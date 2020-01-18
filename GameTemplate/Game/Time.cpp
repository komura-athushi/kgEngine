#include "stdafx.h"
#include "Time.h"
#include "Game.h"
#include "GameData.h"
#include "sound\SoundSource.h"

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
	if (m_game->GetisOwaOwari()) {
		return;
	}
	//�^�C�}�[��0�ɂȂ�΃X�e�[�W�I���t���O�����Ă�
	if (m_timer <= 0.0000001f) {
		m_timer = 0.0f;
		m_game->SetOwaOwari();
		CSoundSource* se = new CSoundSource();
		se->Init(L"Assets/sound/roll.wav");
		se->Play(false);
	}
	else {
		if (!m_gamedata->GetisPose()) {
			//�^�C�}�[�����炷
			m_timer -= GameTime().GetFrameDeltaTime();
		}
	}
	m_degree = (m_timer / m_time) * 180.0f;
	m_sprite1.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.7f),
		DirectX::SpriteEffects_None, 0.5f);
	m_sprite2.DrawScreenPos(CVector2(1180.0f, 130.0f), CVector2(0.6f, 0.6f), CVector2(0.5f, 0.5f), 0.0f, CVector4(1.0f, 1.0f, 1.0f, 0.7f),
		DirectX::SpriteEffects_None, 0.5f, m_degree);
	int minutes = m_timer / 60;
	int seconds = m_timer - minutes * 60;
	wchar_t output[256];
	wchar_t output2[256];
	CVector2 pos;
	CVector4 color;

	//�c�莞�Ԃ�1�����傫�������番�\���A������������b�\��
	//�b
	if (minutes < 1) {
		swprintf_s(output, L"%d\n", seconds);
		swprintf_s(output2, L"�r���E");
		color = CVector4::Red();
		if (seconds > 9) {
			pos = CVector2(1070.0f, 50.0f);
		}
		else {
			pos = CVector2(1040.0f, 50.0f);
		}
		if (m_beforeSeconds != seconds) {
			CSoundSource* se = new CSoundSource();
			se->Init(L"Assets/sound/counttime.wav");
			se->Play(false);
			se->SetVolume(2.2f);
		}
		m_beforeSeconds = seconds;
	}
	//��
	else {
		swprintf_s(output, L"%d\n", minutes);
		swprintf_s(output2, L"�t��\n");
		pos = CVector2(1040.0f, 50.0f);
		color = CVector4::Yellow();
	}

	m_font.DrawScreenPos(L"�A�g\n", CVector2(960.0f, 50.0f), CVector4(0.0f, 0.0f, 0.0f, 1.0f), CVector2(0.67f, 0.67f));
	m_font.DrawScreenPos(output, CVector2(1000.0f, 25.0f), CVector4(0.0f, 0.0f, 0.0f, 1.0f), CVector2(1.3f, 1.3f));
	m_font.DrawScreenPos(output2, pos, CVector4(0.0f, 0.0f, 0.0f, 1.0f), CVector2(0.67f, 0.67f));

	m_font.DrawScreenPos(L"�A�g\n", CVector2(960.0f, 50.0f), CVector4::Yellow(), CVector2(0.6f, 0.6f));
	m_font.DrawScreenPos(output, CVector2(1000.0f, 25.0f), color, CVector2(1.2f, 1.2f));
	m_font.DrawScreenPos(output2, pos, CVector4::Yellow(), CVector2(0.6f, 0.6f));
}
