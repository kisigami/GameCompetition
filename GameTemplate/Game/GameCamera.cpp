#include "stdafx.h"
#include "Game.h"
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
	m_player = FindGO<Player>("player");
	m_toCameraPos.Set(0.0f, 180.0f, -470.0f);
	m_springCamera.Init(
		*g_camera3D,
		2000.0f,
		true,
		5.0f
	);
	return true;
}
void GameCamera::UpdatePositionAndTarget()
{
	Vector3 target = m_player->GetPosition();
	target.y += 320.0f;
	target += g_camera3D->GetForward() * 60.0f;
	Vector3 toCameraPosOld = m_toCameraPos;
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	m_rotation.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	m_rotation.Apply(m_toCameraPos);
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	m_rotation.SetRotationDeg(axisX, 1.5f * y);
	m_rotation.Apply(m_toCameraPos);
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.6f)
	{
		m_toCameraPos = toCameraPosOld;
	}
	Vector3 pos = target + m_toCameraPos;
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);


}

void GameCamera::Update()
{
	UpdatePositionAndTarget();
	m_springCamera.Update();
}
