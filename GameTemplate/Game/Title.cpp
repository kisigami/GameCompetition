#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "HowToPlay.h"
#include "Fade.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

Title::Title()
{

}

Title::~Title()
{
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	g_soundEngine->ResistWaveFileBank(29, "Assets/sound/sentaku.wav");
	g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");

	g_soundEngine->ResistWaveFileBank(23, "Assets/sound/title2.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(23);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);

	//画像の読み込み
	m_titleButtom.Init("Assets/sprite/tite/titleButtom.dds", 1920.0f, 1080.0f);

	m_gamestartFont.Init("Assets/sprite/tite/titleGS.dds", 1920.0f, 1080.0f);

	m_titleFont.Init("Assets/sprite/tite/titlefont.dds", 1920.0f, 1080.0f);

	//選択画面の背景（黒）
	m_blackButton.Init("Assets/sprite/tite/titleBlack.dds", 1920.0f, 1080.0f);
	m_blackButton.SetMulColor(Vector4(1.0f,1.0f,1.0f,0.7f));


	//遊び方のフォント
	m_howtoplayFont.Init("Assets/sprite/tite/titleA.dds", 1920.0f, 1080.0f);
	m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	m_titleSword.Init("Assets/sprite/tite/titlesword.dds", 1920.0f, 1080.0f);

	m_choice.Init("Assets/sprite/yajirusi.dds", 1920.0f, 1080.0f);
	//フェードを探す
	m_fade = FindGO<Fade>("fade");
	m_howtoplay = FindGO<HowToPlay>("howtoplay");
	//フェードインする
	m_fade->StartFadeIn();

	return true;
}

void Title::Update()
{
	if (m_isWaitFadeout)
	{
		m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
	}

	m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
	m_alpha3 += g_gameTime->GetFrameDeltaTime() * 1.0f;

	if (m_TitleState == enTitleState_gamestart)
	{
		m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}
	else if (m_TitleState == enTitleState_Rule)
	{
		m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}

	m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
	m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));

	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		switch (m_TitleState)
		{
		case Title::enTitleState_gamestart:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_TitleState = enTitleState_Rule;
			m_gamestartFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			m_choice.SetPosition(Vector3(0.0f, -144.0f, 0.0f));
			break;
		default:
			break;
		}
	}
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		switch (m_TitleState)
		{
		case Title::enTitleState_Rule:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_TitleState = enTitleState_gamestart;
			m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			m_choice.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
			break;
		default:
			break;
		}
	}

		if (m_isWaitFadeout) {
			if (!m_fade->IsFade())
			{
				if (m_TitleState == enTitleState_gamestart)
				{
					DeleteGO(this);
					NewGO<Game>(0, "game");
					return;
				}
				else if (m_TitleState == enTitleState_Rule)
				{
					DeleteGO(this);
					m_howtoplay = NewGO<HowToPlay>(0, "howtoplay");
					return;
				}
			}
		}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_se = NewGO<SoundSource>(0);
			m_se->Init(30);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
	//画像の更新
	m_titleButtom.Update();
	m_blackButton.Update();
	m_gamestartFont.Update();
	m_howtoplayFont.Update();
	m_choice.Update();
	m_titleFont.Update();
}

void Title::Render(RenderContext& rc)
{
	//画像を描画する
	m_spriteRender.Draw(rc);
	m_blackButton.Draw(rc);
	m_gamestartFont.Draw(rc);
	m_asobikataFont.Draw(rc);
	m_titlesword.Draw(rc);
	m_yajirusi.Draw(rc);
	m_titlefont.Draw(rc);
}