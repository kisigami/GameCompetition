#include "stdafx.h"
#include "GameOver.h"
#include "Game.h"
#include "Fade.h"
#include "Title.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
    DeleteGO(m_bgm);
}

bool GameOver::Start()
{
	m_spriteRender.Init("Assets/sprite/over.dds", 1920.0f, 1080.0f);

    m_spriteRender5.Init("Assets/sprite/overbuttom.dds", 1920.0f, 1080.0f);
    m_spriteRender5.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
   
    m_spriteRender2.Init("Assets/sprite/title.dds", 1920.0f, 1080.0f);
    m_spriteRender2.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));

    m_spriteRender3.Init("Assets/sprite/continyu.dds", 1920.0f, 1080.0f);

    m_spriteRender4.Init("Assets/sprite/yajirusi.dds", 1920.0f, 1080.0f);
  
    m_spriteRender6.Init("Assets/sprite/overfont.dds", 1920.0f, 1080.0f);

    g_soundEngine->ResistWaveFileBank(25, "Assets/sound/over.wav");
    g_soundEngine->ResistWaveFileBank(29, "Assets/sound/sentaku.wav");
    g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(25);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void GameOver::Update()
{
    if (m_isWaitFadeout)
    {
        m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
    }

    m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
    m_spriteRender4.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));

    if (m_OverState == enOverState_continue)
    {
        m_spriteRender3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }
    else if (m_OverState == enOverState_title)
    {
        m_spriteRender2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }

    m_alpha3 += g_gameTime->GetFrameDeltaTime() * 1.0f;
    m_spriteRender6.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));

    if (m_isWaitFadeout) {
        if (!m_fade->IsFade()) {
            if (m_OverState == enOverState_title) {
                NewGO<Title>(0, "title");
                DeleteGO(this);
            }
            else
            {
                NewGO<Game>(0, "game");
                DeleteGO(this);
            }
        }
    }

    else {
        if (g_pad[0]->IsTrigger(enButtonA)) {
            m_se = NewGO<SoundSource>(0);
            m_se->Init(30);
            m_se->Play(false);
            m_se->SetVolume(0.2f);
            m_isWaitFadeout = true;
            m_fade->StartFadeOut();
        }
    }

    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        switch (m_OverState)
        {
        case GameOver::enOverState_continue:
            m_se = NewGO<SoundSource>(0);
            m_se->Init(29);
            m_se->Play(false);
            m_se->SetVolume(0.2f);
            m_OverState = enOverState_title;
            m_spriteRender4.SetPosition(Vector3(0.0f, -119.0f, 0.0f));
            m_spriteRender2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            m_spriteRender3.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            break;
        default:
            break;
        }
    }
    if (g_pad[0]->IsTrigger(enButtonUp))
    {
        switch (m_OverState)
        {
        case GameOver::enOverState_title:
            m_OverState = enOverState_continue;
            m_se = NewGO<SoundSource>(0);
            m_se->Init(29);
            m_se->Play(false);
            m_se->SetVolume(0.2f);
            m_spriteRender4.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
            m_spriteRender2.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            m_spriteRender3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            break;
        default:
            break;
        }
    }
  
	m_spriteRender.Update();
    m_spriteRender2.Update(); 
    m_spriteRender3.Update();
    m_spriteRender4.Update();
    m_spriteRender6.Update();
}

void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
    m_spriteRender5.Draw(rc);
    m_spriteRender2.Draw(rc);
    m_spriteRender3.Draw(rc);
    m_spriteRender4.Draw(rc);
    m_spriteRender6.Draw(rc);
}