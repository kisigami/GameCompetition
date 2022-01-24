#pragma once

//クラス宣言
class Fade;   //フェードクラス
class Title;  //タイトルクラス

/// <summary>
/// ゲームクリア
/// </summary>
class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ゲームクリアのフェード処理
	/// </summary>
	void GameClearFade();
	/// <summary>
	/// アルファ値の処理
	/// </summary>
	void Alpha();

	SoundSource* m_bgm = nullptr;                //BGM
	SpriteRender m_clearButtom;                  //一番下の画像
	SpriteRender m_clearBlack;                   //黒い画像
	SpriteRender m_titleFont;                    //タイトルの文字の画像
	SpriteRender m_clearFont;                    //ゲームクリアの文字の画像
	SpriteRender m_choice;                       //矢印の画像
	bool         m_isWaitFadeout = false;        //フェードフラグ
	float        m_alpha = 1.0f;                 //アルファ値1
	float        m_alpha2 = 1.0f;                //アルファ値2
	float        m_alpha3 = 1.0f;                //アルファ値3
	Title*       m_title = nullptr;              //タイトル
	Fade*        m_fade = nullptr;               //フェード
};

