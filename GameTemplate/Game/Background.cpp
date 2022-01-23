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
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/background/background2.tkm");
	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	//���f�����X�V����
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}

void Background::Update()
{
	//���f���̍X�V
	m_modelRender.Update();
}

void Background::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}