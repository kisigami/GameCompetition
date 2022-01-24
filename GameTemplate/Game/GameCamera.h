#pragma once
//ばねカメラを使うため"camera/SpringCamera.h"をインクルードする
#include "camera/SpringCamera.h"

//クラス宣言
class Player;        //プレイヤークラス

/// <summary>
/// カメラ
/// </summary>
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	/// <summary>
	/// 座標と視点を更新する
	/// </summary>
	void UpdatePositionAndTarget();

private:
	Vector3       m_toCameraPos = Vector3::One;        //注視点から視点に向かうベクトル
	SpringCamera  m_springCamera;                      //スプリングカメラ
	Quaternion    m_rotation;                          //回転
	Player*       m_player = nullptr;                  //プレイヤー

};

