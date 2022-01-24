#pragma once

//クラス宣言
class Player;      //プレイヤークラス

/// <summary>
/// プレイヤーUI
/// </summary>
class PlayerUi : public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// MPの画像の大きさの処理
	/// </summary>
	void MpScale();

	SpriteRender m_playerIcon;              //プレイヤーアイコンの画像
	SpriteRender m_mp;                      //MPの画像
	SpriteRender m_mpFarame;                //MPの枠画像
	SpriteRender m_mpButtom;                //MPの底の画像
	float        m_mpSpriteScale = 1.0f;    //MPの画像の大きさ
	Player* m_player = nullptr;             //プレイヤー
};

