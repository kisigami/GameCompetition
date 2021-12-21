#include "stdafx.h"
#include "Stuck.h"
#include "Player.h"

Stuck::Stuck()
{

}

Stuck::~Stuck()
{

}

bool Stuck::Start()
{
	//m_modelRender.Init("Assets/modelData/item/item.tkm");
	//m_modelRender.SetPosition(m_position);
	///m_modelRender.SetRotation(m_rotation);
	//m_modelRender.SetScale(m_scale);
	//m_modelRender.Update();
	//m_player = FindGO<Player>("player");
	return true;
}

void Stuck::Update()
{
	//Vector3 diff = m_position - m_player->m_position;
	//if (diff.LengthSq() <= 200.0f * 200.0f)
	//{
	//	m_player->Thundernum += 1;
	//	DeleteGO(this);
	//}

	//m_modelRender.Update();
}

void Stuck::Render(RenderContext& rc)
{
	//m_modelRender.Draw(rc);
}
