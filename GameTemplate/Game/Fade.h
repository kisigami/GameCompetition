#pragma once

//�t�F�[�h
class Fade : public IGameObject
{
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;

	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}

	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}

	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}

	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

	enum EnState
	{
		//�t�F�[�h�C��
		enState_FadeIn,
		//�t�F�[�h�A�E�g
		enState_FadeOut,
		//�ҋ@��
		enState_Idle,
	};

	SpriteRender    m_spriteRender;

	EnState         m_state = enState_Idle;
	float           m_currentAlpha = 1.0f;
};

