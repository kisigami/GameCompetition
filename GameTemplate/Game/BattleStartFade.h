#pragma once

class BattleStartFade:public IGameObject
{
public:
	enum EnState
	{
		enState_FadeIn,     //フェードイン
		enState_FadeOut,    //フェードアウト
		enState_Idle,       //待機
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//フェードイン
	void StartFadeIn()
	{
		//フェードインステートへ
		m_state = enState_FadeIn;
	}
	//フェードアウト
	void StartFadeOut()
	{
		//フェードアウトステートへ
		m_state = enState_FadeOut;
	}
private:
	SpriteRender m_battlefade;             //スプライトレンダー
	Vector3      m_position;               //座標
	EnState      m_state = enState_Idle;   //フェードステート
	float        m_currentAlpha = 0.0f;    //アルファ値
};

