#pragma once

//�N���X�錾
class Fade;     //�t�F�[�h�N���X
class Title;    //�^�C�g���N���X
class Game;     //�Q�[���N���X

/// <summary>
/// �Q�[���I�[�o�[
/// </summary>
class GameOver : public IGameObject
{
public:
	/// <summary>
	/// �Q�[���I�[�o�[�X�e�[�g
	/// </summary>
	enum EnOverState
	{
		enOverState_Continue,      //�R���e�j���[�X�e�[�g
		enOverState_Title,         //�^�C�g���X�e�[�g
	};
public:
	GameOver();
	~GameOver();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �Q�[���N���A�̃t�F�[�h����
	/// </summary>
	void GameOverFade();
	/// <summary>
	/// �A���t�@�l�̏���
	/// </summary>
	void Alpha();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();

	EnOverState      m_overState = enOverState_Continue;  //�Q�[���I�[�o�[�X�e�[�g
	SoundSource*     m_bgm = nullptr;                     //BGM
	SpriteRender     m_overButtom;                        //��ԉ��̉摜
	SpriteRender     m_overBlack;                         //�����摜     
	SpriteRender     m_overFont;                          //�Q�[���I�[�o�[�̕����̉摜
	SpriteRender     m_continyuFont;                      //�R���e�j���[�̕����̉摜
	SpriteRender     m_titleFont;                         //�^�C�g���̕����̉摜
	SpriteRender     m_choice;                            //���̉摜
	Title*           m_title = nullptr;                   //�^�C�g��
	Fade*            m_fade = nullptr;                    //�t�F�[�h
	Game*            m_game = nullptr;                    //�Q�[��
	bool             m_isWaitFadeout = false;             //�t�F�[�h�t���O
	float            m_alpha = 1.0f;                      //�A���t�@�l1
	float            m_alpha3 = 1.0f;                     //�A���t�@�l2
	float            m_alpha2 = 1.0f;                     //�A���t�@�l3
};

