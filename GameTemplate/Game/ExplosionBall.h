#pragma once

class Player;
class ExplosionBall;

class ExplosionBall:public IGameObject
{
public:
	ExplosionBall();
	~ExplosionBall();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
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
private:
	Vector3	m_position;
	Quaternion	m_rotation;
	Vector3	m_scale = Vector3::One;
	Vector3	m_moveSpeed;
	EffectEmitter* m_effectEmitter = nullptr;
	float	m_timer = 0.0f;
	CollisionObject* m_collisionObject;
	Player* m_player = nullptr;
};

