#pragma once

//クラス宣言
class Game;           //ゲームクラス
class MagicBall;      //魔法攻撃クラス
class Tower;          //タワークラス
class PlayerUi;       //プレイヤーUI

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
		enPlayerState_Repair,           //タワー修理使用ステート
		enPlayerState_Restraint,        //拘束魔法使用ステート
		enPlayerState_FirstAttack,      //1撃目の攻撃ステート
		enPlayerState_SecondAttack,     //2撃目の攻撃ステート
		enPlayerState_ThirdAttack,      //3撃目の攻撃ステート
		enPlayerState_Magic,            //魔法攻撃ステート
		enPlayerState_ReceiveDamage,    //被ダメージステート
	};
public:
	/// <summary>
	/// スキルステート
	/// </summary>
	enum EnSkillState {
		enSkillState_Repair,              //タワー修理ステート
		enSkillState_Restraint,           //拘束魔法ステート
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
			m_playerState != enPlayerState_Restraint &&
			m_playerState != enPlayerState_Repair &&
			m_playerState != enPlayerState_ReceiveDamage &&
			m_playerState != enPlayerState_Magic;
	}
	/// <summary>
	/// タワー修理ステートか？
	/// </summary>
	/// <returns>タワー修理ステートならtrue</returns>
	const bool SelectRepair() const
	{
		return m_skillState == enSkillState_Repair;
	}
	/// <summary>
	/// 拘束魔法ステートか？
	/// </summary>
	/// <returns>拘束魔法ステートならtrue</returns>
	const bool SelectRestraint() const
	{
		return m_skillState == enSkillState_Restraint;
	}
	/// <summary>
	/// タワー修理の使用回数を取得
	/// </summary>
	/// <returns>タワー修理の使用回数</returns>
	const int& GetRepairNum() const
	{
		return m_repairNum;
	}
	/// <summary>
	/// 拘束魔法の使用回数を取得
	/// </summary>
	/// <returns>拘束魔法の使用回数</returns>
	const int& GetRestraintNum() const
	{
		return m_restraintNum;
	}
private:
	/// <summary>
	/// スキル選択処理
	/// </summary>
	void SelectSkill();
	/// <summary>
	/// 連続攻撃の判定処理
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
	///	拘束魔法の効果範囲のエフェクト
	/// </summary>
	void MakeRestraintRangeEffect();
	/// <summary>
	/// 拘束魔法のエフェクト
	/// </summary>
	void MakeRestraintEffect();
	/// <summary>
	/// 拘束魔法のコリジョン
	/// </summary>
	void MakeRestraintCollision();
	/// <summary>
	/// タワー修理のエフェクト
	/// </summary>
	void MakeRepairEffect();
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
	void ProcessFristAttackStateTransition();
	/// <summary>
	/// 2撃目の攻撃ステートの遷移処理
	/// </summary>
	void ProcessSecondAttackStateTransition();
	/// <summary>
	/// 3撃目の攻撃ステートの遷移処理
	/// </summary>
	void ProcessThirdAttackStateTransition();
	/// <summary>
	/// 魔法攻撃ステートの遷移処理
	/// </summary>
	void ProcessMagicStateTransition();
	/// <summary>
	/// 拘束魔法ステートの遷移処理
	/// </summary>
	void ProcessRestraintStateTransition();
	/// <summary>
	/// フェード待機ステートの遷移処理
	/// </summary>
	void ProcessFadeWaitStateTransition();
	/// <summary>
	/// タワー修理ステートの遷移処理
	/// </summary>
	void ProcessRepairStateTransition();

	//アニメーションクリップ
	enum EnAnimationClip {
		enAnimationClip_Idle,              //待機アニメーション
		enAnimationClip_Run,               //走りアニメーション
		enAnimationClip_Damage,            //被ダメージアニメーション
		enAnimationClip_FirstAttack,       //1撃目の攻撃アニメーション
		enAnimationClip_SecondAttack,      //2撃目の攻撃アニメーション
		enAnimationClip_ThirdAttack,       //3撃目の攻撃アニメーション
		enAnimationClip_Magic,             //魔法攻撃アニメーション
		enAnimationClip_Restraint,		   //拘束魔法アニメーション
		enAnimationClip_Repair,            //タワー修理アニメーション
		enAnimationClip_Num                //アニメーションの数
	};
    
	ModelRender          m_modelRender;                          //モデルレンダー
	AnimationClip		 m_animationClips[enAnimationClip_Num];  //アニメーションクリップ//モデルレンダー
	Vector3              m_position;                             //座標
	Vector3              m_scale = Vector3::One;                 //大きさ
	Vector3              m_moveSpeed;                            //移動速度
	Vector3              m_forward = Vector3::AxisZ;             //プレイヤーの前方向のベクトル
	Quaternion           m_rotation;                             //回転
	CharacterController  m_charaCon;                             //キャラクターコントローラー
	EnPlayerState        m_playerState = enPlayerState_Idle;     //プレイヤーステート
	EnSkillState         m_skillState = enSkillState_Restraint;  //スキルステート
	bool                 m_isUnderAttack = false;                //攻撃中か？
	int                  m_swordBoneId = -1;                     //ソードボーンID
	float                m_mp = 40;                              //MP
	int                  m_attackNumber = 0;                     //攻撃番号
	bool                 m_secondAttackTimerFrag = false;        //1撃目からのタイマーのフラグ
	bool                 m_thirdAttackTimerFrag = false;         //2撃目からのタイマーのフラグ
	float                m_secondAttackTimer = 0.0f;             //1撃目からのタイマー
	float                m_thirdAttackTimer = 0.0f;              //2撃目からのタイマー
	bool                 m_pressButtonFrag = false;              //プレスボタンフラグ
	int                  m_restraintNum = 1;                     //拘束魔法の使用回数
	int                  m_repairNum = 1;                        //タワー修理の使用回数
	Game*                m_game = nullptr;                       //ゲーム
	Tower*               m_tower = nullptr;                      //タワー
	PlayerUi*            m_playerUi = nullptr;                   //プレイヤーUI
};
