#include "stdafx.h"
#include "BattleStartFade.h"

bool BattleStartFade::Start()
{
	//画像の読み込み
	m_battlefade.Init("Assets/sprite/battlefade.dds", 1920.0f, 1080.0f);
	return true;
}

void BattleStartFade::Update()
{
	switch (m_state)
	{
		//フェードインステートの時
	case BattleStartFade::enState_FadeIn:
		//アルファ値を減少させる
		m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		//アルファ値が0.0fより小さかったら
		if (m_currentAlpha <= 0.0f) {
			//アルファ値を0.0fにする
			m_currentAlpha = 0.0f;
			//待機ステートへ
			m_state = enState_Idle;
		}
		break;
		//フェードアウトステートの時
	case BattleStartFade::enState_FadeOut:
		//アルファ値を増加させる
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		//アルファ値が1.0fより大きかったら
		if (m_currentAlpha >= 1.0f) {
			//アルファ値を1.0fにする
			m_currentAlpha = 1.0f;
			//待機ステートへ
			m_state = enState_Idle;
		}
		break;
		//待機ステートの時
	case BattleStartFade::enState_Idle:
		//何もしない
		break;
	}
}

void BattleStartFade::Render(RenderContext& rc)
{
	//アルファ値が0.0fより大きかったら
	if (m_currentAlpha > 0.0f)
	{
		//フェード画像を描画する
		m_battlefade.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_battlefade.Draw(rc);
	}
}