#pragma once
class Player;
class IceBall;

class IceBall : public IGameObject
{
public:
	enum EnUser {
		enUser_None,
		enUser_Player,
		enUser_Enemy,
	};
public:
	IceBall();
	~IceBall();
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
	void SetEnUser(const EnUser enUser)
	{
		m_enUser = enUser;
	}

private:
	Vector3	m_position;
	Quaternion	m_rotation;
	Vector3	m_scale = Vector3::One;
	Vector3	m_moveSpeed;
	EffectEmitter* m_effectEmitter = nullptr;
	float	m_timer = 0.0f;
	CollisionObject* m_collisionObject;
	EnUser m_enUser = enUser_None;
	Player* m_player = nullptr;
};

