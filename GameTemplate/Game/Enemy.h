#pragma once

//クラス宣言
class Player;
class Tower;
class Game;
class GamePoint;

//エネミー
class Enemy:public IGameObject
{
public:
	//エネミーステート
	enum EnEnemyState {
		enEnemyState_Idle,
		enEnemyState_Assult,        //タワー突撃
		enEnemyState_Attack,        //攻撃
		enEnemyState_ReceiveDamage,
		enEnemyState_ElectricShock,
		enEnemyState_Down,
	};
public:
	Enemy();
	~Enemy();
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
	//HPを設定
	void SetHP(const int hp)
	{
		m_hp = hp;
	}

	//塔への移動処理
	void Assult();
	void Electric();
	//回転処理
	void Rotation();
	//攻撃処理
	void Attack();
	//プレイヤーとの当たり判定処理
	void Collision();
	//攻撃用の当たり判定を作成する
	void MakeAttackCollision();
	void MakeDamageEffect();
	//アニメーションイベント用の関数
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//アニメーションの再生
	void PlayAnimation();
	//各ステートの遷移処理
	void ManageState();
	//共通のステート遷移処理
	void ProcessCommonStateTransition();
	//待機ステートの遷移処理
	void ProcessIdleStateTransition();
	//突撃ステートの遷移処理
	void ProcessAssultStateTransition();
	//攻撃ステートの遷移処理
	void ProcessAttackStateTransition();
	//被ダメージステートの遷移処理
	void ProcessReceiveDamageTransition();
	//ダウンステートの遷移処理
	void ProcessDownTransition();

	void ProcessElectricShockTransition();

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
		enAnimationClip_ReceiveDamage,
		enAnimationClip_ElectricShock,
		enAnimationClip_Down,
		enAnimationClip_Num,	//アニメーションの数
	};
	//移動できるか
	bool IsEnableMove() const
	{
		return
			m_enemyState != enEnemyState_Attack &&
			m_enemyState != enEnemyState_ReceiveDamage &&
			m_enemyState != enEnemyState_ElectricShock &&
			m_enemyState != enEnemyState_Down;
	}

	AnimationClip m_animationClips[enAnimationClip_Num];
	ModelRender m_modelRender;
	FontRender m_timerFont;
	CharacterController	m_charaCon;							
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	bool m_isUnderAttack = false;
	EnEnemyState m_enemyState = enEnemyState_Assult;
	Tower* m_tower = nullptr;
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	int m_swordBoneId = -1;
	int    m_hp =3;
	int ram; 
	float electrictimer = 9.0f;
	EffectEmitter* effectEmitter;
	int m_enemynum = 6;
	Game* m_game = nullptr;
};

