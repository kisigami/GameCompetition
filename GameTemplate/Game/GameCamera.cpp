#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

GameCamera::GameCamera()
{

}
GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定
	m_toCameraPos.Set(0.0f, 180.0f, -470.0f);

	//ばねカメラの初期化
	m_springCamera.Init(
		*g_camera3D,       //ばねカメラの処理を行うカメラを指定する
		2000.0f,           //カメラの移動速度の最大値
		true,              //カメラに設定される球体コリジョン
		5.0f               //カメラと地形との当たり判定を取るかどうかのフラグ
	);

	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	return true;
}
void GameCamera::UpdatePositionAndTarget()
{
	//カメラを更新
	//注視点を計算する
	Vector3 target = m_player->GetPosition();
	//プレイヤーの少し上を注視点とする
	target.y += 320.0f;
	target += g_camera3D->GetForward() * 60.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸回りの回
	m_rotation.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	m_rotation.Apply(m_toCameraPos);
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();

	//X軸回りの回
	m_rotation.SetRotationDeg(axisX, 1.5f * y);
	m_rotation.Apply(m_toCameraPos);
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	if (toPosDir.y < -0.5f)
	{
		//カメラが上向きすぎ
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.6f)
	{
		//カメラが下向きすぎ
		m_toCameraPos = toCameraPosOld;
	}

	//視点を計算する
	Vector3 pos = target + m_toCameraPos;

	//ばねカメラに注視点と視点を設定する
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::Update()
{
	/// <summary>
	/// 視点と注視点を更新
	/// </summary>
	UpdatePositionAndTarget();
	//カメラの更新
	m_springCamera.Update();
}
