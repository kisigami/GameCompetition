#pragma once

class BattleStartFade:public IGameObject
{
public:
	enum EnState
	{
		enState_FadeIn,
		enState_FadeOut,
		enState_Idle,
	};
	enum EnEndState
	{
		enEndState_FadeIn,
		enEndState_FadeOut,
		enEndState_Idle,
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}

	void StartEndFadeIn()
	{
		m_endstate = enEndState_FadeIn;
	}
	
	void StartEndFadeOut()
	{
		m_endstate = enEndState_FadeOut;
	}
	const bool IsFade() const
	{
		return m_endstate != enEndState_Idle;
	}
	SpriteRender m_battlefade;
	SpriteRender m_worningfont;
	SpriteRender m_battleend;
	Vector3      m_position;
	EnState      m_state = enState_Idle;
	EnEndState   m_endstate = enEndState_Idle;
	float        m_currentAlpha = 0.0f;
	float        m_currentAlpha2 = 0.0f;
};

