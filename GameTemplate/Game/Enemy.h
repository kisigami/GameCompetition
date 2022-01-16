#pragma once

//クラス宣言
class Player;          //プレイヤークラス
class Tower;           //タワークラス
class Game;            //ゲームクラス

//エネミー
class Enemy:public IGameObject
{
public:
	//エネミーステート
	enum EnEnemyState {
		enEnemyState_Idle,              //待機中
		enEnemyState_Assault,           //突撃
		enEnemyState_Attack,            //攻撃
		enEnemyState_ReceiveDamage,     //被ダメージ
		enEnemyState_ReceiveRestraint,  //拘束
		enEnemyState_Down,              //ダウン
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

	//突撃処理
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
	//塔を攻撃できるか
	const bool IsCanTowerAttack() const;

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

