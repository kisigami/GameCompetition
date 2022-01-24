#include "stdafx.h"
#include "TowerUi.h"

#include "Tower.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;                      //画像の横幅
	const float LENGTH_SIZE = 1080.0f;                    //画像の立幅
	const float TOWER_ICON_POSITION_X = -380.0f;          //タワーアイコンのX座標
	const float TOWER_ICON_POSITION_Y = 212.0f;           //タワーアイコンのY座標
	const float TOWER_ICON_SCALE = 0.6f;                  //タワーアイコンの大きさ
	const float TOWER_DURABILITY_MAX = 200.0f;            //タワーの耐久値の最大値
	const float DURABILITY_POSITION_X = -825.0f;          //耐久値のX座標
	const float DURABILITY_POSITION_Y = -63.0f;           //耐久値のY座標
	const float DURABILITY_FRAME_POSITION_X = 130.0f;     //耐久値の枠のX座標
	const float DURABILITY_FRAME_POSITION_Y = 477.0f;     //耐久値の枠のY座標
	const float DURABILITY_BUTTOM_POSITION_X = 136.0f;    //耐久値の底のX座標
	const float DURABILITY_BUTTOM_POSITION_Y = 478.0f;    //耐久値の底のY座標
	const float DURABILITY_BUTTOM_ALPHA = 0.7f;           //耐久値の底のアルファ値
	const float DURABILITY_FONT_POSITION_X = -150.0f;     //耐久値の文字の画像のX座標
	const float DURABILITY_FONT_POSITION_Y = 65.0f;       //耐久値の文字の画像のY座標
	const float DURABILITY_FONT_SCALE = 0.9f;             //耐久値の文字の画像の大きさ
}

TowerUi::TowerUi()
{

}

TowerUi::~TowerUi()
{

}

bool TowerUi::Start()
{
	//タワーアイコンの画像の読み込み
	m_towerIcon.Init("Assets/sprite/UI/towericon2.dds", SIDE_SIZE, LENGTH_SIZE);
	m_towerIcon.SetPosition(Vector3(TOWER_ICON_POSITION_X, TOWER_ICON_POSITION_Y, 0.0f));
	m_towerIcon.SetScale(Vector3(TOWER_ICON_SCALE, TOWER_ICON_SCALE,1.0f));
	//耐久値の画像の読み込み
	m_durability.Init("Assets/sprite/UI/towerhpmidori.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durability.SetPosition(Vector3(DURABILITY_POSITION_X, DURABILITY_POSITION_Y, 0.0f));
	m_durability.SetPivot(Vector2(0.0f, 0.0f));
	//耐久値の枠の画像の読み込み
	m_durabilityFrame.Init("Assets/sprite/UI/towerhpwaku.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityFrame.SetPosition(Vector3(DURABILITY_FRAME_POSITION_X, DURABILITY_FRAME_POSITION_Y, 0.0f));
	m_durabilityFrame.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//耐久値の底の画像の読み込み
	m_durabilityButtom.Init("Assets/sprite/UI/towerhpbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityButtom.SetPosition(Vector3(DURABILITY_BUTTOM_POSITION_X, DURABILITY_BUTTOM_POSITION_Y, 0.0f));
	m_durabilityButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, DURABILITY_BUTTOM_ALPHA));
	//耐久値の文字の画像の読み込み
	m_durabilityFont.Init("Assets/sprite/UI/towerfont.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityFont.SetScale(Vector3(DURABILITY_FONT_SCALE, DURABILITY_FONT_SCALE, DURABILITY_FONT_SCALE));
	m_durabilityFont.SetPosition(Vector3(DURABILITY_FONT_POSITION_X, DURABILITY_FONT_POSITION_Y, 0.0f));
	m_durabilityFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_tower = FindGO<Tower>("tower");
	return true;
}

void TowerUi::DurabilityScale()
{
	//耐久値の画像の大きさ
	m_durabilityScale = m_tower->m_hp / TOWER_DURABILITY_MAX;
	//耐久値の画像の大きさが0.0fより小さかったら
	if (m_durabilityScale < 0.0f)
	{
		//耐久値の画像の大きさを0.0fにする
		m_durability.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	m_durability.SetScale(Vector3(m_durabilityScale, 1.0f, 1.0f));
}

void TowerUi::Update()
{
	/// <summary>
	/// 耐久値の画像の大きさの処理
	/// </summary>
	DurabilityScale();

	//画像の更新処理
	m_durabilityButtom.Update();
	m_towerIcon.Update();
	m_durabilityFrame.Update();
	m_durability.Update();
	m_durabilityFont.Update();
}

void TowerUi::Render(RenderContext& rc)
{
	//画像の描画処理
	m_durabilityButtom.Draw(rc);
	m_towerIcon.Draw(rc);
	m_durability.Draw(rc);
	m_durabilityFrame.Draw(rc);
	m_durabilityFont.Draw(rc);
}