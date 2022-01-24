#include "stdafx.h"
#include "GameClear.h"

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
    const float CHOICE_POSITION = -65.0f;   //矢印の画像の座標
}

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
    //BGMを削除する
    DeleteGO(m_bgm);
}

bool GameClear::Start()
{
    //画像を読み込む
    m_clearButtom.Init("Assets/sprite/clear/clearbuttom.dds",SIDE_SIZE,LENGTH_SIZE);
    m_clearBlack.Init("Assets/sprite/clear/clearblack.dds", SIDE_SIZE, LENGTH_SIZE);
    m_clearBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
    m_titleFont.Init("Assets/sprite/clear/titlefont.dds", SIDE_SIZE, LENGTH_SIZE);
    m_clearFont.Init("Assets/sprite/clear/clearfont.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
   
    //SEとBGMを読み込む
    g_soundEngine->ResistWaveFileBank(24, "Assets/sound/clear2.wav");
    g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");
   
    //BGMを再生する
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(24);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);

    m_fade = FindGO<Fade>("fade");
    //フェードイン
    m_fade->StartFadeIn();
	return true;
}
                                            
void GameClear::Update()
{
    /// <summary>
    /// ゲームクリアのフェード処理
    /// </summary>
    GameClearFade();
    /// <summary>
    /// アルファ値の処理
    /// </summary>
    Alpha();

    //画像の更新処理
	m_clearButtom.Update();
    m_clearBlack.Update();
    m_clearFont.Update();
    m_choice.Update();
    m_titleFont.Update();
}

void GameClear::GameClearFade()
{
    //フェードフラグがtrueだったら
    if (m_isWaitFadeout) {
        //フェード中ではなかったら
        if (!m_fade->IsFade()) {
            //タイトルを作成する
            NewGO<Title>(0, "title");
            //自身を削除する
            DeleteGO(this);
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
            //フェードフラグをtrueにする
            m_isWaitFadeout = true;
            //フェードアウト
            m_fade->StartFadeOut();
        }
    }
}

void GameClear::Alpha()
{
    //フェードフラグがtrueだったら
    if (m_isWaitFadeout)
    {
        //アルファ値を変化させる
        m_alpha += g_gameTime->GetFrameDeltaTime() * ALPHA_SPEED;
    }

    //タイトルの文字の画像を点滅させる
    m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

    //アルファ値を変化させる
    m_alpha2 += g_gameTime->GetFrameDeltaTime() * ALPHA2_SPEED;
    m_alpha3 += g_gameTime->GetFrameDeltaTime() * ALPHA3_SPEED;

    //矢印の画像を点滅させる
    m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
    //ゲームクリアの文字の画像を点滅させる
    m_clearFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void GameClear::Render(RenderContext& rc)
{
    //画像の描画処理
	m_clearFont.Draw(rc);
    m_clearBlack.Draw(rc);
    m_titleFont.Draw(rc);
    m_choice.Draw(rc);
    m_clearFont.Draw(rc);
}