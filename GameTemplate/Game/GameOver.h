#pragma once
class Fade;
class Title;
class Game;

class GameOver :public IGameObject
{
	enum EnOverState
	{
		enOverState_continue,
		enOverState_title,
	};
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	EnOverState m_OverState = enOverState_continue;
	SoundSource* m_bgm = nullptr;
	SoundSource* m_se = nullptr;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRender2;
	SpriteRender m_spriteRender3;
	SpriteRender m_spriteRender4;
	SpriteRender m_spriteRender5;
	SpriteRender m_spriteRender6;
	Title* m_title = nullptr;
	Fade* m_fade = nullptr;
	Game* m_game = nullptr;
	bool m_isWaitFadeout = false;
	float m_alpha3 = 1.0f;
	float m_alpha2 = 1.0f;
	float m_alpha = 1.0f;
};

