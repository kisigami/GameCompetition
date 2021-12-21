#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "Fade.h"

Title::Title()
{

}

Title::~Title()
{

}

bool Title::Start()
{
	//画像の読み込み
	m_spriteRender.Init("Assets/sprite/tite/titleButtom.dds", 1920.0f, 1080.0f);

	//ゲームタイトルのフォント
	m_titleFont.Init("Assets/sprite/tite/titleIG.dds", 1920.0f, 1080.0f);

	//選択画面の背景（黒）
	m_blackButton.Init("Assets/sprite/tite/titleBlack.dds", 1920.0f, 1080.0f);
	m_blackButton.SetMulColor(Vector4(1.0f,1.0f,1.0f,0.7f));

	//選択画面の枠（白）
	m_whiteWaku.Init("Assets/sprite/tite/titleSirowaku.dds", 1920.0f, 1080.0f);

	//ゲームスタートのフォント
	m_gamestartFont.Init("Assets/sprite/tite/titleGS.dds", 1920.0f, 1080.0f);
	m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//遊び方のフォント
	m_asobikataFont.Init("Assets/sprite/tite/titleA.dds", 1920.0f, 1080.0f);
	m_asobikataFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//操作方法のフォント
	m_sousahouhouFont.Init("Assets/sprite/tite/titleSH.dds", 1920.0f, 1080.0f);
	m_sousahouhouFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_player.Init("Assets/sprite/tite/titleIplayer.dds", 1920.0f, 1080.0f);

	//フェードを探す
	m_fade = FindGO<Fade>("fade");
	//フェードインする
	m_fade->StartFadeIn();

	return true;
}

void Title::Update()
{
	ProcessCommonStateTransition();
	ManageState();


		if (m_isWaitFadeout) {
			if (!m_fade->IsFade()) {
				NewGO<Game>(0, "game");
				DeleteGO(this);
			}
		}
	else {
		//Aボタンを押したら。
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
	//画像の更新
	m_spriteRender.Update();
	m_blackButton.Update();
	m_whiteWaku.Update();
	m_gamestartFont.Update();
	m_asobikataFont.Update();
	m_sousahouhouFont.Update();
}

void Title::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		switch (m_TitleState)
		{
		case Title::enTitleState_gamestart:
			m_TitleState = enTitleState_Rule;
			break;
		case Title::enTitleState_Rule:
			m_TitleState = enTitleState_OperationExplanation;
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
			m_TitleState = enTitleState_gamestart;
			break;
		case Title::enTitleState_OperationExplanation:
			m_TitleState = enTitleState_Rule;
			break;
		default:
			break;
		}
	}
}

void Title::ManageState()
{
	switch (m_TitleState)
	{
	case Title::enTitleState_gamestart:
		m_sousahouhouFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_asobikataFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case Title::enTitleState_Rule:
		m_gamestartFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_sousahouhouFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_asobikataFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	case Title::enTitleState_OperationExplanation:
		m_gamestartFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_asobikataFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		m_sousahouhouFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		break;
	default:
		break;
	}
}

void Title::Render(RenderContext& rc)
{
	//画像を描画する
	m_spriteRender.Draw(rc);
	m_titleFont.Draw(rc);
	m_blackButton.Draw(rc);
	m_whiteWaku.Draw(rc);
	m_gamestartFont.Draw(rc);
	m_asobikataFont.Draw(rc);
	m_sousahouhouFont.Draw(rc);
	m_player.Draw(rc);
}