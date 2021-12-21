#include "stdafx.h"
#include "TowerUi.h"
#include "Tower.h"

TowerUi::TowerUi()
{

}

TowerUi::~TowerUi()
{

}

bool TowerUi::Start()
{
	TowerIconRender.Init("Assets/sprite/UI/towericon.dds",1920.0f, 1080.0f);
	TowerIconRender.SetPosition(Vector3(-940.0f, 476.0f, 0.0f));
	TowerIconRender.SetScale(Vector3(0.6f,0.6f,1.0f));
	TowerIconRender.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.9f));

	TowerHpwaku.Init("Assets/sprite/UI/towerhpwaku.dds", 1920.0f, 1080.0f);
	TowerHpwaku.SetPosition(Vector3(130.0f, 477.0f, 0.0f));
	TowerHpwaku.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	TowerMidori.Init("Assets/sprite/UI/towerhpmidori.dds", 1920.0f, 1080.0f);
	TowerMidori.SetPosition(Vector3(-825.0f,-63.0f, 0.0f));
	TowerMidori.SetPivot(Vector2(0.0f, 0.0f));

	TowerHpButtom.Init("Assets/sprite/UI/towerhpbuttom.dds", 1920.0f, 1080.0f);
	TowerHpButtom.SetPosition(Vector3(136.0f, 478.0f, 0.0f));
	TowerHpButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 0.7f));

	m_tower = FindGO<Tower>("tower");
	return true;
}

void TowerUi::HpScale()
{
	towerhpscale = m_tower->m_hp / 200;
	if (towerhpscale < 0.0f)
	{
		TowerMidori.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	TowerMidori.SetScale(Vector3(towerhpscale, 1.0f, 1.0f));
}

void TowerUi::Update()
{
	HpScale();
	TowerHpButtom.Update();
	TowerIconRender.Update();
	TowerHpwaku.Update();
	TowerMidori.Update();
}

void TowerUi::Render(RenderContext& rc)
{
	TowerHpButtom.Draw(rc);
	TowerIconRender.Draw(rc);
	TowerMidori.Draw(rc);
	TowerHpwaku.Draw(rc);
}