#include "stdafx.h"
#include "HowToPlay.h"
#include "Fade.h"
#include "Title.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

HowToPlay::HowToPlay()
{

}

HowToPlay::~HowToPlay()
{
	DeleteGO(m_bgm);
}

bool HowToPlay::Start()
{
	g_soundEngine->ResistWaveFileBank(14, "Assets/sound/howtoplay.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/kettei.wav");
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(28);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.25f);

	sousahouhou.Init("Assets/sprite/tite/sousa.dds",1920.0f,1080.0f);
	setumei1.Init("Assets/sprite/tite/setumei1.dds", 1920.0f, 1080.0f);
	setumei2.Init("Assets/sprite/tite/setumei2.dds", 1920.0f, 1080.0f);
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void HowToPlay::Update()
{
	if (m_isWaitFadeout) {
		//もしフェード中ではなかったら
		if (!m_fade->IsFade()) {
			//ゲームを作成する
			NewGO<Title>(0, "title");
			//自信を削除する
			DeleteGO(this);
		}
	}
	else {
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_se = NewGO<SoundSource>(0);
			m_se->Init(30);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}


	if (g_pad[0]->IsTrigger(enButtonRight))
	{
		switch (m_HowToPlayState)
		{
		case enHowToPlayState_sousa:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_HowToPlayState = enHowToPlayState_setumei1;
			break;
		case enHowToPlayState_setumei1:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_HowToPlayState = enHowToPlayState_setumei2;
		default:
			break;
		}
	}
	
	if (g_pad[0]->IsTrigger(enButtonLeft))
	{
		switch (m_HowToPlayState)
		{
		case enHowToPlayState_setumei1:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_HowToPlayState = enHowToPlayState_sousa;
			break;
		case enHowToPlayState_setumei2:
			m_se = NewGO<SoundSource>(0);
			m_se->Init(29);
			m_se->Play(false);
			m_se->SetVolume(0.2f);
			m_HowToPlayState = enHowToPlayState_setumei1;
			break;
		default:
			break;
		}
	}

	sousahouhou.Update();
	setumei1.Update();
	setumei2.Update();
}

void HowToPlay::Render(RenderContext& rc)
{
	switch (m_HowToPlayState)
	{
	case HowToPlay::enHowToPlayState_sousa:
		sousahouhou.Draw(rc);
		break;
	case HowToPlay::enHowToPlayState_setumei1:
		setumei1.Draw(rc);
		break;
	case HowToPlay::enHowToPlayState_setumei2:
		setumei2.Draw(rc);
		break;
	default:
		break;
	}
}