#pragma once
class GamePoint :public IGameObject
{
public:
	GamePoint();
	~GamePoint();
	bool Start();
	void Update();
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}


	Vector3	m_position;
	Quaternion	m_rotation;
	Vector3	m_scale = Vector3::One;
	Vector3	m_moveSpeed;
	EffectEmitter* m_effectEmitter = nullptr;
	float	m_timer = 0.0f;
	CollisionObject* m_collisionObject;
};

