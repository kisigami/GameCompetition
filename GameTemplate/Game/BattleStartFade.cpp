#include "stdafx.h"
#include "BattleStartFade.h"

bool BattleStartFade::Start()
{
	m_battlefade.Init("Assets/sprite/battlefade.dds", 1920.0f, 1080.0f);
	m_battleend.Init("Assets/sprite/battleend.dds", 1920.0f, 1080.0f);

	return true;
}

void BattleStartFade::Update()
{
	switch (m_state)
	{
	case BattleStartFade::enState_FadeIn:
		m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
	case BattleStartFade::enState_FadeOut:
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
	case BattleStartFade::enState_Idle:
		break;
	}
		
	switch (m_endstate)
	{
	case BattleStartFade::enEndState_FadeIn:
		m_currentAlpha2 -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha2 < 0.0f)
		{
			m_currentAlpha2 = 0.0f;
			m_endstate = enEndState_Idle;
		}
		break;
	case BattleStartFade::enEndState_FadeOut:
		m_currentAlpha2 += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha2 > 1.0f)
		{
			m_currentAlpha2 = 1.0f;
			m_endstate = enEndState_Idle;
		}
		break;
	case BattleStartFade::enEndState_Idle:
		
		
		break;
	}

}

void BattleStartFade::Render(RenderContext& rc)
{
	if (m_currentAlpha > 0.0f)
	{
		m_battlefade.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_battlefade.Draw(rc);
	
	}
	if (m_currentAlpha2 > 0.0f)
	{
		m_battleend.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha2 });
		m_battleend.Draw(rc);
	}
}