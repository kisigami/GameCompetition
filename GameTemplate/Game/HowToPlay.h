#pragma once

class Title;
class Fade;

class HowToPlay:public IGameObject
{
public:
	enum EnHowToPlayState
	{
		enHowToPlayState_sousa,
		enHowToPlayState_setumei1,
		enHowToPlayState_setumei2,
	};
public:
	HowToPlay();
	~HowToPlay();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender sousahouhou;
	SpriteRender setumei1;
	SpriteRender setumei2;
	EnHowToPlayState m_HowToPlayState = enHowToPlayState_sousa;
	Title* m_title = nullptr;
	Fade* m_fade = nullptr;
	bool m_isWaitFadeout = false;
	SoundSource* m_bgm = nullptr;
	SoundSource* m_se = nullptr;
};

