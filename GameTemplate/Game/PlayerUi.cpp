#include "stdafx.h"
#include "PlayerUi.h"

#include "Player.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;                  //�摜�̉���
	const float LENGTH_SIZE = 1080.0f;                //�摜�̏c��
	const float PLAYER_ICON_POSITION_X = -388.0f;     //�v���C���[�A�C�R����X���W
	const float PLAYER_ICON_POSITION_Y = -188.0f;     //�v���C���[�A�C�R����Y���W
	const float PLAYER_ICON_SIZE = 0.6;               //�v���C���[�A�C�R���̑傫��
	const float PLAYER_MP_MAX = 40.0f;                //MP�̍ő�l
	const float MP_POSITION_X = -825.0f;              //MP��X���W
	const float MP_POSITION_Y = -988.0f;              //MP��Y���W
	const float MP_FRAME_POSITION_X = 130.0f;         //MP�̘g��X���W
	const float MP_FRAME_POSITION_Y = -448.0f;        //MP�̘g��Y���W
	const float MP_BUTTOM_POSITION_X = 137.0f;        //MP�̒��X���W
	const float MP_BUTTOM_POSITION_Y = -449.0f;       //MP�̒��Y���W
	const float MP_BUTTOM_ALPHA = 0.7f;               //MP�̒��Y���W
}

bool PlayerUi::Start()
{
	//�v���C���[�A�C�R���̉摜�̓ǂݍ���
	m_playerIcon.Init("Assets/sprite/UI/playericon2.dds", SIDE_SIZE, LENGTH_SIZE);
	m_playerIcon.SetPosition(Vector3(PLAYER_ICON_POSITION_X, PLAYER_ICON_POSITION_Y, 0.0f));
	m_playerIcon.SetScale(Vector3(PLAYER_ICON_SIZE, PLAYER_ICON_SIZE, 1.0f));
	//MP�̉摜�̓ǂݍ���
	m_mp.Init("Assets/sprite/UI/playermpmidori.dds", SIDE_SIZE, LENGTH_SIZE);
	m_mp.SetPosition(Vector3(MP_POSITION_X, MP_POSITION_Y, 0.0f));
	m_mp.SetPivot(Vector2(0.0f, 0.0f));
	//MP�̘g�̉摜�̓ǂݍ���
	m_mpFarame.Init("Assets/sprite/UI/playermpwaku.dds",SIDE_SIZE, LENGTH_SIZE);
	m_mpFarame.SetPosition(Vector3(MP_FRAME_POSITION_X, MP_FRAME_POSITION_Y, 0.0f));
	m_mpFarame.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_mpFarame.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//MP�̒�̉摜�̓ǂݍ���
	m_mpButtom.Init("Assets/sprite/UI/playerhpbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_mpButtom.SetPosition(Vector3(MP_BUTTOM_POSITION_X, MP_BUTTOM_POSITION_Y, 0.0f));
	m_mpButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, MP_BUTTOM_ALPHA));

	m_player = FindGO<Player>("player");
	return true;
}

void PlayerUi::Update()
{
	/// <summary>
	/// Mp�̉摜�̑傫���̏���
	/// </summary>
	MpScale();

	//�摜�̍X�V����
	m_playerIcon.Update();
	m_mpButtom.Update();
	m_mp.Update();
	m_mpFarame.Update();
}

void PlayerUi::MpScale()
{
	//MP�̑傫��
	m_mpSpriteScale = m_player->m_mp / PLAYER_MP_MAX;
	//MP�̑傫����0.0f��菬����������
	if (m_mpSpriteScale < 0.0f)
	{
		//MP�̑傫����0�ɂ���
		m_mp.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}

	m_mp.SetScale(Vector3(m_mpSpriteScale, 1.0f, 1.0f));
}

void PlayerUi::Render(RenderContext& rc)
{
	//�摜�̕`�揈��
	m_mpButtom.Draw(rc);
	m_playerIcon.Draw(rc);
	m_mp.Draw(rc);
	m_mpFarame.Draw(rc);
}