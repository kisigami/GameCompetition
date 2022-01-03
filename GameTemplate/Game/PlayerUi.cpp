#include "stdafx.h"
#include "PlayerUi.h"
#include "Player.h"

bool PlayerUi::Start()
{
	PlayerIcon.Init("Assets/sprite/UI/playericon2.dds", 1920.0f, 1080.0f);
	PlayerIcon.SetPosition(Vector3(-388.0f, -188.0f, 0.0f));
	PlayerIcon.SetScale(Vector3(0.6f, 0.6f, 1.0f));

	PlayerMpWaku.Init("Assets/sprite/UI/playermpwaku.dds", 1920.0f, 1080.0f);
	PlayerMpWaku.SetPosition(Vector3(130.0f, -448.0f, 0.0f));
	PlayerMpWaku.SetScale(Vector3(1.0f, 1.0f, 1.0f));
	PlayerMpWaku.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	PlayerMpMidori.Init("Assets/sprite/UI/playermpmidori.dds", 1920.0f, 1080.0f);
	PlayerMpMidori.SetPosition(Vector3(-825.0f, -988.0f, 0.0f));
	PlayerMpMidori.SetPivot(Vector2(0.0f, 0.0f));

	PlayerMpButtom.Init("Assets/sprite/UI/playerhpbuttom.dds", 1920.0f, 1080.0f);
	PlayerMpButtom.SetPosition(Vector3(137.0f, -449.0f, 0.0f));
	PlayerMpButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));

	PlayerHpButtom.Init("Assets/sprite/UI/mpfont.dds", 1920.0f, 1080.0f);
	PlayerHpButtom.SetPosition(Vector3(-65.0f, -10.0f, 0.0f));
	PlayerHpButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_player = FindGO<Player>("player");
	return true;
}

void PlayerUi::Update()
{
	HpScale();
	PlayerHpButtom.Update();
	PlayerMpButtom.Update();
	PlayerIcon.Update();
	PlayerHpMidori.Update();
	PlayerHpWaku.Update();
	PlayerMpMidori.Update();
	PlayerMpWaku.Update();
}

void PlayerUi::HpScale()
{
	PlayerHpScale = m_player->m_hp / 100;
	if (PlayerHpScale < 0.0f)
	{
		PlayerHpMidori.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	PlayerHpMidori.SetScale(Vector3(PlayerHpScale, 1.0f, 1.0f));
	
	PlayerMpScale = m_player->m_mp / 40;
	if (PlayerMpScale < 0.0f)
	{
		PlayerMpMidori.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	PlayerMpMidori.SetScale(Vector3(PlayerMpScale, 1.0f, 1.0f));

}

void PlayerUi::Render(RenderContext& rc)
{
	PlayerHpButtom.Draw(rc);
	PlayerMpButtom.Draw(rc);
	PlayerIcon.Draw(rc);
	PlayerMpMidori.Draw(rc);
	PlayerMpWaku.Draw(rc);
}