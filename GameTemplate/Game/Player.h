#pragma once

//クラス宣言
class Game;           //ゲームクラス
class Enemy;          //エネミークラス
class IceBall;        //魔法攻撃クラス
class Tower;          //タワークラス

//プレイヤー
class Player : public IGameObject
{
public:
	//プレイヤーのステート
	enum EnPlayerState {
		enPlayerState_FadeWait,         //フェード待機
		enPlayerState_Idle,             //待機中
		enPlayerState_Run,              //走り
		enPlayerState_UseItem,          //拘束魔法使用中
		enPlayerState_UseHeal,          //タワー修理使用中
		enPlayerState_FirstAttack,      //1回目の攻撃
		enPlayerState_SecondAttack,     //2回目の攻撃
		enPlayerState_ThirdAttack,      //3回目の攻撃
		enPlayerState_Magic,            //魔法攻撃
		enPlayerState_ReceiveDamage,    //被ダメージ
	};
	//アイテムのステート
	enum EnEquipState {
		enEquipState_Heal,              //タワー修理
		enEquipState_Thuner,            //拘束魔法
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
			m_playerState != enPlayerState_FadeWait&&
			m_playerState != enPlayerState_FirstAttack &&
			m_playerState != enPlayerState_SecondAttack &&
			m_playerState != enPlayerState_ThirdAttack &&
			m_playerState != enPlayerState_UseItem &&
			m_playerState != enPlayerState_UseHeal &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Magic;
	}

	//連続攻撃判定
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
	//足止めスキル（効果範囲）のエフェクト
	void MakeRangeEffect();
	//足止めスキル（使用時）のエフェクト
	void MakeEnemyStopEffect();
	//足止めスキルのコリジョン
	void MakeEnemyStopCollision();
	//スキル選択
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
	//フェード待機ステートの遷移処理
	void ProcessFadeWaitStateTransition();
	//タワー修理使用中ステートの遷移処理
	void ProcessHealStateTransition();

	//アニメーションクリップ
	enum EnAnimationClip {
		enAnimationClip_Idle,              //待機アニメーション
		enAnimationClip_Run,               //走りアニメーション
		enAnimationClip_Damage,            //被ダメージアニメーション
		enAnimationClip_FirstAttack,       //攻撃アニメーション（1撃目）
		enAnimationClip_SecondAttack,      //攻撃アニメーション（2撃目）
		enAnimationClip_ThirdAttack,       //攻撃アニメーション（3撃目）
		enAnimationClip_Magic,             //魔法攻撃アニメーション
		enAnimationClip_UseItem,		   //スキル使用アニメーション（拘束魔法）
		enAnimationClip_UseHeal,           //スキル使用アニメーション（タワー修理）
		enAnimationClip_Num                //アニメーションの数
	};
	
	SpriteRender m_PlayerUseMagic;
	SpriteRender PlayerSubMagic;
	SpriteRender m_itemWaku;
	SpriteRender m_itemStop;
	SpriteRender m_itemHeal;

	ModelRender          m_modelRender;
	SpriteRender         m_sprintRender;
	SpriteRender         m_sprintRender2;
	SpriteRender         m_sprintRender3;
	FontRender           m_fontRender1;
	FontRender           m_fontRender2;
	FontRender           m_fontRender3;  
	AnimationClip		 m_animationClips[enAnimationClip_Num];//アニメーションクリップ//モデルレンダー
	Vector3              m_position;       
	Vector3              m_scale = Vector3::One;                         //大きさ
	Vector3              m_moveSpeed;                          //移動速度
	Vector3              m_forward = Vector3::AxisZ;           //前//回転
	Quaternion           m_rotation;
	CharacterController  m_charaCon;                           //キャラクターコントローラー
	EnPlayerState        m_playerState = enPlayerState_Idle;   //ステート
	EnEquipState         m_equipState = enEquipState_Heal;
	//使う魔法攻撃
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_hp = 100;
	float                m_mp = 20;
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

	int Thundernum = 1;
	int Healnum = 1;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	float m_alpha3 = 0.0f;
	EffectEmitter* effectEmitter = nullptr;
	int a = 0;
	int b = 0;

	Enemy* m_enemy = nullptr;               //エネミー
	Game* m_game = nullptr;                 //ゲーム
	Tower* m_tower = nullptr;               //タワー
};
