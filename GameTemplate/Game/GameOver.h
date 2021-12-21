#pragma once
class Fade;
class Title;

class GameOver :public IGameObject
{
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	SpriteRender m_spriteRender;
	Title* m_title = nullptr;
	Fade* m_fade = nullptr;
	bool m_isWaitFadeout = false;
};

