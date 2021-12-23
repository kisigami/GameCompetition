#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//�摜�̓ǂݍ���
	m_spriteRender.Init("Assets/sprite/fade.dds", 1920.0f, 1080.0f);

	m_spriteRender.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
		//�t�F�[�h�C��
	case enState_FadeIn:
		m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
		//�t�F�[�h�A�E�g
	case enState_FadeOut:
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
		//�ҋ@
	case enState_Idle:

		break;
	}

}

void Fade::Render(RenderContext& rc)
{

	if (m_currentAlpha > 0.0f)
	{
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_spriteRender.Draw(rc);
	}

}