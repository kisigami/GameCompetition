#pragma once

//クラス宣言
class Tower;

/// <summary>
/// タワーUI
/// </summary>
class TowerUi:public IGameObject
{
public:
	TowerUi();
	~TowerUi();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// 耐久値の大きさ
	/// </summary>
	void DurabilityScale();

	SpriteRender m_towerIcon;                 //タワーアイコン
	SpriteRender m_durabilityFrame;           //耐久値の枠の画像
	SpriteRender m_durability;                //耐久値の画像
	SpriteRender m_durabilityButtom;          //耐久値の底の画像
	SpriteRender m_durabilityFont;            //耐久値の文字の画像
	float        m_durabilityScale = 1.0f;    //耐久値の画像の大きさ
	Tower* m_tower = nullptr;                 //タワー
};

