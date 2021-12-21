#include "stdafx.h"
#include "Fade.h"

namespace
{
	//�摜�̑傫��
	const Vector3 SCALE = Vector3(1.436f, 1.436f, 1.0f);
	//�摜�̈ʒu
	const Vector3 POSITION = Vector3(-250.0f, 90, 0.0f);
}

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
	//�摜�̑傫��
	m_spriteRender.SetScale(SCALE);
	//�摜�̈ʒu
	m_spriteRender.SetPosition(POSITION);
	//�摜�̍X�V
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