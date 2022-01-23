#pragma once

class BattleStartFade:public IGameObject
{
public:
	enum EnState
	{
		enState_FadeIn,     //�t�F�[�h�C��
		enState_FadeOut,    //�t�F�[�h�A�E�g
		enState_Idle,       //�ҋ@
	};
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//�t�F�[�h�C��
	void StartFadeIn()
	{
		//�t�F�[�h�C���X�e�[�g��
		m_state = enState_FadeIn;
	}
	//�t�F�[�h�A�E�g
	void StartFadeOut()
	{
		//�t�F�[�h�A�E�g�X�e�[�g��
		m_state = enState_FadeOut;
	}
private:
	SpriteRender m_battlefade;             //�X�v���C�g�����_�[
	Vector3      m_position;               //���W
	EnState      m_state = enState_Idle;   //�t�F�[�h�X�e�[�g
	float        m_currentAlpha = 0.0f;    //�A���t�@�l
};

