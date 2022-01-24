#pragma once

//クラス宣言
class Fade;     //フェードクラス
class Title;    //タイトルクラス
class Game;     //ゲームクラス

/// <summary>
/// ゲームオーバー
/// </summary>
class GameOver : public IGameObject
{
public:
	/// <summary>
	/// ゲームオーバーステート
	/// </summary>
	enum EnOverState
	{
		enOverState_Continue,      //コンテニューステート
		enOverState_Title,         //タイトルステート
	};
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// ゲームクリアのフェード処理
	/// </summary>
	void GameOverFade();
	/// <summary>
	/// アルファ値の処理
	/// </summary>
	void Alpha();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();

	EnOverState      m_overState = enOverState_Continue;  //ゲームオーバーステート
	SoundSource*     m_bgm = nullptr;                     //BGM
	SpriteRender     m_overButtom;                        //一番下の画像
	SpriteRender     m_overBlack;                         //黒い画像     
	SpriteRender     m_overFont;                          //ゲームオーバーの文字の画像
	SpriteRender     m_continyuFont;                      //コンテニューの文字の画像
	SpriteRender     m_titleFont;                         //タイトルの文字の画像
	SpriteRender     m_choice;                            //矢印の画像
	Title*           m_title = nullptr;                   //タイトル
	Fade*            m_fade = nullptr;                    //フェード
	Game*            m_game = nullptr;                    //ゲーム
	bool             m_isWaitFadeout = false;             //フェードフラグ
	float            m_alpha = 1.0f;                      //アルファ値1
	float            m_alpha3 = 1.0f;                     //アルファ値2
	float            m_alpha2 = 1.0f;                     //アルファ値3
};

