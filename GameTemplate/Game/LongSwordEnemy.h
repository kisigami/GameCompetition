#pragma once

class Player;
class Tower;
class IceBall;
class Game;

class LongSwordEnemy:public IGameObject
{
public:
	enum EnEnemyState {
		enEnemyState_Idle,
		enEnemyState_Assult,  
		enEnemyState_Chase,
		enEnemyState_Attack,        //攻撃
		enEnemyState_Attack2,
		enEnemyState_MagicAttack,
		enEnemyState_ReceiveDamage,
		enEnemyState_ElectricShock,
		enEnemyState_Down,
	};
public:
	LongSwordEnemy();
	~LongSwordEnemy();
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

	void Electric();
	void Assult();
	void Chase();
	void Attack();
	void MakeMagic();
	void MakeAttackCollision();
	void Collision();
	void Rotation();
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	void PlayAnimation();
	void ManageState();
	void ProcessIdleStateTransition();
	void ProcessAssultStateTransition();
	void ProcessAttackStateTransition();
	void ProcessMagicAttackStateTransition();
	void ProcessElectricShockStateTransition();
	void ProcessReceiveDamageTransition();
	void ProcessDownTransition();
	void ProcessCommonStateTransition();

	//プレイヤーを発見したか
	const bool SearchPlayer() const;
	//プレイヤーを攻撃できるか
	const bool IsCanPlayerAttack() const;
	//塔を攻撃できるか
	const bool IsCanTowerAttack() const;

	enum EnAnimationClip {    	//アニメーションクリップ
		enAnimationClip_Idle,  //待機アニメーション
		enAnimationClip_Assult, //突撃アニメーション
		enAnimationClip_Attack, //攻撃アニメーション
		enAnimationClip_Attack2,
		enAnimationClip_MagicAttack,
		enAnimationClip_ElectricShock,
		enAnimationClip_ReceiveDamage,
		enAnimationClip_Down,
		enAnimationClip_Num,	//アニメーションの数
	};

	bool IsEnableMove() const
	{
		return
			m_enemyState != enEnemyState_Attack &&
			m_enemyState != enEnemyState_Chase &&
			m_enemyState != enEnemyState_Attack2 &&
			m_enemyState != enEnemyState_MagicAttack&&
			m_enemyState != enEnemyState_ElectricShock &&
			m_enemyState != enEnemyState_ReceiveDamage &&
			m_enemyState != enEnemyState_Down;
	}
	FontRender m_fontRender3;
	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	CharacterController	m_charaCon;
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	EnEnemyState m_enemyState = enEnemyState_Assult;
	bool m_isUnderAttack = false;
	Tower* m_tower = nullptr;
	Player* m_player = nullptr;
	Game* m_game = nullptr;
	int m_swordBoneId = -1;
	int    m_hp = 6;
	int ram = 0;
	float electrictimer = 5.0f;
	EffectEmitter m_effectemitter;
	int m_LongSwordEnemynum = 1;
};

