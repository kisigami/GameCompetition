#pragma once

//クラス宣言
class Fade;            //フェードクラス
class Game;            //ゲームクラス
class HowToPlay;       //遊び方

/// <summary>
/// タイトル
/// </summary>
class Title : public IGameObject
{
public:
	//タイトルステート
	enum EnTitleState
	{
		enTitleState_Gamestart,    //ゲームスタートステート
		enTitleState_Howtoplay,    //遊び方ステート
	};
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// タイトルのフェード処理
	/// </summary>
	void TitleFade();
	/// <summary>
	/// アルファ値の設定
	/// </summary>
	void Alpha();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();

	SoundSource*    m_bgm = nullptr;                        //BGM
	SpriteRender    m_titleButtom;                          //タイトルの一番下の画像
	SpriteRender    m_titleSword;                           //剣の画像
	SpriteRender    m_gamestartFont;                        //ゲームスタートの文字の画像
	SpriteRender    m_howtoplayFont;                        //遊び方の文字の画像
	SpriteRender    m_titleBlack;                           //黒い画像
	SpriteRender    m_choice;                               //矢印の画像
	SpriteRender    m_titleFont;                            //タイトルの文字の画像
	bool	        m_isWaitFadeout = false;                //フェードフラグ
	EnTitleState    m_titleState = enTitleState_Gamestart;  //タイトルステート
	float           m_alpha = 1.0f;                         //アルファ値1
	float           m_alpha2 = 1.0f;                        //アルファ値2
	float           m_alpha3 = 1.0f;                        //アルファ値3
	Fade*           m_fade = nullptr;		                //フェード
	Game*           m_game = nullptr;                       //ゲーム
	HowToPlay*      m_howtoplay = nullptr;                  //遊び方
};
