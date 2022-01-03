#include "stdafx.h"
#include "GameClear.h"

#include "Fade.h"
#include "Title.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
    DeleteGO(m_bgm);
}

bool GameClear::Start()
{
    m_spriteRender.Init("Assets/sprite/over/over.dds", 1920.0f, 1080.0f);

    m_spriteRender2.Init("Assets/sprite/over/overblack.dds", 1920.0f, 1080.0f);
    m_spriteRender2.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));
   
    m_spriteRender3.Init("Assets/sprite/over/titlefont.dds", 1920.0f, 1080.0f);
   
    m_spriteRender4.Init("Assets/sprite/over/overfont.dds", 1920.0f, 1080.0f);
   
    m_yajirusi.Init("Assets/sprite/yajirusi.dds", 1920.0f, 1080.0f);
    m_yajirusi.SetPosition(Vector3(0.0f, -65.0f, 0.0f));
    g_soundEngine->ResistWaveFileBank(24, "Assets/sound/clear2.wav");
    g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(24);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);

    m_fade = FindGO<Fade>("fade");
    m_fade->StartFadeIn();
	return true;
}
                                            
void GameClear::Update()
{
    if (m_isWaitFadeout)
    {
        m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
    }
    m_spriteRender3.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

    m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
    m_yajirusi.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));

    m_alpha3 += g_gameTime->GetFrameDeltaTime() * 1.0f;
    m_spriteRender4.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));

    if (m_isWaitFadeout) {
        if (!m_fade->IsFade()) {
            NewGO<Title>(0, "title");
            DeleteGO(this);
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
	m_spriteRender.Update();
    m_spriteRender2.Update();
    m_spriteRender3.Update();
    m_yajirusi.Update();
    m_spriteRender4.Update();
}

void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
    m_spriteRender2.Draw(rc);
    m_spriteRender3.Draw(rc);
    m_yajirusi.Draw(rc);
    m_spriteRender4.Draw(rc);
}