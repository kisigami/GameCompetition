#include "stdafx.h"
#include "PlayerUi.h"

#include "Player.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;                  //画像の横幅
	const float LENGTH_SIZE = 1080.0f;                //画像の縦幅
	const float PLAYER_ICON_POSITION_X = -388.0f;     //プレイヤーアイコンのX座標
	const float PLAYER_ICON_POSITION_Y = -188.0f;     //プレイヤーアイコンのY座標
	const float PLAYER_ICON_SIZE = 0.6;               //プレイヤーアイコンの大きさ
	const float PLAYER_MP_MAX = 40.0f;                //MPの最大値
	const float MP_POSITION_X = -825.0f;              //MPのX座標
	const float MP_POSITION_Y = -988.0f;              //MPのY座標
	const float MP_FRAME_POSITION_X = 130.0f;         //MPの枠のX座標
	const float MP_FRAME_POSITION_Y = -448.0f;        //MPの枠のY座標
	const float MP_BUTTOM_POSITION_X = 137.0f;        //MPの底のX座標
	const float MP_BUTTOM_POSITION_Y = -449.0f;       //MPの底のY座標
	const float MP_BUTTOM_ALPHA = 0.7f;               //MPの底のY座標
}

bool PlayerUi::Start()
{
	//プレイヤーアイコンの画像の読み込み
	m_playerIcon.Init("Assets/sprite/UI/playericon2.dds", SIDE_SIZE, LENGTH_SIZE);
	m_playerIcon.SetPosition(Vector3(PLAYER_ICON_POSITION_X, PLAYER_ICON_POSITION_Y, 0.0f));
	m_playerIcon.SetScale(Vector3(PLAYER_ICON_SIZE, PLAYER_ICON_SIZE, 1.0f));
	//MPの画像の読み込み
	m_mp.Init("Assets/sprite/UI/playermpmidori.dds", SIDE_SIZE, LENGTH_SIZE);
	m_mp.SetPosition(Vector3(MP_POSITION_X, MP_POSITION_Y, 0.0f));
	m_mp.SetPivot(Vector2(0.0f, 0.0f));
	//MPの枠の画像の読み込み
	m_mpFarame.Init("Assets/sprite/UI/playermpwaku.dds",SIDE_SIZE, LENGTH_SIZE);
	m_mpFarame.SetPosition(Vector3(MP_FRAME_POSITION_X, MP_FRAME_POSITION_Y, 0.0f));
	m_mpFarame.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_mpFarame.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//MPの底の画像の読み込み
	m_mpButtom.Init("Assets/sprite/UI/playerhpbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_mpButtom.SetPosition(Vector3(MP_BUTTOM_POSITION_X, MP_BUTTOM_POSITION_Y, 0.0f));
	m_mpButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, MP_BUTTOM_ALPHA));
	//スキルの枠の画像の読み込
	m_skillFrame.Init("Assets/sprite/skill/skillframe.dds", SIDE_SIZE, LENGTH_SIZE);
	//拘束魔法の画像の読み込み
	m_restraint.Init("Assets/sprite/skill/restraint.dds", SIDE_SIZE, LENGTH_SIZE);
	//タワー修理の画像の読み込み
	m_repair.Init("Assets/sprite/skill/repair.dds", SIDE_SIZE, LENGTH_SIZE);

	m_player = FindGO<Player>("player");
	return true;
}

void PlayerUi::Update()
{
	/// <summary>
	/// Mpの画像の大きさの処理
	/// </summary>
	MpScale();
	/// <summary>
	/// スキルの文字の処理
	/// </summary>
	SkillFont();
	//画像の更新処理
	m_playerIcon.Update();
	m_mpButtom.Update();
	m_mp.Update();
	m_mpFarame.Update();
	m_skillFrame.Update();
	m_restraint.Update();
	m_repair.Update();
}

void PlayerUi::MpScale()
{
	//MPの画像の大きさ
	m_mpSpriteScale = m_player->GetMp() / PLAYER_MP_MAX;
	//MPの画像の大きさが0.0fより小さかったら
	if (m_mpSpriteScale <= 0.0f)
	{
		//MPの画像の大きさを0.0fにする
		m_mp.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	//MPの画像の大きさを設定
	m_mp.SetScale(Vector3(m_mpSpriteScale, 1.0f, 1.0f));
}

void PlayerUi::SkillFont()
{
	wchar_t text[256];
	//プレイヤーがスキル（拘束魔法）を選択していたら
	if (m_player->SelectRestraint())
	{
		//拘束魔法の残り使用回数
		swprintf_s(text, 256, L"%d", m_player->GetRestraintNum());
	}
	//プレイヤーがスキル（タワー修理）を選択していたら
	else if (m_player->SelectRepair())
	{
		//タワー修理の残り使用回数
		swprintf_s(text, 256, L"%d", m_player->GetRepairNum());
	}
	m_skillNumFont.SetText(text);
	m_skillNumFont.SetPosition(Vector3(808.0f, -430.0f, 0.0f));
	m_skillNumFont.SetScale(1.0f);
}

void PlayerUi::Render(RenderContext& rc)
{
	//文字の描画処理
	m_skillNumFont.Draw(rc);
	//画像の描画処理
	m_mpButtom.Draw(rc);
	m_playerIcon.Draw(rc);
	m_mp.Draw(rc);
	m_mpFarame.Draw(rc);
	m_skillFrame.Draw(rc);
	//プレイヤーが拘束魔法を選択していたら
	if (m_player->SelectRestraint())
	{
		//拘束魔法の画像を描画する
		m_restraint.Draw(rc);
	}
	//プレイヤーがタワー修理を選択していたら
	else if (m_player->SelectRepair())
	{
		//タワー修理の画像を描画する
		m_repair.Draw(rc);
	}
}