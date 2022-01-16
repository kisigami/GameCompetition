#pragma once

//クラス宣言
class Player;       //プレイヤークラス
class Game;         //ゲームクラス

//プレイヤーアタックエネミー
class PlayerAttackEnemy:public IGameObject
{
public:
	//エネミーステート
	enum EnPlayerAttackEnemyState {
		enPlayerAttackEnemyState_Idle,               //待機中
		enPlayerAttackEnemyState_Assault,            //突撃
		enPlayerAttackEnemyState_Attack,             //攻撃
		enPlayerAttackEnemyState_ReceiveDamage,      //被ダメージ
		enPlayerAttackEnemyState_ReceiveRestraint,   //拘束
		enPlayerAttackEnemyState_Down,               //ダウン
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

	//移動処理
	void Move();
	//回転処理
	void Rotation();
	//攻撃処理
	void Attack();
	//プレイヤーとの当たり判定処理
	void Collision();
	//攻撃用の当たり判定を作成する
	void MakeAttackCollision();
	//拘束処理
	void Restraint();
	//被ダメージエフェクト
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
	void ProcessReceiveDamageStateTransition();
	//ダウンステートの遷移処理
	void ProcessDownStateTransition();
	//拘束ステートの遷移処理
	void ProcessReceiveRestraintStateTransition();

	//プレイヤーを攻撃できるか
	const bool IsCanPlayerAttack() const;

	enum EnAnimationClip {    	              //アニメーションクリップ
		enAnimationClip_Idle,                 //待機アニメーション
		enAnimationClip_Assault,              //突撃アニメーション
		enAnimationClip_Attack,               //攻撃アニメーション
		enAnimationClip_ReceiveDamage,        //被ダメージアニメーション
		enAnimationClip_ReceiveRestraint,     //拘束アニメーション
		enAnimationClip_Down,                 //ダウンアニメーション
		enAnimationClip_Num,	              //アニメーションの数
	};

	AnimationClip                  m_animationClips[enAnimationClip_Num];		                 //アニメーションクリップ
	ModelRender                    m_modelRender;                                                //モデルレンダー
	CharacterController	           m_charaCon;								                     //キャラコン
	Vector3                        m_position;                                                   //座標
	Vector3                        m_moveSpeed;                                                  //移動速度
	Vector3                        m_scale = Vector3::One;                                       //大きさ
	Vector3                        m_forward = Vector3::AxisZ;                                   //プレイヤーアタックエネミーの正面ベクトル
	Quaternion                     m_rotation;                                                   //回転
	bool                           m_isUnderAttack = false;                                      //攻撃中か？
	EnPlayerAttackEnemyState       m_playerattackenemyState = enPlayerAttackEnemyState_Assault;  //プレイヤーアタックエネミーステート
	int                            m_swordBoneId = -1;                                           //ソードボーンID
	int                            m_hp = 2;                                                     //HP
	float                          electrictimer = 8.0f;                                         //拘束タイマー
	Player*                        m_player = nullptr;                                           //プレイヤー
	Game*                          m_game = nullptr;                                             //ゲーム
};
