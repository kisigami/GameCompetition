#include "stdafx.h"
#include "GameClear.h"

#include "Fade.h"
#include "Title.h"

GameClear::GameClear()
{

}

GameClear::~GameClear()
{

}

bool GameClear::Start()
{
	m_spriteRender.Init("Assets/sprite/clear.dds", 1960.0f, 1080.0f);
	m_fade = FindGO<Fade>("fade");
	m_fade->StartFadeIn();
	return true;
}

void GameClear::Update()
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

void GameClear::Render(RenderContext& rc)
{
	m_spriteRender.Draw(rc);
}