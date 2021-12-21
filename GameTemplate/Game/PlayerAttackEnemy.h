#pragma once

//クラス宣言
class Player;
class Game;

//エネミー
class PlayerAttackEnemy :public IGameObject
{
public:
	//エネミーステート
	enum EnPlayerAttackEnemyState {
		enPlayerAttackEnemyState_Idle,
		enPlayerAttackEnemyState_Assult,        //タワー突撃
		enPlayerAttackEnemyState_Attack,        //攻撃
		enPlayerAttackEnemyState_ReceiveDamage,
		enPlayerAttackEnemyState_ElectricShock,
		enPlayerAttackEnemyState_Down,
	};
public:
	PlayerAttackEnemy();
	~PlayerAttackEnemy();
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

	void UpdateInstansingData(
		const Vector3& pos,
		const Quaternion& rot, const Vector3& scale
	) {
		m_modelRender.UpdateInstancingData(pos, rot, scale);
	}
	void Electric();
	//塔への移動処理
	void Assult();
	//回転処理
	void Rotation();
	//攻撃処理
	void Attack();
	//プレイヤーとの当たり判定処理
	void Collision();
	//攻撃用の当たり判定を作成する
	void MakeAttackCollision();
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
	void ProcessElectricShockTransition();
	//ダウンステートの遷移処理
	void ProcessDownTransition();
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
		//攻撃時は移動しない
		return 
			m_enemyState != enPlayerAttackEnemyState_Attack &&
			m_enemyState != enPlayerAttackEnemyState_ReceiveDamage &&
			m_enemyState != enPlayerAttackEnemyState_Down &&
			m_enemyState != enAnimationClip_ElectricShock;
	}

	AnimationClip m_animationClips[enAnimationClip_Num];		//アニメーションクリップ
	ModelRender m_modelRender;
	FontRender m_fontRender;
	CharacterController	m_charaCon;									//キャラコン
	Vector3 m_position;
	Vector3 m_moveSpeed;
	Vector3	m_scale = Vector3::One;
	Vector3 m_forward = Vector3::AxisZ;
	Quaternion m_rotation;
	bool m_isUnderAttack = false;
	EnPlayerAttackEnemyState m_enemyState = enPlayerAttackEnemyState_Assult;
	Player* m_player = nullptr;
	int m_swordBoneId = -1;
	int    m_hp = 2;
	float electrictimer = 9.0f;
	int m_PlayerAttackEnemynum = 3;
	Game* m_game = nullptr;
};
