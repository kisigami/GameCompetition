#pragma once

/// <summary>
/// フェード
/// </summary>
class Fade : public IGameObject
{
public:
	/// <summary>
	/// フェードステート
	/// </summary>
	enum EnState
	{
		enState_FadeIn,   //フェードインステート
		enState_FadeOut,  //フェードアウトステート
		enState_Idle,     //待機ステート
	};
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// フェードイン
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// フェードアウト
	/// </summary>
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// フェード中か？
	/// </summary>
	/// <returns>フェード中ならtrue</returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/// <summary>
	/// アルファ値を取得
	/// </summary>
	/// <returns>アルファ値</returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	SpriteRender    m_spriteRender;             //フェードの画像
	EnState         m_state = enState_Idle;     //フェードステート
	float           m_currentAlpha = 1.0f;      //アルファ値
};

