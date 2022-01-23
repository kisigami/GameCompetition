#include "stdafx.h"
#include "BattleStartFade.h"

bool BattleStartFade::Start()
{
	//�摜�̓ǂݍ���
	m_battlefade.Init("Assets/sprite/battlefade.dds", 1920.0f, 1080.0f);
	return true;
}

void BattleStartFade::Update()
{
	switch (m_state)
	{
		//�t�F�[�h�C���X�e�[�g�̎�
	case BattleStartFade::enState_FadeIn:
		//�A���t�@�l������������
		m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		//�A���t�@�l��0.0f��菬����������
		if (m_currentAlpha <= 0.0f) {
			//�A���t�@�l��0.0f�ɂ���
			m_currentAlpha = 0.0f;
			//�ҋ@�X�e�[�g��
			m_state = enState_Idle;
		}
		break;
		//�t�F�[�h�A�E�g�X�e�[�g�̎�
	case BattleStartFade::enState_FadeOut:
		//�A���t�@�l�𑝉�������
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		//�A���t�@�l��1.0f���傫��������
		if (m_currentAlpha >= 1.0f) {
			//�A���t�@�l��1.0f�ɂ���
			m_currentAlpha = 1.0f;
			//�ҋ@�X�e�[�g��
			m_state = enState_Idle;
		}
		break;
		//�ҋ@�X�e�[�g�̎�
	case BattleStartFade::enState_Idle:
		//�������Ȃ�
		break;
	}
}

void BattleStartFade::Render(RenderContext& rc)
{
	//�A���t�@�l��0.0f���傫��������
	if (m_currentAlpha > 0.0f)
	{
		//�t�F�[�h�摜��`�悷��
		m_battlefade.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_battlefade.Draw(rc);
	}
}