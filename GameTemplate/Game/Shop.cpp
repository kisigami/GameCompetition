#include "stdafx.h"
#include "Shop.h"

Shop::Shop()
{

}

Shop::~Shop()
{

}

bool Shop::Start()
{
	m_shopskincolor.Init("Assets/sprite/Shop/Shopskincolor.dds", 1920.0f, 1080.0f);
	m_shopfont.Init("Assets/sprite/Shop/ShopFont.dds", 1920.0f, 1080.0f);
	m_shopitembuttom.Init("Assets/sprite/Shop/ShopItemButtom.dds", 1920.0f, 1080.0f);
	m_shopbombshot.Init("Assets/sprite/Shop/ShopBombShot.dds", 1920.0f, 1080.0f);
	return true;
}

void Shop::Update()
{

}

void Shop::Render(RenderContext& rc)
{
	m_shopskincolor.Draw(rc);
	m_shopfont.Draw(rc);
	m_shopitembuttom.Draw(rc);
	m_shopbombshot.Draw(rc);
}
