#pragma once

class Fade;
class Title;

class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);


	SoundSource* m_bgm = nullptr;
	SoundSource* m_se = nullptr;
	SpriteRender m_spriteRender;
	SpriteRender m_spriteRender2;
	SpriteRender m_spriteRender3;
	SpriteRender m_spriteRender4;
	SpriteRender m_yajirusi;
	Title* m_title = nullptr;
	Fade* m_fade = nullptr;
	bool              m_isWaitFadeout = false;
	float             m_alpha = 1.0f;
	float             m_alpha2 = 1.0f;
	float             m_alpha3 = 1.0f;

};

