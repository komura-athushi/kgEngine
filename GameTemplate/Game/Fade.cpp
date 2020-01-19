#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
	SetPriorityGO(this, 5);
}


Fade::~Fade()
{
}

bool Fade::Start()
{
	//ƒ[ƒh‰æ–Ê‚Ì‰æ‘œ‚ð“Ç‚Ýž‚Þ
	m_sprite.Init(L"Resource/sprite/loading.dds");
	m_loading.Init(L"Resource/sprite/loading3.dds");
	return true;
}
void Fade::Update()
{
	const float Scaler = 1.0f;

	switch (m_state) {
	case enState_FadeIn:
		//m_currentAlpha -= 1.5f * GameTime().GetFrameDeltaTime();
		m_position.y += FRAME_BUFFER_H * GameTime().GetFrameDeltaTime() * Scaler;
		m_position.x -= FRAME_BUFFER_W * GameTime().GetFrameDeltaTime() * Scaler;
		if (m_position.y >= FRAME_BUFFER_H) {
			m_state = enState_Idle;
			m_position = CVector2(FRAME_BUFFER_W * 2, -FRAME_BUFFER_H);
		}
		/*if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}*/
		break;
	case enState_FadeOut:
		//m_currentAlpha += 1.5f * GameTime().GetFrameDeltaTime();
		m_position.y += FRAME_BUFFER_H * GameTime().GetFrameDeltaTime() * Scaler;
		m_position.x -= FRAME_BUFFER_W * GameTime().GetFrameDeltaTime() * Scaler;
		if (m_position.y >= FRAME_BUFFER_H / 2) {
			m_state = enState_FadeInLoading;
		}
		/*if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}*/
		break;
	case enState_FadeInLoading:
		m_currentAlpha += 1.5f * GameTime().GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
	case enState_FadeOutLoading:
		m_currentAlpha -= 1.5f * GameTime().GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_FadeIn;
		}
		break;
	case enState_Idle:

		break;
	}
}
void Fade::PostRender()
{
	if (m_position.y > -FRAME_BUFFER_H) {
		m_sprite.DrawScreenPos(m_position, CVector2(1.05f,1.05f), CVector2(0.5f,0.5f),
			0.0f,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			DirectX::SpriteEffects_None,
			0.1f);
		m_loading.DrawScreenPos(CVector2::Zero(), CVector2::One(), CVector2::Zero(),
			0.0f,
			{ m_currentAlpha, m_currentAlpha, m_currentAlpha, m_currentAlpha},
			DirectX::SpriteEffects_None,
			0.1f
		);
	}
}