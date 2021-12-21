#pragma once

#include "camera/SpringCamera.h"

class Player;
class Game;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void UpdatePositionAndTarget();


	Game* m_game = nullptr;
	Player* m_player = nullptr;
	Vector3 m_toCameraPos = Vector3::One;
	SpringCamera  m_springCamera;
	Quaternion    m_rotation;

};

