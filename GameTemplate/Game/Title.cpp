#include "stdafx.h"
#include "Title.h"

#include "Game.h"
#include "HowToPlay.h"
#include "Fade.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;        //�摜�̉���
	const float LENGTH_SIZE = 1080.0f;      //�摜�̏c��
	const float ALPHA_SPEED = 20.0f;        //�A���t�@�l1�̕ω����x
	const float ALPHA2_SPEED = 5.0f;        //�A���t�@�l2�̕ω����x
	const float ALPHA3_SPEED = 1.0f;        //�A���t�@�l3�̕ω����x
	const float BLACKSPRITE_ALPHA = 0.7f;   //�����摜�̃A���t�@�l
	const float CHOICE_POSITION = -144.0f;  //���̉摜�̍��W
}

Title::Title()
{

}

Title::~Title()
{
	//BGM���폜����
	DeleteGO(m_bgm);
}

bool Title::Start()
{
	//�摜�̓ǂݍ���
	m_titleButtom.Init("Assets/sprite/tite/titleButtom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_gamestartFont.Init("Assets/sprite/tite/titleGS.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleFont.Init("Assets/sprite/tite/titlefont.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleBlack.Init("Assets/sprite/tite/titleBlack.dds", SIDE_SIZE, LENGTH_SIZE);
	m_titleBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
	m_howtoplayFont.Init("Assets/sprite/tite/titleA.dds", SIDE_SIZE, LENGTH_SIZE);
	m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	m_titleSword.Init("Assets/sprite/tite/titlesword.dds", SIDE_SIZE, LENGTH_SIZE);
	m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
	
	//SE��BGM��ǂݍ���
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/kettei.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/title2.wav");

	//BGM���Đ�����
	m_bgm = NewGO<SoundSource>(0);
	m_bgm->Init(23);
	m_bgm->Play(true);
	m_bgm->SetVolume(0.2f);

	//�t�F�[�h��T��
	m_fade = FindGO<Fade>("fade");
	m_howtoplay = FindGO<HowToPlay>("howtoplay");
	//�t�F�[�h�C������
	m_fade->StartFadeIn();
	return true;
}

void Title::Update()
{
	/// <summary>
	/// �t�F�[�h����
	/// </summary>
	TitleFade();
	/// <summary>
	/// �A���t�@�l�̏���
	/// </summary>
	Alpha();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	ManageState();

	//�摜�̍X�V
	m_titleButtom.Update();
	m_titleBlack.Update();
	m_gamestartFont.Update();
	m_howtoplayFont.Update();
	m_choice.Update();
	m_titleFont.Update();
}

void Title::TitleFade()
{
	//�t�F�[�h�t�����Otrue��������
	if (m_isWaitFadeout) {
		//�t�F�[�h���ł͂Ȃ�������
		if (!m_fade->IsFade())
		{
			//�Q�[���X�^�[�g�X�e�[�g��������
			if (m_titleState == enTitleState_Gamestart)
			{
				//�Q�[�����쐬����
				NewGO<Game>(0, "game");
				//���g���폜����
				DeleteGO(this);
				return;
			}
			//�V�ѕ��X�e�[�g��������
			else if (m_titleState == enTitleState_Howtoplay)
			{
				//�V�ѕ���ʂ��쐬����
				m_howtoplay = NewGO<HowToPlay>(0, "howtoplay");
				//���g���폜����
				DeleteGO(this);
				return;
			}
		}
	}
	else {
		//A�{�^������������B
		if (g_pad[0]->IsTrigger(enButtonA)) {
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(30);
			se->Play(false);
			se->SetVolume(0.2f);
			//�t�F�[�h�t���O��true�ɂ���
			m_isWaitFadeout = true;
			//�t�F�[�h�A�E�g����
			m_fade->StartFadeOut();
		}
	}
}

void Title::Alpha()
{
	//�t�F�[�h�t���O��true��������
	if (m_isWaitFadeout)
	{
		//�A���t�@�l��ω�������
		m_alpha += g_gameTime->GetFrameDeltaTime() * ALPHA_SPEED;
	}
	//�A���t�@�l��ω�������
	m_alpha2 += g_gameTime->GetFrameDeltaTime() * ALPHA2_SPEED;
	m_alpha3 += g_gameTime->GetFrameDeltaTime() * ALPHA3_SPEED;


	//�Q�[���X�^�[�g�X�e�[�g��������
	if (m_titleState == enTitleState_Gamestart)
	{
		//�Q�[���X�^�[�g�̕�����_�ł�����
		m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}
	//�V�ѕ��X�e�[�g��������
	else if (m_titleState == enTitleState_Howtoplay)
	{
		//�V�ѕ��̕�����_�ł�����
		m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
	}

	//����_�ł�����
	m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
	//�^�C�g���̕�����_�ł�����
	m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void Title::ManageState()
{
	//���{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonDown))
	{
		//�Q�[���X�^�[�g�X�e�[�g��������
		if (m_titleState == enTitleState_Gamestart)
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->Play(false);
			se->SetVolume(0.2f);
			//�V�ѕ��X�e�[�g��
			m_titleState = enTitleState_Howtoplay;
			//�Q�[���X�^�[�g�̕����̐F��ω�������
			m_gamestartFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			//�V�ѕ��̕����̐F��ω�������
			m_howtoplayFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//���̍��W���������ɂ���
			m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
		}
	}
	//��{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonUp))
	{
		//�V�ѕ��X�e�[�g��������
		if (m_titleState == enTitleState_Howtoplay)
		{
			//SE���Đ�����
			SoundSource* se = NewGO<SoundSource>(0);
			se->Init(29);
			se->Play(false);
			se->SetVolume(0.2f);
			//�Q�[���X�^�[�g�X�e�[�g��
			m_titleState = enTitleState_Gamestart;
			//�V�ѕ��̕����̐F��ω�������
			m_howtoplayFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
			//�Q�[���X�^�[�g�̕����̐F��ω�������
			m_gamestartFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			//���̍��W��ݒ�
			m_choice.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Title::Render(RenderContext& rc)
{
	//�摜��`�悷��
	m_titleButtom.Draw(rc);
	m_titleBlack.Draw(rc);
	m_gamestartFont.Draw(rc);
	m_howtoplayFont.Draw(rc);
	m_titleSword.Draw(rc);
	m_choice.Draw(rc);
	m_titleFont.Draw(rc);
}