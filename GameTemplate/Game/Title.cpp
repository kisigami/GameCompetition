#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "HowToPlay.h"
#include "Fade.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;        //画像の横幅
	const float LENGTH_SIZE = 1080.0f;      //画像の縦幅
	const float ALPHA_SPEED = 20.0f;        //アルファ値1の変化速度
	const float ALPHA2_SPEED = 5.0f;        //アルファ値2の変化速度
	const float ALPHA3_SPEED = 1.0f;        //アルファ値3の変化速度
	const float BLACKSPRITE_ALPHA = 0.7f;   //黒い画像のアルファ値
	const float CHOICE_POSITION = -144.0f;  //矢印の画像の座標
}

Title::Title()
{

}

Title::~Title()
{
	//BGMを削除する
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	//画像の読み込み
	m_titleButtom.Init("Assets/sprite/tite/titleButtom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_gamestartFont.Init("Assets/sprite/tite/titleGS.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleFont.Init("Assets/sprite/tite/titlefont.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleBlack.Init("Assets/sprite/tite/titleBlack.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
	m_howtoplayFont.Init("Assets/sprite/tite/titleA.dds", SIDE_SIZE, LENGTH_SIZE);
	m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_titleSword.Init("Assets/sprite/tite/titlesword.dds", SIDE_SIZE, LENGTH_SIZE);
	m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
	
	//SEとBGMを読み込む
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/kettei.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/title2.wav");

	//BGMを再生する
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(23);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);

	//フェードを探す
	m_fade = FindGO<Fade>("fade");
	m_howtoplay = FindGO<HowToPlay>("howtoplay");
	//フェードインする
	m_fade->StartFadeIn();
	return true;
}

void Title::Update()
{
	/// <summary>
	/// フェード処理
	/// </summary>
	TitleFade();
	/// <summary>
	/// アルファ値の処理
	/// </summary>
	Alpha();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	ManageState();

	//画像の更新
	m_titleButtom.Update();
	m_titleBlack.Update();
	m_gamestartFont.Update();
	m_howtoplayFont.Update();
	m_choice.Update();
	m_titleFont.Update();
}

void Title::TitleFade()
{
	//フェードフラがグtrueだったら
	if (m_isWaitFadeout) {
		//フェード中ではなかったら
		if (!m_fade->IsFade())
		{
			//ゲームスタートステートだったら
			if (m_titleState == enTitleState_Gamestart)
			{
				//ゲームを作成する
				NewGO<Game>(0, "game");
				//自身を削除する
				DeleteGO(this);
				return;
			}
			//遊び方ステートだったら
			else if (m_titleState == enTitleState_Howtoplay)
			{
				//遊び方画面を作成する
				m_howtoplay = NewGO<HowToPlay>(0, "howtoplay");
				//自身を削除する
				DeleteGO(this);
				return;
			}
		}
	}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(30);
			se->Play(false);
			se->SetVolume(0.2f);
			//フェードフラグをtrueにする
			m_isWaitFadeout = true;
			//フェードアウトする
			m_fade->StartFadeOut();
		}
	}
}

void Title::Alpha()
{
	//フェードフラグがtrueだったら
	if (m_isWaitFadeout)
	{
		//アルファ値を変化させる
		m_alpha += g_gameTime->GetFrameDeltaTime() * ALPHA_SPEED;
	}
	//アルファ値を変化させる
	m_alpha2 += g_gameTime->GetFrameDeltaTime() * ALPHA2_SPEED;
	m_alpha3 += g_gameTime->GetFrameDeltaTime() * ALPHA3_SPEED;


	//ゲームスタートステートだったら
	if (m_titleState == enTitleState_Gamestart)
	{
		//ゲームスタートの文字を点滅させる
		m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}
	//遊び方ステートだったら
	else if (m_titleState == enTitleState_Howtoplay)
	{
		//遊び方の文字を点滅させる
		m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}

	//矢印を点滅させる
	m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
	//タイトルの文字を点滅させる
	m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void Title::ManageState()
{
	//下ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		//ゲームスタートステートだったら
		if (m_titleState == enTitleState_Gamestart)
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->Play(false);
			se->SetVolume(0.2f);
			//遊び方ステートへ
			m_titleState = enTitleState_Howtoplay;
			//ゲームスタートの文字の色を変化させる
			m_gamestartFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			//遊び方の文字の色を変化させる
			m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//矢印の座標を少し下にする
			m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
		}
	}
	//上ボタンが押されたら
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		//遊び方ステートだったら
		if (m_titleState == enTitleState_Howtoplay)
		{
			//SEを再生する
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->Play(false);
			se->SetVolume(0.2f);
			//ゲームスタートステートへ
			m_titleState = enTitleState_Gamestart;
			//遊び方の文字の色を変化させる
			m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			//ゲームスタートの文字の色を変化させる
			m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//矢印の座標を設定
			m_choice.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Title::Render(RenderContext& rc)
{
	//画像を描画する
	m_titleButtom.Draw(rc);
	m_titleBlack.Draw(rc);
	m_gamestartFont.Draw(rc);
	m_howtoplayFont.Draw(rc);
	m_titleSword.Draw(rc);
	m_choice.Draw(rc);
	m_titleFont.Draw(rc);
}