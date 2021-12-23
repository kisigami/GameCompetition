#pragma once

//クラス宣言
class Game;
class Enemy;
class IceBall;
class ExplosionBall;
class Tower;

//プレイヤー
class Player : public IGameObject
{
public:
	//プレイヤーのステート
	enum EnPlayerState {
		enPlayerState_Idle,
		enPlayerState_Run,
		enPlayerState_UseItem,
		enPlayerState_FirstAttack,
		enPlayerState_SecondAttack,
		enPlayerState_ThirdAttack,
		enPlayerState_Cheak,
		enPlayerState_Magic,
		enPlayerState_ReceiveDamage,
		enPlayerState_Win,
		enPlayerState_Down,
	};
	//アイテムのステート
	enum EnEquipState {
		enEquipState_No,
		enEquipState_Heal,
		enEquipState_Thuner,
	};
	//魔法攻撃ステート
	enum EnUseMagic {
		enUseMagic_NormalMagic,
		enUseMagic_PowerMagic
	};
public:
	Player();
	~Player();
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
	
	//大きさ
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	
	//回転を設定
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//動ける状態かどうか
	bool IsEnableMove() const
	{
		return
			m_playerState != enPlayerState_FirstAttack &&
			m_playerState != enPlayerState_SecondAttack &&
			m_playerState != enPlayerState_ThirdAttack &&
			m_playerState != enPlayerState_UseItem &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Win &&
			m_playerState != enPlayerState_Magic;
	}
//private:/ 	
	void  Hit();
	//移動処理
	void Move();
	//回転処理
	void Rotation();
	//当たり判定
	void Collision();
	//攻撃中の処理
	void Attack();
	//攻撃用のコリジョン
	void MakeAttackCollision();
	//攻撃1のエフェクト
	void MakeFirstSlashingEffect();
	//攻撃2のエフェクト
	void MakeSecondSlashingEffect();
	//攻撃3のエフェクト
	void MakeThirdSlashingEffect();
	//魔法攻撃を作成する
	void MakeMagicBall();
	void MakeExplosionBall();
	//足止めアイテム（効果範囲）のエフェクト
	void MakeRangeEffect();
	//足止めアイテム（使用時）のエフェクト
	void MakeEnemyStopEffect();
	//足止めアイテムのコリジョン
	void MakeEnemyStopCollision();
	//アイテム選択
	void ChoiseItem();


	//アニメーションの再生
	void PlayAnimation();
	//各ステートの遷移処理
	void ManageState();
	//アニメーション名前イベント
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//共通のステート遷移処理
	void ProcessCommonStateTransition();
	//待機ステートの遷移処理
	void ProcessIdleStateTransition();
	//走りステートの遷移処理
	void ProcessRunStateTransition();
	//被ダメージステートの遷移処理
	void ProcessDamageStateTransition();
	//1撃目の攻撃ステートの遷移処理
	void ProcessAttackStateTransition();
	//2撃目の攻撃ステートの遷移処理
	void ProcessAttack2StateTransition();
	//3撃目の攻撃ステートの遷移処理
	void ProcessAttack3StateTransition();
	//魔法攻撃ステートの遷移処理
	void ProcessMagicStateTransition();
	//アイテム使用ステートの遷移処理
	void ProcessUseItemStateTransition();

	void ProcessWinStateTransition();
	void ProcessDownStateTransition();

	//アニメーションクリップ
	enum EnAnimationClip {
		enAnimationClip_Idle,
		enAnimationClip_Run,
		enAnimationClip_Damage,
		enAnimationClip_FirstAttack,
		enAnimationClip_SecondAttack,
		enAnimationClip_ThirdAttack,
		enAnimationClip_Magic,
		enAnimationClip_UseItem,
		enAnimationClip_Win,
		enAnimationClip_Down,
		enAnimationClip_Num
	};
	
	SpriteRender PlayerUseMagic;
	SpriteRender PlayerSubMagic;
	SpriteRender m_itemWaku;
	SpriteRender m_itemStop;
	SpriteRender m_itemHeal;
	ModelRender          m_modelRender;
	SpriteRender         m_sprintRender;
	SpriteRender         m_sprintRender2;
	SpriteRender         m_sprintRender3;
	FontRender           m_fontRender;
	FontRender           m_fontRender2;
	FontRender           m_fontRender3;  
	AnimationClip			m_animationClips[enAnimationClip_Num];//アニメーションクリップ//モデルレンダー
	Vector3              m_position;       
	Vector3              m_scale = Vector3::One;                         //大きさ
	Vector3              m_moveSpeed;                          //移動速度
	Vector3              m_forward = Vector3::AxisZ;           //前//回転
	Quaternion           m_rotation;
	CharacterController  m_charaCon;                           //キャラクターコントローラー
	EnPlayerState        m_playerState = enPlayerState_Idle;   //ステート
	EnEquipState         m_equipState = enEquipState_Heal;
	//使う魔法攻撃
	EnUseMagic           m_UseMagic = enUseMagic_NormalMagic;
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_hp = 100;
	float               m_mp = 40;
	//0=一撃目　1=2撃目  2=3撃目
	int Type = 0;
	//0=タイマー停止　1=タイマー起動
	int move = 0;
	int move2 = 0;
	//1撃目からの経過時間
	float m_attackTimer = 0.0f;
	//2撃目からの経過時間
	float m_attackTimer2 = 0.0f;
	float timer = 0.0f;
	
	float battleStateTimer = 0.0f;

	int Thundernum = 5;
	int Healnum = 5;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	float m_alpha3 = 0.0f;
	EffectEmitter* effectEmitter;
	int a = 0;
	int b = 0;

	Enemy* m_enemy;
	Game* m_game;
	Tower* m_tower;

	int HavePoint = 0;
};
