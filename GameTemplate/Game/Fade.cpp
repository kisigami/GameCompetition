#include "stdafx.h"
#include "Fade.h"

namespace
{
	//画像の大きさ
	const Vector3 SCALE = Vector3(1.436f, 1.436f, 1.0f);
	//画像の位置
	const Vector3 POSITION = Vector3(-250.0f, 90, 0.0f);
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
	m_spriteRender.Init("Assets/sprite/fade.dds", 1920.0f, 1080.0f);
	//画像の大きさ
	m_spriteRender.SetScale(SCALE);
	//画像の位置
	m_spriteRender.SetPosition(POSITION);
	//画像の更新
	m_spriteRender.Update();
	return true;
}

void Fade::Update()
{
	switch (m_state) {
		//フェードイン
	case enState_FadeIn:
		m_currentAlpha -= 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha <= 0.0f) {
			m_currentAlpha = 0.0f;
			m_state = enState_Idle;
		}
		break;
		//フェードアウト
	case enState_FadeOut:
		m_currentAlpha += 1.0f * g_gameTime->GetFrameDeltaTime();
		if (m_currentAlpha >= 1.0f) {
			m_currentAlpha = 1.0f;
			m_state = enState_Idle;
		}
		break;
		//待機
	case enState_Idle:

		break;
	}

}

void Fade::Render(RenderContext& rc)
{

	if (m_currentAlpha > 0.0f)
	{
		m_spriteRender.SetMulColor({ 1.0f,1.0f,1.0f,m_currentAlpha });
		m_spriteRender.Draw(rc);
	}

}