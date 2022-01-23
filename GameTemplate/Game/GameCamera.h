#pragma once
//ばねカメラを使用したい場合は、springcameraをインクルードする
#include "camera/SpringCamera.h"

class Player;        //プレイヤークラス
//ゲームカメラクラス
class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	//座標と視点を更新する
	void UpdatePositionAndTarget();
private:
	Vector3       m_toCameraPos = Vector3::One;        //注視点から視点に向かうベクトル
	SpringCamera  m_springCamera;                      //スプリングカメラ
	Quaternion    m_rotation;                          //回転
	Player*       m_player = nullptr;                  //プレイヤー

};

