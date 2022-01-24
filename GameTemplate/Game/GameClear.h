#pragma once

//�N���X�錾
class Fade;   //�t�F�[�h�N���X
class Title;  //�^�C�g���N���X

/// <summary>
/// �Q�[���N���A
/// </summary>
class GameClear :public IGameObject
{
public:
	GameClear();
	~GameClear();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// �Q�[���N���A�̃t�F�[�h����
	/// </summary>
	void GameClearFade();
	/// <summary>
	/// �A���t�@�l�̏���
	/// </summary>
	void Alpha();

	SoundSource* m_bgm = nullptr;                //BGM
	SpriteRender m_clearButtom;                  //��ԉ��̉摜
	SpriteRender m_clearBlack;                   //�����摜
	SpriteRender m_titleFont;                    //�^�C�g���̕����̉摜
	SpriteRender m_clearFont;                    //�Q�[���N���A�̕����̉摜
	SpriteRender m_choice;                       //���̉摜
	bool         m_isWaitFadeout = false;        //�t�F�[�h�t���O
	float        m_alpha = 1.0f;                 //�A���t�@�l1
	float        m_alpha2 = 1.0f;                //�A���t�@�l2
	float        m_alpha3 = 1.0f;                //�A���t�@�l3
	Title*       m_title = nullptr;              //�^�C�g��
	Fade*        m_fade = nullptr;               //�t�F�[�h
};

