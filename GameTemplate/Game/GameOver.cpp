#include "stdafx.h"
#include "GameOver.h"
#include "Fade.h"
#include "Title.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

bool GameOver::Start()
{
	m_spriteRender.Init("Assets/sprite/over.dds", 1960.0f, 1080.0f);
	m_title = FindGO<Title>("title");
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void GameOver::Update()
{
    if (m_isWaitFadeout) {
        if (!m_fade->IsFade()) {
            NewGO<Title>(0, "title");
            DeleteGO(this);
        }
    }

    else {
        if (g_pad[0]->IsTrigger(enButtonA)) {
            m_isWaitFadeout = true;
            m_fade->StartFadeOut();
        }
    }

	m_spriteRender.Update();
}

void GameOver::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}