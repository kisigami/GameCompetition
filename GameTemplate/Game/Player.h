#pragma once

//クラス宣言
class Game;           //ゲームクラス
class MagicBall;      //魔法攻撃クラス
class Tower;          //タワークラス

/// <summary>
/// プレイヤー
/// </summary>
class Player : public IGameObject
{
public:
	/// <summary>
	/// プレイヤーステート
	/// </summary>
	enum EnPlayerState {
		enPlayerState_FadeWait,         //フェード待機ステート
		enPlayerState_Idle,             //待機ステート
		enPlayerState_Run,              //走りステート
		enPlayerState_UseHeal,          //タワー修理使用ステート
		enPlayerState_UseItem,          //拘束魔法使用ステート
		enPlayerState_FirstAttack,      //1回目の攻撃ステート
		enPlayerState_SecondAttack,     //2回目の攻撃ステート
		enPlayerState_ThirdAttack,      //3回目の攻撃ステート
		enPlayerState_Magic,            //魔法攻撃ステート
		enPlayerState_ReceiveDamage,    //被ダメージステート
	};
public:
	/// <summary>
	/// スキルステート
	/// </summary>
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
	/// 大きさを取得
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
	/// Mpを設定
	/// </summary>
	/// <param name="mp">MP</param>
	void SetMp(const float& mp)
	{
		m_mp = mp;
	}
	/// <summary>
	/// Mpを取得
	/// </summary>
	/// <returns>MP</returns>
	const float& GetMp() const
	{
		return m_mp;
	}
	/// <summary>
	/// 動ける状態かどうか取得
	/// </summary>
	/// <returns>動けるステートならtrue</returns>
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
private:
	/// <summary>
	/// 連続攻撃の判定
	/// </summary>
	void  Hit();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 敵の攻撃との当たり判定処理
	/// </summary>
	void Collision();
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack();
	/// <summary>
	/// 攻撃用の当たり判定用のコリジョンを作成する
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 1撃目の攻撃エフェクト
	/// </summary>
	void MakeFirstSlashingEffect();
	//// <summary>
	/// 2撃目の攻撃エフェクト
	/// </summary>
	void MakeSecondSlashingEffect();
	/// <summary>
	/// 3撃目の攻撃エフェクト
	/// </summary>
	void MakeThirdSlashingEffect();
	/// <summary>
	/// 魔法攻撃を作成する
	/// </summary>
	void MakeMagicBall();
	/// <summary>
	///足止めスキル（効果範囲）のエフェクト
	/// </summary>
	void MakeRangeEffect();
	/// <summary>
	/// 足止めスキル（使用時）のエフェクト
	/// </summary>
	void MakeEnemyStopEffect();
	/// <summary>
	/// 足止めスキルのコリジョン
	/// </summary>
	void MakeEnemyStopCollision();
	/// <summary>
	/// スキル選択処理
	/// </summary>
	void ChoiseItem();
	/// <summary>
	/// アニメーションの再生
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// 各ステートの遷移処理
	/// </summary>
	void ManageState();
	/// <summary>
	/// アニメーション名前イベント
	/// </summary>
	/// <param name="clipName">アニメーションの名前</param>
	/// <param name="eventName">アニメーションイベントキーの名前</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// 共通のステート遷移処理
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// 待機ステートの遷移処理
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// 走りステートの遷移処理
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// 被ダメージステートの遷移処理
	/// </summary>
	void ProcessDamageStateTransition();
	/// <summary>
	/// 1撃目の攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 2撃目の攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttack2StateTransition();
	/// <summary>
	/// 3撃目の攻撃ステートの遷移処理
	/// </summary>
	void ProcessAttack3StateTransition();
	/// <summary>
	/// 魔法攻撃ステートの遷移処理
	/// </summary>
	void ProcessMagicStateTransition();
	/// <summary>
	/// アイテム使用ステートの遷移処理
	/// </summary>
	void ProcessUseItemStateTransition();
	/// <summary>
	/// フェード待機ステートの遷移処理
	/// </summary>
	void ProcessFadeWaitStateTransition();
	/// <summary>
	/// タワー修理使用中ステートの遷移処理
	/// </summary>
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
	EnEquipState         m_equipState = enEquipState_Heal;     //使う魔法攻撃
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_mp = 40;
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
	int m_restraintNum = 1;
	int m_repairNum = 1;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	float m_alpha3 = 0.0f;
	EffectEmitter* effectEmitter = nullptr;
	int a = 0;
	int b = 0;
	Game* m_game = nullptr;                 //ゲーム
	Tower* m_tower = nullptr;               //タワー
};
