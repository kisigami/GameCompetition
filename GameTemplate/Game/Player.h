#pragma once

//�N���X�錾
class Game;           //�Q�[���N���X
class MagicBall;      //���@�U���N���X
class Tower;          //�^���[�N���X

/// <summary>
/// �v���C���[
/// </summary>
class Player : public IGameObject
{
public:
	/// <summary>
	/// �v���C���[�X�e�[�g
	/// </summary>
	enum EnPlayerState {
		enPlayerState_FadeWait,         //�t�F�[�h�ҋ@�X�e�[�g
		enPlayerState_Idle,             //�ҋ@�X�e�[�g
		enPlayerState_Run,              //����X�e�[�g
		enPlayerState_UseHeal,          //�^���[�C���g�p�X�e�[�g
		enPlayerState_UseItem,          //�S�����@�g�p�X�e�[�g
		enPlayerState_FirstAttack,      //1��ڂ̍U���X�e�[�g
		enPlayerState_SecondAttack,     //2��ڂ̍U���X�e�[�g
		enPlayerState_ThirdAttack,      //3��ڂ̍U���X�e�[�g
		enPlayerState_Magic,            //���@�U���X�e�[�g
		enPlayerState_ReceiveDamage,    //��_���[�W�X�e�[�g
	};
public:
	/// <summary>
	/// �X�L���X�e�[�g
	/// </summary>
	enum EnEquipState {
		enEquipState_Heal,              //�^���[�C��
		enEquipState_Thuner,            //�S�����@
	};
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	/// <summary>
	/// �傫�����擾
	/// </summary>
	/// <param name="scale">�傫��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// Mp��ݒ�
	/// </summary>
	/// <param name="mp">MP</param>
	void SetMp(const float& mp)
	{
		m_mp = mp;
	}
	/// <summary>
	/// Mp���擾
	/// </summary>
	/// <returns>MP</returns>
	const float& GetMp() const
	{
		return m_mp;
	}
	/// <summary>
	/// �������Ԃ��ǂ����擾
	/// </summary>
	/// <returns>������X�e�[�g�Ȃ�true</returns>
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
	/// �A���U���̔���
	/// </summary>
	void  Hit();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �G�̍U���Ƃ̓����蔻�菈��
	/// </summary>
	void Collision();
	/// <summary>
	/// �U������
	/// </summary>
	void Attack();
	/// <summary>
	/// �U���p�̓����蔻��p�̃R���W�������쐬����
	/// </summary>
	void MakeAttackCollision();
	/// <summary>
	/// 1���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeFirstSlashingEffect();
	//// <summary>
	/// 2���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeSecondSlashingEffect();
	/// <summary>
	/// 3���ڂ̍U���G�t�F�N�g
	/// </summary>
	void MakeThirdSlashingEffect();
	/// <summary>
	/// ���@�U�����쐬����
	/// </summary>
	void MakeMagicBall();
	/// <summary>
	///���~�߃X�L���i���ʔ͈́j�̃G�t�F�N�g
	/// </summary>
	void MakeRangeEffect();
	/// <summary>
	/// ���~�߃X�L���i�g�p���j�̃G�t�F�N�g
	/// </summary>
	void MakeEnemyStopEffect();
	/// <summary>
	/// ���~�߃X�L���̃R���W����
	/// </summary>
	void MakeEnemyStopCollision();
	/// <summary>
	/// �X�L���I������
	/// </summary>
	void ChoiseItem();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ManageState();
	/// <summary>
	/// �A�j���[�V�������O�C�x���g
	/// </summary>
	/// <param name="clipName">�A�j���[�V�����̖��O</param>
	/// <param name="eventName">�A�j���[�V�����C�x���g�L�[�̖��O</param>
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	/// <summary>
	/// ���ʂ̃X�e�[�g�J�ڏ���
	/// </summary>
	void ProcessCommonStateTransition();
	/// <summary>
	/// �ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// ����X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessRunStateTransition();
	/// <summary>
	/// ��_���[�W�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessDamageStateTransition();
	/// <summary>
	/// 1���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttackStateTransition();
	/// <summary>
	/// 2���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttack2StateTransition();
	/// <summary>
	/// 3���ڂ̍U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessAttack3StateTransition();
	/// <summary>
	/// ���@�U���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessMagicStateTransition();
	/// <summary>
	/// �A�C�e���g�p�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessUseItemStateTransition();
	/// <summary>
	/// �t�F�[�h�ҋ@�X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessFadeWaitStateTransition();
	/// <summary>
	/// �^���[�C���g�p���X�e�[�g�̑J�ڏ���
	/// </summary>
	void ProcessHealStateTransition();

	//�A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimationClip_Idle,              //�ҋ@�A�j���[�V����
		enAnimationClip_Run,               //����A�j���[�V����
		enAnimationClip_Damage,            //��_���[�W�A�j���[�V����
		enAnimationClip_FirstAttack,       //�U���A�j���[�V�����i1���ځj
		enAnimationClip_SecondAttack,      //�U���A�j���[�V�����i2���ځj
		enAnimationClip_ThirdAttack,       //�U���A�j���[�V�����i3���ځj
		enAnimationClip_Magic,             //���@�U���A�j���[�V����
		enAnimationClip_UseItem,		   //�X�L���g�p�A�j���[�V�����i�S�����@�j
		enAnimationClip_UseHeal,           //�X�L���g�p�A�j���[�V�����i�^���[�C���j
		enAnimationClip_Num                //�A�j���[�V�����̐�
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
	AnimationClip		 m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v//���f�������_�[
	Vector3              m_position;       
	Vector3              m_scale = Vector3::One;                         //�傫��
	Vector3              m_moveSpeed;                          //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;           //�O//��]
	Quaternion           m_rotation;
	CharacterController  m_charaCon;                           //�L�����N�^�[�R���g���[���[
	EnPlayerState        m_playerState = enPlayerState_Idle;   //�X�e�[�g
	EnEquipState         m_equipState = enEquipState_Heal;     //�g�����@�U��
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_mp = 40;
	//0=�ꌂ�ځ@1=2����  2=3����
	int Type = 0;
	//0=�^�C�}�[��~�@1=�^�C�}�[�N��
	int move = 0;
	int move2 = 0;
	//1���ڂ���̌o�ߎ���
	float m_attackTimer = 0.0f;
	//2���ڂ���̌o�ߎ���
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
	Game* m_game = nullptr;                 //�Q�[��
	Tower* m_tower = nullptr;               //�^���[
};
