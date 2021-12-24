#pragma once

class Enemy;
class Game;
class GameOver;


class Tower : public IGameObject
{
public:
	//エネミーステート
	enum EnTowerState {
		enTowerState_Idle,
		enTowerState_DamageInvalid,
	};
public:
	enum EnBreakState {
		enBreakState_NoBreak,
		enBreakState_HelfBreak,
		enBreakState_AllBreak,
	};
public:
	Tower();
	~Tower();
	bool Start();
	void Update();
	void DamageInvalid();
	void BreakEffect();
	void TowerEffect();
	void ProcessCommonStateTransition();
	void Render(RenderContext& rc);
	void Collision();

	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	float m_hp = 200.0f;


	CollisionObject m_collisionObject;
	SpriteRender m_spriteRender;
	FontRender m_fontRender;
	FontRender m_fontRender2;
	ModelRender m_modelRender;
	ModelRender m_modelRender2;
	ModelRender m_modelRender3;
	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_moveSpeed;
	Quaternion m_rotation;
	PhysicsStaticObject m_physicsStaticObject;
	CharacterController m_charaCon;
	Enemy* m_enemy = nullptr;
	Game* m_game = nullptr;
	GameOver* m_gameOver;
	bool TowerBreak = false;
	float damageinvalidtimer = 1.0f;
	EnTowerState m_towerState = enTowerState_Idle;
	EffectEmitter* effectEmitter;
	EnBreakState m_BreakState = enBreakState_NoBreak;
};

