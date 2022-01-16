#pragma once

class Player;           //プレイヤークラス
class Tower;            //タワークラス
class IceBall;          //魔法攻撃クラス
class Game;             //ゲームクラス

class MagicEnemy: public IGameObject
{
public:
	//エネミーステート
	enum EnMagicEnemyState {
		enMagicEnemyState_Idle,              //待機中
		enMagicEnemyState_Assault,            //突撃
		enMagicEnemyState_Attack,            //攻撃
		enMagicEnemyState_ReceiveDamage,     //被ダメージ
		enMagicEnemyState_ReceiveRestraint,  //拘束
		enMagicEnemyState_Down,              //ダウン
	};
public:
	MagicEnemy();
	~MagicEnemy();
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
	void MakeMagicBall();
	//プレイヤーとの当たり判定処理
	void Collision();
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

	enum EnAnimationClip {                  //アニメーションクリップ
		enAnimationClip_Idle,               //待機アニメーション
		enAnimationClip_Assault,             //突撃アニメーション
		enAnimationClip_Attack,             //攻撃アニメーション
		enAnimationClip_ReceiveDamage,      //被ダメージアニメーション
		enAnimationClip_ReceiveRestraint,   //拘束アニメーション
		enAnimationClip_Down,               //ダウンアニメーション
		enAnimationClip_Num,                //アニメーションの数
	};
	
	AnimationClip         m_animationClips[enAnimationClip_Num];	 	  //アニメーションクリップ
	ModelRender           m_modelRender;                                  //モデルレンダー
	CharacterController	  m_charaCon;								 	  //キャラコン
	Vector3               m_position;                                     //座標
	Vector3               m_moveSpeed;                                    //移動速度
	Vector3	              m_scale = Vector3::One;                         //大きさ
	Vector3               m_forward = Vector3::AxisZ;                     //マジックエネミーの正面ベクトル
	Quaternion            m_rotation;                                     //回転
	EnMagicEnemyState     m_magicenemyState = enMagicEnemyState_Assault;  //マジックエネミーステート
	int                   m_hp = 2;                                       //HP
	float                 m_restraintTimer = 8.0f;                        //拘束タイマー
	Tower*                m_tower = nullptr;                              //タワー
	Player*               m_player = nullptr;                             //プレイヤー
	Game*                 m_game = nullptr;                               //ゲーム
};

