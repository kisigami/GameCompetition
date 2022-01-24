#include "stdafx.h"
#include "GameClear.h"

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
    const float CHOICE_POSITION = -65.0f;   //���̉摜�̍��W
}

GameClear::GameClear()
{

}

GameClear::~GameClear()
{
    //BGM���폜����
    DeleteGO(m_bgm);
}

bool GameClear::Start()
{
    //�摜��ǂݍ���
    m_clearButtom.Init("Assets/sprite/clear/clearbuttom.dds",SIDE_SIZE,LENGTH_SIZE);
    m_clearBlack.Init("Assets/sprite/clear/clearblack.dds", SIDE_SIZE, LENGTH_SIZE);
    m_clearBlack.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, BLACKSPRITE_ALPHA));
    m_titleFont.Init("Assets/sprite/clear/titlefont.dds", SIDE_SIZE, LENGTH_SIZE);
    m_clearFont.Init("Assets/sprite/clear/clearfont.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.Init("Assets/sprite/choice.dds", SIDE_SIZE, LENGTH_SIZE);
    m_choice.SetPosition(Vector3(0.0f, CHOICE_POSITION, 0.0f));
   
    //SE��BGM��ǂݍ���
    g_soundEngine->ResistWaveFileBank(24, "Assets/sound/clear2.wav");
    g_soundEngine->ResistWaveFileBank(30, "Assets/sound/kettei.wav");
   
    //BGM���Đ�����
    m_bgm = NewGO<SoundSource>(0);
    m_bgm->Init(24);
    m_bgm->Play(true);
    m_bgm->SetVolume(0.2f);

    m_fade = FindGO<Fade>("fade");
    //�t�F�[�h�C��
    m_fade->StartFadeIn();
	return true;
}
                                            
void GameClear::Update()
{
    /// <summary>
    /// �Q�[���N���A�̃t�F�[�h����
    /// </summary>
    GameClearFade();
    /// <summary>
    /// �A���t�@�l�̏���
    /// </summary>
    Alpha();

    //�摜�̍X�V����
	m_clearButtom.Update();
    m_clearBlack.Update();
    m_clearFont.Update();
    m_choice.Update();
    m_titleFont.Update();
}

void GameClear::GameClearFade()
{
    //�t�F�[�h�t���O��true��������
    if (m_isWaitFadeout) {
        //�t�F�[�h���ł͂Ȃ�������
        if (!m_fade->IsFade()) {
            //�^�C�g�����쐬����
            NewGO<Title>(0, "title");
            //���g���폜����
            DeleteGO(this);
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
            //�t�F�[�h�t���O��true�ɂ���
            m_isWaitFadeout = true;
            //�t�F�[�h�A�E�g
            m_fade->StartFadeOut();
        }
    }
}

void GameClear::Alpha()
{
    //�t�F�[�h�t���O��true��������
    if (m_isWaitFadeout)
    {
        //�A���t�@�l��ω�������
        m_alpha += g_gameTime->GetFrameDeltaTime() * ALPHA_SPEED;
    }

    //�^�C�g���̕����̉摜��_�ł�����
    m_titleFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha))));

    //�A���t�@�l��ω�������
    m_alpha2 += g_gameTime->GetFrameDeltaTime() * ALPHA2_SPEED;
    m_alpha3 += g_gameTime->GetFrameDeltaTime() * ALPHA3_SPEED;

    //���̉摜��_�ł�����
    m_choice.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha2))));
    //�Q�[���N���A�̕����̉摜��_�ł�����
    m_clearFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, fabsf(sinf(m_alpha3))));
}

void GameClear::Render(RenderContext& rc)
{
    //�摜�̕`�揈��
	m_clearFont.Draw(rc);
    m_clearBlack.Draw(rc);
    m_titleFont.Draw(rc);
    m_choice.Draw(rc);
    m_clearFont.Draw(rc);
}