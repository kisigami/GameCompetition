#pragma once

//フェードクラス
class Fade;
class Game;
class HowToPlay;
//タイトル
class Title : public IGameObject
{
	enum EnTitleState
	{
		enTitleState_gamestart,
		enTitleState_Rule,
	};
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender    m_spriteRender;           //スプリントレンダー
	SpriteRender    m_titlesword;
	SpriteRender    m_gamestartFont;
	SpriteRender    m_asobikataFont;
	SpriteRender    m_blackButton;
	SpriteRender    m_yajirusi;
	SpriteRender    m_titlefont;

	Vector3 m_position;
	Fade* m_fade = nullptr;		  //フェード。
	bool	        m_isWaitFadeout = false;  //フラグ?
	Game* m_game = nullptr;
	EnTitleState m_TitleState = enTitleState_gamestart;
	SoundSource* m_bgm = nullptr;
	SoundSource* m_se = nullptr;
	HowToPlay* m_howtoplay = nullptr;
	float m_alpha = 1.0f;
	float m_alpha2 = 1.0f;
	float m_alpha3 = 1.0f;
};
