#pragma once

//クラス宣言
class Player;          //プレイヤークラス
class Tower;           //タワークラス
class Game;            //ゲームクラス

/// <summary>
/// エネミー
/// </summary>
class Enemy:public IGameObject
{
public:
	/// <summary>
	/// エネミーステート
	/// </summary>
	enum EnEnemyState {
		enEnemyState_Idle,              //待機ステート
		enEnemyState_Assault,           //突撃ステート
		enEnemyState_Attack,            //攻撃ステート
		enEnemyState_ReceiveDamage,     //被ダメージステート
		enEnemyState_ReceiveRestraint,  //拘束ステート
		enEnemyState_Down,              //ダウンステート
	};
public:
	Enemy();
	~Enemy();
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
	/// HPを設定
	/// </summary>
	/// <param name="hp">HP</param>
	void SetHP(const int hp)
	{
		m_hp = hp;
	}

private:
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// プレイヤーの攻撃との当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃用の当たり判定を作成する
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 拘束処理
	/// </summary>
	void Restraint();
	/// <summary>
	/// 被ダメージエフェクト
	/// </summary>
	void MakeDamageEffect();
	/// <summary>
	/// アニメーションイベント用の関数
	/// </summary>
	/// <param name="clipName">アニメーションの名前</param>
	/// <param name="eventName">アニメーションイベントのキーの名前</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 突撃ステートの遷移処理
	/// </summary>
	void ProcessAssultStateTransition();
	/// <summary>
	/// 攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理
	/// </summary>
	void ProcessReceiveDamageStateTransition();
	/// <summary>
	/// ダウンステートの遷移処理
	/// </summary>
	void ProcessDownStateTransition();
	/// <summary>
	/// 拘束ステートの遷移処理
	/// </summary>
	void ProcessReceiveRestraintStateTransition();

	/// <summary>
	/// プレイヤーを攻撃できるか
	/// </summary>
	/// <returns>攻撃できるならtrue</returns>
	const bool IsCanPlayerAttack() const;
	/// <summary>
	/// 塔を攻撃できるか
	/// </summary>
	/// <returns>攻撃できるならtrue</returns>
	const bool IsCanTowerAttack() const;
	/// <summary>
	///アニメーションクリップ
	/// </summary>
	enum EnAnimationClip {    	           //アニメーションクリップ
		enAnimationClip_Idle,              //待機アニメーション
		enAnimationClip_Assult,            //突撃アニメーション
		enAnimationClip_Attack,            //攻撃アニメーション
		enAnimationClip_ReceiveDamage,     //被ダメージアニメーション
		enAnimationClip_ReceiveRestraint,  //拘束アニメーション
		enAnimationClip_Down,              //ダウンアニメーション
		enAnimationClip_Num,	           //アニメーションの数
	};
	AnimationClip         m_animationClips[enAnimationClip_Num];    //アニメーションクリップ 
	ModelRender           m_modelRender;                            //モデルレンダー
	CharacterController	  m_charaCon;						        //キャラクターコントローラー
	Vector3               m_position;                               //座標
	Vector3               m_moveSpeed;                              //移動速度
	Vector3	              m_scale = Vector3::One;                   //大きさ
	Vector3               m_forward = Vector3::AxisZ;               //エネミーの正面ベクトル
	Quaternion            m_rotation;                               //回転
	bool                  m_isUnderAttack = false;                  //攻撃中か？
	EnEnemyState          m_enemyState = enEnemyState_Assault;      //エネミーステート
	int                   m_swordBoneId = -1;                       //ソードボーンID
	int                   m_hp =3;                                  //HP
	float                 m_restraintTimer = 8.0f;                  //拘束タイマー
	Game*                 m_game = nullptr;                         //ゲーム
	Tower*                m_tower = nullptr;                        //タワー
	Player*               m_player = nullptr;                       //プレイヤー
};

