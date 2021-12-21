#include "stdafx.h"
#include "Heal.h"
#include "Player.h"

Heal::Heal()
{

}

Heal::~Heal()
{

}

bool Heal::Start()
{
	//m_modelRender.Init("Assets/modelData/item/heal.tkm");
	//m_modelRender.SetPosition(m_position);
	//m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	//m_modelRender.Update();
	//m_player = FindGO<Player>("player");
	return true;
}

void Heal::Update()
{
	//Vector3 diff = m_position - m_player->GetPosition();
	//if (diff.LengthSq() <= 200.0f * 200.0f)
	//{
	//	m_player->Healnum += 1;
	//	DeleteGO(this);
		
	//}

	//m_modelRender.Update();
}

void Heal::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}
