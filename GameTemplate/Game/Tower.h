#pragma once

//クラス宣言
class Game;         //ゲーム
class GameOver;     //ゲームオーバー

/// <summary>
/// タワー
/// </summary>
class Tower : public IGameObject
{
public:
	/// <summary>
	/// タワーステート
	/// </summary>
	enum EnTowerState {
		enTowerState_Idle,
		enTowerState_DamageInvalid,
	};
public:
	/// <summary>
	/// タワー破壊ステート
	/// </summary>
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
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>座標</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// 大きさを設定
	/// </summary>
	/// <param name="scale">大きさ</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	///耐久値を設定
	/// </summary>
	/// <param name="durability">耐久値</param>
	void SetDurability(const float& durability)
	{
		m_durability = durability;
	}
	/// <summary>
	/// 耐久値を取得
	/// </summary>
	/// <returns>耐久値</returns>
	const float& GetDurability() const
	{
		return m_durability;
	}
private:
	/// <summary>
	/// 敵の攻撃の当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// ダメージ無効処理
	/// </summary>
	void DamageInvalid();
	/// <summary>
	/// 破壊エフェクト
	/// </summary>
	void BreakEffect();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();

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
	Game* m_game = nullptr;
	GameOver* m_gameOver;
	bool m_towerBreakFrag = false;
	float damageinvalidtimer = 1.0f;
	EnTowerState m_towerState = enTowerState_Idle;
	EffectEmitter* effectEmitter;
	EnBreakState m_BreakState = enBreakState_NoBreak;
	float m_durability = 200.0f;
};

