#include "stdafx.h"
#include "Background.h"

Background::Background()
{

}
Background::~Background()
{

}

bool Background::Start()
{
	m_modelRender.Init("Assets/modelData/background/background2.tkm");
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//���f�����X�V����
	//m_modelRender.Update();PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}

void Background::Update()
{
	m_modelRender.Update();
}

void Background::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}