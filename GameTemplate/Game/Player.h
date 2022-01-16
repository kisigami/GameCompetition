#pragma once

//�N���X�錾
class Game;           //�Q�[���N���X
class Enemy;          //�G�l�~�[�N���X
class IceBall;        //���@�U���N���X
class Tower;          //�^���[�N���X

//�v���C���[
class Player : public IGameObject
{
public:
	//�v���C���[�̃X�e�[�g
	enum EnPlayerState {
		enPlayerState_FadeWait,         //�t�F�[�h�ҋ@
		enPlayerState_Idle,             //�ҋ@��
		enPlayerState_Run,              //����
		enPlayerState_UseItem,          //�S�����@�g�p��
		enPlayerState_UseHeal,          //�^���[�C���g�p��
		enPlayerState_FirstAttack,      //1��ڂ̍U��
		enPlayerState_SecondAttack,     //2��ڂ̍U��
		enPlayerState_ThirdAttack,      //3��ڂ̍U��
		enPlayerState_Magic,            //���@�U��
		enPlayerState_ReceiveDamage,    //��_���[�W
	};
	//�A�C�e���̃X�e�[�g
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
	
	//���W��ݒ�
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	
	//���W���擾
	const Vector3& GetPosition() const
	{
		return m_position;
	}
	
	//�傫��
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	
	//��]��ݒ�
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}

	//�������Ԃ��ǂ���
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

	//�A���U������
	void  Hit();
	//�ړ�����
	void Move();
	//��]����
	void Rotation();
	//�����蔻��
	void Collision();
	//�U�����̏���
	void Attack();
	//�U���p�̃R���W����
	void MakeAttackCollision();
	//�U��1�̃G�t�F�N�g
	void MakeFirstSlashingEffect();
	//�U��2�̃G�t�F�N�g
	void MakeSecondSlashingEffect();
	//�U��3�̃G�t�F�N�g
	void MakeThirdSlashingEffect();
	//���@�U�����쐬����
	void MakeMagicBall();
	//���~�߃X�L���i���ʔ͈́j�̃G�t�F�N�g
	void MakeRangeEffect();
	//���~�߃X�L���i�g�p���j�̃G�t�F�N�g
	void MakeEnemyStopEffect();
	//���~�߃X�L���̃R���W����
	void MakeEnemyStopCollision();
	//�X�L���I��
	void ChoiseItem();


	//�A�j���[�V�����̍Đ�
	void PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	void ManageState();
	//�A�j���[�V�������O�C�x���g
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//���ʂ̃X�e�[�g�J�ڏ���
	void ProcessCommonStateTransition();
	//�ҋ@�X�e�[�g�̑J�ڏ���
	void ProcessIdleStateTransition();
	//����X�e�[�g�̑J�ڏ���
	void ProcessRunStateTransition();
	//��_���[�W�X�e�[�g�̑J�ڏ���
	void ProcessDamageStateTransition();
	//1���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttackStateTransition();
	//2���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttack2StateTransition();
	//3���ڂ̍U���X�e�[�g�̑J�ڏ���
	void ProcessAttack3StateTransition();
	//���@�U���X�e�[�g�̑J�ڏ���
	void ProcessMagicStateTransition();
	//�A�C�e���g�p�X�e�[�g�̑J�ڏ���
	void ProcessUseItemStateTransition();
	//�t�F�[�h�ҋ@�X�e�[�g�̑J�ڏ���
	void ProcessFadeWaitStateTransition();
	//�^���[�C���g�p���X�e�[�g�̑J�ڏ���
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
	AnimationClip		 m_animationClips[enAnimationClip_Num];//�A�j���[�V�����N���b�v//���f�������_�[
	Vector3              m_position;       
	Vector3              m_scale = Vector3::One;                         //�傫��
	Vector3              m_moveSpeed;                          //�ړ����x
	Vector3              m_forward = Vector3::AxisZ;           //�O//��]
	Quaternion           m_rotation;
	CharacterController  m_charaCon;                           //�L�����N�^�[�R���g���[���[
	EnPlayerState        m_playerState = enPlayerState_Idle;   //�X�e�[�g
	EnEquipState         m_equipState = enEquipState_Heal;
	//�g�����@�U��
	bool                 m_isUnderAttack = false;
	int                  m_swordBoneId = -1;
	float                m_hp = 100;
	float                m_mp = 20;
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

	int Thundernum = 1;
	int Healnum = 1;
	float m_alpha = 0.0f;
	float m_alpha2 = 0.0f;
	float m_alpha3 = 0.0f;
	EffectEmitter* effectEmitter = nullptr;
	int a = 0;
	int b = 0;

	Enemy* m_enemy = nullptr;               //�G�l�~�[
	Game* m_game = nullptr;                 //�Q�[��
	Tower* m_tower = nullptr;               //�^���[
};
