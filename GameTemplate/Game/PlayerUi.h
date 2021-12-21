#pragma once

class Player;

class PlayerUi:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void HpScale();

	SpriteRender PlayerIcon;
	SpriteRender PlayerHpMidori;
	SpriteRender PlayerHpWaku;
	SpriteRender PlayerMpMidori;
	SpriteRender PlayerMpWaku;
	SpriteRender PlayerHpButtom;
	SpriteRender PlayerMpButtom;
	Player* m_player;
	float PlayerHpScale = 1.0f;
	float PlayerMpScale = 1.0f;
};

