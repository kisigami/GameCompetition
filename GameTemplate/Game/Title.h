#pragma once

//�t�F�[�h�N���X
class Fade;
class Game;
class HowToPlay;
//�^�C�g��
class Title : public IGameObject
{
	enum EnTitleState
	{
		enTitleState_gamestart,
		enTitleState_Rule,
	};
public:
	Title();
	~Title();
	bool Start();
	void Update();
	void Render(RenderContext& rc);

	SpriteRender    m_titleButtom;           //�X�v�����g�����_�[
	SpriteRender    m_titleSword;
	SpriteRender    m_gamestartFont;
	SpriteRender    m_howtoplayFont;
	SpriteRender    m_blackButton;
	SpriteRender    m_choice;
	SpriteRender    m_titleFont;

	Vector3 m_position;
	Fade* m_fade = nullptr;		  //�t�F�[�h�B
	bool	        m_isWaitFadeout = false;  //�t���O?
	Game* m_game = nullptr;
	EnTitleState m_TitleState = enTitleState_gamestart;
	SoundSource* m_bgm = nullptr;
	SoundSource* m_se = nullptr;
	HowToPlay* m_howtoplay = nullptr;
	float m_alpha = 1.0f;
	float m_alpha2 = 1.0f;
	float m_alpha3 = 1.0f;
};
