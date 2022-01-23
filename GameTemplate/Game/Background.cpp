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
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/background/background2.tkm");
	//座標の設定
	m_modelRender.SetPosition(m_position);
	//大きさを設定する
	m_modelRender.SetScale(m_scale);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//モデルを更新する
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	return true;
}

void Background::Update()
{
	//モデルの更新
	m_modelRender.Update();
}

void Background::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}