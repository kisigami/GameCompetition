#include "stdafx.h"
#include "GameOver.h"

#include "Game.h"
#include "Fade.h"
#include "Title.h"

namespace
{
    const float SIDE_SIZE = 1920.0f;        //�摜�̉���
    const float LENGTH_SIZE = 1080.0f;      //�摜�̏c��
    const float ALPHA_SPEED = 20.0f;        //�A���t�@�l1�̕ω����x
    const float ALPHA2_SPEED = 5.0f;        //�A���t�@�l2�̕ω����x
    const float ALPHA3_SPEED = 1.0f;        //�A���t�@�l3�̕ω����x
    const float BLACKSPRITE_ALPHA = 0.7f;   //�����摜�̃A���t�@�l
    const float CHOICE_POSITION = -119.0f;  //���̉摜�̍��W
}

GameOver::GameOver()
{

}

GameOver::~GameOver()
{
    //BGM���폜����
    DeleteGO(m_bgm);
}

bool GameOver::Start()
{
    //�摜�̓ǂݍ���
	m_overButtom.Init("Assets/sprite/overbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overBlack.Init("Assets/sprite/overblack.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
    m_titleFont.Init("Assets/sprite/title.dds", SIDE_SIZE, LENGTH_SIZE);
    m_titleFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
    m_continyuFont.Init("Assets/sprite/continyu.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
    m_overFont.Init("Assets/sprite/overfont.dds", SIDE_SIZE, LENGTH_SIZE);

    //SE��BGM��ǂݍ���
    g_soundEngine->ResistWaveFileBank(13, "Assets/sound/over.wav");
    g_soundEngine->ResistWaveFileBank(7, "Assets/sound/sentaku.wav");
    g_soundEngine->ResistWaveFileBank(10, "Assets/sound/kettei.wav");

    //BGM���Đ�����
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(25);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);

	m_fade = FindGO<Fade>("fade");
    //�t�F�[�h�C������
	m_fade->StartFadeIn();
	return true;
}

void GameOver::Update()
{
    /// <summary>
    /// �Q�[���I�[�o�[�̃t�F�[�h����
    /// </summary>
    GameOverFade();
    /// <summary>
    /// �A���t�@�l�̏���
    /// </summary>
    Alpha();
    /// <summary>
    /// �e�X�e�[�g�̑J�ڏ���
    /// </summary>
    ManageState();

    //�摜�̍X�V����
	m_overButtom.Update();
    m_overBlack.Update(); 
    m_overFont.Update();
    m_continyuFont.Update();
    m_titleFont.Update();
}

void GameOver::GameOverFade()
{
    //�t�F�[�h�t���O��true�ɂ���
    if (m_isWaitFadeout) {
        //�t�F�[�h���ł͂Ȃ�������
        if (!m_fade->IsFade()) {
            //�^�C�g���X�e�[�g��������
            if (m_overState == enOverState_Title) {
                //�^�C�g�����쐬����
                NewGO<Title>(0, "title");
                //���g���폜����
                DeleteGO(this);
            }
            //�R���e�j���[�X�e�[�g��������
            else if (m_overState == enOverState_Continue)
            {
                //�Q�[�����쐬����
                NewGO<Game>(0, "game");
                //���g���폜����
                DeleteGO(this);
            }
        }
    }

    else {
        //A�{�^���������ꂽ��
        if (g_pad[0]->IsTrigger(enButtonA)) {
            //SE���Đ�����
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(30);
            se->Play(false);
            se->SetVolume(0.2f);
            //�t�F�[�h�t���O��true��������
            m_isWaitFadeout = true;
            //�t�F�[�h�A�E�g����
            m_fade->StartFadeOut();
        }
    }
}

void GameOver::Alpha()
{
    //�t�F�[�h�t���O��true��������
    if (m_isWaitFadeout)
    {
        //�A���t�@�l��ω�������
        m_alpha += g_gameTime->GetFrameDeltaTime() * 20.0f;
    }

    //�R���e�j���[�X�e�[�g��������
    if (m_overState == enOverState_Continue)
    {
        //�R���e�j���[�̕����̉摜��_�ł�����
        m_continyuFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }
    //�^�C�g���X�e�[�g��������
    else if (m_overState == enOverState_Title)
    {
        //�^�C�g���̕����̉摜��_�ł�����
        m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));
    }

    //�A���t�@�l��ω�������
    m_alpha2 += g_gameTime->GetFrameDeltaTime() * 5.0f;
    m_alpha3 += g_gameTime->GetFrameDeltaTime() * 1.0f;

    //���̉摜��_�ł�����
    m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
    //�Q�[���I�[�o�[�̕�����_�ł�����
    m_overFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void GameOver::ManageState()
{
    //���{�^���������ꂽ��
    if (g_pad[0]->IsTrigger(enButtonDown))
    {
        //�R���e�j���[�X�e�[�g��������
        if (m_overState == enOverState_Continue)
        {
            //SE���Đ�����
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(29);
            se->Play(false);
            se->SetVolume(0.2f);
            //���̉摜�̍��W������������
            m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
            //�^�C�g���̕����̉摜�̐F�𔒂�����
            m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            //�R���e�j���[�̕����̉摜�̐F����������
            m_continyuFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            //�^�C�g���X�e�[�g��
            m_overState = enOverState_Title;
        }
    }
    //��{�^���������ꂽ��
    if (g_pad[0]->IsTrigger(enButtonUp))
    {
        //�^�C�g���X�e�[�g��������
        if (m_overState == enOverState_Title)
        {
            //SE���Đ�����
            SoundSource* se = NewGO<SoundSource>(0);
            se->Init(29);
            se->Play(false);
            se->SetVolume(0.2f);
            //���̉摜�̍��W��������ɂ���
            m_choice.SetPosition(Vector3(0.0f, 0.0f, 0.0f));
            //�^�C�g���̕����̉摜�̐F����������
            m_titleFont.SetMulColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
            //�R���e�j���[�̕����̉摜�̐F�𔒂�����
            m_continyuFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
            //�R���e�j���[�X�e�[�g��
            m_overState = enOverState_Continue;
        }
    }
}

void GameOver::Render(RenderContext& rc)
{
    //�摜�̕`�揈��
	m_overButtom.Draw(rc);
    m_overBlack.Draw(rc);
    m_titleFont.Draw(rc);
    m_continyuFont.Draw(rc);
    m_overFont.Draw(rc);
    m_choice.Draw(rc);
}