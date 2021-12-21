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
	//�摜�̓ǂݍ���
	m_spriteRender.Init("Assets/sprite/tite/titleButtom.dds", 1920.0f, 1080.0f);

	//�Q�[���^�C�g���̃t�H���g
	m_titleFont.Init("Assets/sprite/tite/titleIG.dds", 1920.0f, 1080.0f);

	//�I����ʂ̔w�i�i���j
	m_blackButton.Init("Assets/sprite/tite/titleBlack.dds", 1920.0f, 1080.0f);
	m_blackButton.SetMulColor(Vector4(1.0f,1.0f,1.0f,0.7f));

	//�I����ʂ̘g�i���j
	m_whiteWaku.Init("Assets/sprite/tite/titleSirowaku.dds", 1920.0f, 1080.0f);

	//�Q�[���X�^�[�g�̃t�H���g
	m_gamestartFont.Init("Assets/sprite/tite/titleGS.dds", 1920.0f, 1080.0f);
	m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//�V�ѕ��̃t�H���g
	m_asobikataFont.Init("Assets/sprite/tite/titleA.dds", 1920.0f, 1080.0f);
	m_asobikataFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//������@�̃t�H���g
	m_sousahouhouFont.Init("Assets/sprite/tite/titleSH.dds", 1920.0f, 1080.0f);
	m_sousahouhouFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_player.Init("Assets/sprite/tite/titleIplayer.dds", 1920.0f, 1080.0f);

	//�t�F�[�h��T��
	m_fade = FindGO<Fade>("fade");
	//�t�F�[�h�C������
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
		//A�{�^������������B
		if (g_pad[0]->IsTrigger(enButtonA)) {
			m_isWaitFadeout = true;
			m_fade->StartFadeOut();
		}
	}
	//�摜�̍X�V
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
	//�摜��`�悷��
	m_spriteRender.Draw(rc);
	m_titleFont.Draw(rc);
	m_blackButton.Draw(rc);
	m_whiteWaku.Draw(rc);
	m_gamestartFont.Draw(rc);
	m_asobikataFont.Draw(rc);
	m_sousahouhouFont.Draw(rc);
	m_player.Draw(rc);
}