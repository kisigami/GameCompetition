#pragma once

class Player;
class Tower;
class IceBall;
class Game;

class TwoSwordEnemy:public IGameObject
{
public:
	enum EnEnemyState {
		enEnemyState_Idle,
		enEnemyState_Assult,
		enEnemyState_QuickAttack,
		enEnemyState_HeavyAttack,
		enEnemyState_ReceiveDamage,
		enEnemyState_ElectricShock,
		enEnemyState_Down,
	};
public:
	TwoSwordEnemy();
	~TwoSwordEnemy();
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
	//大きさを設定
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	//回転を設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	void ElectricShock();
	void Assult();
	void Attack();
	void MakeQuickAttackCollision();
	void MakeHeavyAttackCollision();
	void Collision();
	void Rotation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void PlayAnimation();
	void ManageState();
	void ProcessCommonStateTransition();
	void ProcessIdleStateTransition();
	void ProcessAssultStateTransition();
	void ProcessAttackStateTransition();
	void ProcessElectricShockStateTransition();
	void ProcessReceiveDamageStateTransition();
	void ProcessDownTransition();

	//プレイヤーを発見したか

	const bool IsCanPlayerAttack() const;
	const bool IsCanTowerAttack() const;
	const bool IsCanQuickAttack() const;

	enum EnAnimationClip { 
		enAnimationClip_Idle,
		enAnimationClip_Assult,
		enAnimationClip_QuickAttack,
		enAnimationClip_HeavyAttack,
		enAnimationClip_ReceiveDamage,
		enAnimationClip_ElectricShock,
		enAnimationClip_Down,
		enAnimationClip_Num,
	};

	bool IsEnableMove() const
	{
		return
			m_enemyState != enEnemyState_QuickAttack &&
			m_enemyState != enEnemyState_HeavyAttack &&
			m_enemyState != enEnemyState_ReceiveDamage &&
			m_enemyState != enAnimationClip_ElectricShock &&
			m_enemyState != enEnemyState_Down;
	}


	AnimationClip m_animationClips[enAnimationClip_Num];
	CharacterController	m_charaCon;
	Vector3 m_moveSpeed;
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	EnEnemyState m_enemyState = enEnemyState_Assult;
	bool m_isUnderAttack = false;
	Tower* m_tower = nullptr;
	Player* m_player = nullptr;
	int m_swordBoneId = -1;
	int m_swordBoneId2 = -2;
	int    m_hp = 6;
	int ram;
	float electricshocktimer = 5.0f;
	EffectEmitter m_effectemitter;
	int TwoSwordEnemynum = 1;
	Game* m_game = nullptr;
};

