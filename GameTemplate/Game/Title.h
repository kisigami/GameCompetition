#pragma once

//�N���X�錾
class Fade;            //�t�F�[�h�N���X
class Game;            //�Q�[���N���X
class HowToPlay;       //�V�ѕ�

/// <summary>
/// �^�C�g��
/// </summary>
class Title : public IGameObject
{
public:
	//�^�C�g���X�e�[�g
	enum EnTitleState
	{
		enTitleState_Gamestart,    //�Q�[���X�^�[�g�X�e�[�g
		enTitleState_Howtoplay,    //�V�ѕ��X�e�[�g
	};
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	/// <summary>
	/// �^�C�g���̃t�F�[�h����
	/// </summary>
	void TitleFade();
	/// <summary>
	/// �A���t�@�l�̐ݒ�
	/// </summary>
	void Alpha();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();

	SoundSource*    m_bgm = nullptr;                        //BGM
	SpriteRender    m_titleButtom;                          //�^�C�g���̈�ԉ��̉摜
	SpriteRender    m_titleSword;                           //���̉摜
	SpriteRender    m_gamestartFont;                        //�Q�[���X�^�[�g�̕����̉摜
	SpriteRender    m_howtoplayFont;                        //�V�ѕ��̕����̉摜
	SpriteRender    m_titleBlack;                           //�����摜
	SpriteRender    m_choice;                               //���̉摜
	SpriteRender    m_titleFont;                            //�^�C�g���̕����̉摜
	bool	        m_isWaitFadeout = false;                //�t�F�[�h�t���O
	EnTitleState    m_titleState = enTitleState_Gamestart;  //�^�C�g���X�e�[�g
	float           m_alpha = 1.0f;                         //�A���t�@�l1
	float           m_alpha2 = 1.0f;                        //�A���t�@�l2
	float           m_alpha3 = 1.0f;                        //�A���t�@�l3
	Fade*           m_fade = nullptr;		                //�t�F�[�h
	Game*           m_game = nullptr;                       //�Q�[��
	HowToPlay*      m_howtoplay = nullptr;                  //�V�ѕ�
};
