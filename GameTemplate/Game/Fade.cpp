#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;       //�摜�̉���
	const float LENGTH_SIZE = 1080.0f;     //�摜�̏c��
	const float FADE_SPEED = 0.8f;         //�t�F�[�h�̑���
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
	m_spriteRender.Init("Assets/sprite/fade.dds", SIDE_SIZE, LENGTH_SIZE);
	m_spriteRender.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
		//�t�F�[�h�C���X�e�[�g�̎�
	case enState_FadeIn:
		//�A���t�@�l������������@
		m_currentAlpha -= FADE_SPEED * g_gameTime->GetFrameDeltaTime();
		//�A���t�@�l��0.0f��菬����������
		if (m_currentAlpha <= 0.0f)
		{
			//�A���t�@�l��0.0f�ɂ���
			m_currentAlpha = 0.0f;
			//�ҋ@�X�e�[�g��
			m_state = enState_Idle;
		}
		break;
		//�t�F�[�h�A�E�g�X�e�[�g�̎�
	case enState_FadeOut:
		//�A���t�@�l�𑝉�������
		m_currentAlpha += FADE_SPEED * g_gameTime->GetFrameDeltaTime();
		//�A���t�@�l��1.0f���傫��������
		if (m_currentAlpha >= 1.0f)
		{
			//�A���t�@�l��1.0f�ɂ���
			m_currentAlpha = 1.0f;
			//�ҋ@�X�e�[�g��
			m_state = enState_Idle;
		}
		break;
		//�ҋ@�X�e�[�g�̎�
	case enState_Idle:
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	//�A���t�@�l��0.0f���傫��������
	if (m_currentAlpha > 0.0f)
	{
		//�t�F�[�h�̉摜�̃A���t�@�l��ݒ肷��
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		//�t�F�[�h�̉摜��`�悷��
		m_spriteRender.Draw(rc);
	}
}