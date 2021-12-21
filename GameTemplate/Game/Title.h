#pragma once

//フェードクラス
class Fade;
class Game;
//タイトル
class Title : public IGameObject
{
	enum EnTitleState
	{
		enTitleState_gamestart,
		enTitleState_Rule,
		enTitleState_OperationExplanation,
	};
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void ProcessCommonStateTransition();
	void ManageState();

private:
	SpriteRender    m_spriteRender;           //スプリントレンダー
	SpriteRender    m_titleFont;
	SpriteRender    m_gamestartFont;
	SpriteRender    m_asobikataFont;
	SpriteRender    m_sousahouhouFont;
	SpriteRender    m_blackButton;
	SpriteRender    m_whiteWaku;
	SpriteRender    m_player;
	Vector3 m_position;
	Fade* m_fade = nullptr;		  //フェード。
	bool	        m_isWaitFadeout = false;  //フラグ?
	Game* m_game = nullptr;
	EnTitleState m_TitleState = enTitleState_gamestart;
};
