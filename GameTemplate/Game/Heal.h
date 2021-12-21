#pragma once

class Player;

class Heal:public IGameObject
{
public:
	Heal();
	~Heal();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
public:
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_scale;
	Quaternion m_rotation;
	Player* m_player;
};
