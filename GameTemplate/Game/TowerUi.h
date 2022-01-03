#pragma once

class Tower;

class TowerUi:public IGameObject
{
public:
	TowerUi();
	~TowerUi();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	void HpScale();

	SpriteRender TowerIconRender;
	SpriteRender TowerHpwaku;
	SpriteRender TowerMidori;
	SpriteRender TowerHpButtom;
	SpriteRender TowerFont;
	Tower* m_tower = nullptr;
	float towerhpscale = 1.0f;
};

