#include "stdafx.h"
#include "GameOver.h"

#include "Game.h"
#include "Fade.h"
#include "Title.h"

namespace
{
    const float SIDE_SIZE = 1920.0f;        //画像の横幅
    const float LENGTH_SIZE = 1080.0f;      //画像の縦幅
    const float ALPHA_SPEED = 20.0f;        //アルファ値1の変化速度
    const float ALPHA2_SPEED = 5.0f;        //アルファ値2の変化速度
    const float ALPHA3_SPEED = 1.0f;        //アルファ値3の変化速度
    const float BLACKSPRITE_ALPHA = 0.7f;   //黒い画像のアルファ値
    const float CHOICE_POSITION = -119.0f;  //矢印の画像の座標
}

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
    //BGMを削除する
    DeleteGO(m_bgm);
}

bool GameOver::Start()
{
    //画像の読み込み
	m_overButtom.Init("Assets/sprite/overbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overBlack.Init("Assets/sprite/overblack.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
    m_titleFont.Init("Assets/sprite/title.dds", SIDE_SIZE, LENGTH_SIZE);
    m_titleFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    m_continyuFont.Init("Assets/sprite/continyu.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overFont.Init("Assets/sprite/overfont.dds", SIDE_SIZE, LENGTH_SIZE);

    //SEとBGMを読み込む
    g_soundEngine->ResistWaveFileBank(13, "Assets/sound/over.wav");
    g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");
    g_soundEngine->ResistWaveFileBank(10, "Assets/sound/kettei.wav");

    //BGMを再生する
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(25);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);

	m_fade = FindGO<Fade>("fade");
    //フェードインする
	m_fade->StartFadeIn();
	return true;
}

void GameOver::Update()
{
    /// <summary>
    /// ゲームオーバーのフェード処理
    /// </summary>
    GameOverFade();
    /// <summary>
    /// アルファ値の処理
    /// </summary>
    Alpha();
    /// <summary>
    /// 各ステートの遷移処理
    /// </summary>
    ManageState();

    //画像の更新処理
	m_overButtom.Update();
    m_overBlack.Update(); 
    m_overFont.Update();
    m_continyuFont.Update();
    m_titleFont.Update();
}

void GameOver::GameOverFade()
{
    //フェードフラグをtrueにする
    if (m_isWaitFadeout) {
        //フェード中ではなかったら
        if (!m_fade->IsFade()) {
            //タイトルステートだったら
            if (m_overState == enOverState_Title) {
                //タイトルを作成する
                NewGO<Title>(0, "title");
                //自身を削除する
                DeleteGO(this);
            }
            //コンテニューステートだったら
            else if (m_overState == enOverState_Continue)
            {
                //ゲームを作成する
                NewGO<Game>(0, "game");
                //自身を削除する
                DeleteGO(this);
            }
        }
    }

    else {
        //Aボタンが押されたら
        if (g_pad[0]->IsTrigger(enButtonA)) {
            //SEを再生する
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(30);
            se->Play(false);
            se->SetVolume(0.2f);
            //フェードフラグがtrueだったら
            m_isWaitFadeout = true;
            //フェードアウトする
            m_fade->StartFadeOut();
        }
    }
}

void GameOver::Alpha()
{
    //フェードフラグがtrueだったら
    if (m_isWaitFadeout)
    {
        //アルファ値を変化させる
        m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
    }

    //コンテニューステートだったら
    if (m_overState == enOverState_Continue)
    {
        //コンテニューの文字の画像を点滅させる
        m_continyuFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }
    //タイトルステートだったら
    else if (m_overState == enOverState_Title)
    {
        //タイトルの文字の画像を点滅させる
        m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }

    //アルファ値を変化させる
    m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
    m_alpha3 += g_gameTime->GetFrameDeltaTime() * 1.0f;

    //矢印の画像を点滅させる
    m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
    //ゲームオーバーの文字を点滅させる
    m_overFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void GameOver::ManageState()
{
    //下ボタンが押されたら
    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        //コンテニューステートだったら
        if (m_overState == enOverState_Continue)
        {
            //SEを再生する
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(29);
            se->Play(false);
            se->SetVolume(0.2f);
            //矢印の画像の座標を少し下げる
            m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
            //タイトルの文字の画像の色を白くする
            m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            //コンテニューの文字の画像の色を黒くする
            m_continyuFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            //タイトルステートへ
            m_overState = enOverState_Title;
        }
    }
    //上ボタンが押されたら
    if (g_pad[0]->IsTrigger(enButtonUp))
    {
        //タイトルステートだったら
        if (m_overState == enOverState_Title)
        {
            //SEを再生する
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(29);
            se->Play(false);
            se->SetVolume(0.2f);
            //矢印の画像の座標を少し上にする
            m_choice.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
            //タイトルの文字の画像の色を黒くする
            m_titleFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            //コンテニューの文字の画像の色を白くする
            m_continyuFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            //コンテニューステートへ
            m_overState = enOverState_Continue;
        }
    }
}

void GameOver::Render(RenderContext& rc)
{
    //画像の描画処理
	m_overButtom.Draw(rc);
    m_overBlack.Draw(rc);
    m_titleFont.Draw(rc);
    m_continyuFont.Draw(rc);
    m_overFont.Draw(rc);
    m_choice.Draw(rc);
}