#pragma once

/// <summary>
/// �t�F�[�h
/// </summary>
class Fade : public IGameObject
{
public:
	/// <summary>
	/// �t�F�[�h�X�e�[�g
	/// </summary>
	enum EnState
	{
		enState_FadeIn,   //�t�F�[�h�C���X�e�[�g
		enState_FadeOut,  //�t�F�[�h�A�E�g�X�e�[�g
		enState_Idle,     //�ҋ@�X�e�[�g
	};
public:
	Fade();
	~Fade();
	bool Start() override;
	void Update() override;
	void Render(RenderContext& rc) override;
	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	void StartFadeIn()
	{
		m_state = enState_FadeIn;
	}
	/// <summary>
	/// �t�F�[�h�A�E�g
	/// </summary>
	void StartFadeOut()
	{
		m_state = enState_FadeOut;
	}
	/// <summary>
	/// �t�F�[�h�����H
	/// </summary>
	/// <returns>�t�F�[�h���Ȃ�true</returns>
	const bool IsFade() const
	{
		return m_state != enState_Idle;
	}
	/// <summary>
	/// �A���t�@�l���擾
	/// </summary>
	/// <returns>�A���t�@�l</returns>
	const float GetCurrentAlpha() const
	{
		return m_currentAlpha;
	}

private:
	SpriteRender    m_spriteRender;             //�t�F�[�h�̉摜
	EnState         m_state = enState_Idle;     //�t�F�[�h�X�e�[�g
	float           m_currentAlpha = 1.0f;      //�A���t�@�l
};

