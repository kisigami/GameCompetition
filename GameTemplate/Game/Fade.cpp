#include "stdafx.h"
#include "Fade.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;       //画像の横幅
	const float LENGTH_SIZE = 1080.0f;     //画像の縦幅
	const float FADE_SPEED = 0.8f;         //フェードの速さ
}

Fade::Fade()
{

}

Fade::~Fade()
{

}

bool Fade::Start()
{
	//画像の読み込み
	m_spriteRender.Init("Assets/sprite/fade.dds", SIDE_SIZE, LENGTH_SIZE);
	m_spriteRender.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
		//フェードインステートの時
	case enState_FadeIn:
		//アルファ値を減少させる　
		m_currentAlpha -= FADE_SPEED * g_gameTime->GetFrameDeltaTime();
		//アルファ値が0.0fより小さかったら
		if (m_currentAlpha <= 0.0f)
		{
			//アルファ値を0.0fにする
			m_currentAlpha = 0.0f;
			//待機ステートへ
			m_state = enState_Idle;
		}
		break;
		//フェードアウトステートの時
	case enState_FadeOut:
		//アルファ値を増加させる
		m_currentAlpha += FADE_SPEED * g_gameTime->GetFrameDeltaTime();
		//アルファ値が1.0fより大きかったら
		if (m_currentAlpha >= 1.0f)
		{
			//アルファ値を1.0fにする
			m_currentAlpha = 1.0f;
			//待機ステートへ
			m_state = enState_Idle;
		}
		break;
		//待機ステートの時
	case enState_Idle:
		break;
	}
}

void Fade::Render(RenderContext& rc)
{
	//アルファ値が0.0fより大きかったら
	if (m_currentAlpha > 0.0f)
	{
		//フェードの画像のアルファ値を設定する
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		//フェードの画像を描画する
		m_spriteRender.Draw(rc);
	}
}