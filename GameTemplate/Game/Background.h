#pragma once

//背景クラス
class Background:public IGameObject
{
public:
	Background();
	~Background();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	//座標を設定
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	//座標を取得
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	//大きさ
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//回転を設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	ModelRender          m_modelRender;                        //モデルレンダー
	Vector3              m_position;                           //座標
	Vector3              m_scale;                              //大きさ
	Quaternion           m_rotation;                           //回転
	PhysicsStaticObject	 m_physicsStaticObject;                //フィジクススタティックオブジェクト
};

