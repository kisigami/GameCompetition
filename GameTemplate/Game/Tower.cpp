#include "stdafx.h"
#include "Tower.h"
#include "Enemy.h"
#include "Game.h"
#include "GameOver.h"
#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float ENEMYDAMAGE = 10.0f;
}

Tower::Tower()
{

}

Tower::~Tower()
{

}

bool Tower::Start()
{
	EffectEngine::GetInstance()->ResistEffect(8, u"Assets/efk/Smoke.efk");
	EffectEngine::GetInstance()->ResistEffect(9, u"Assets/efk/towereffect.efk");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/towerbreak.wav");
	g_soundEngine->ResistWaveFileBank(44, "Assets/sound/towerdamage.wav");
	m_modelRender.Init("Assets/modelData/tower2/tower2.tkm");
	m_modelRender.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);
	
	m_modelRender2.Init("Assets/modelData/tower2/breaktower.tkm");
	m_modelRender2.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender2.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender2.SetRotation(m_rotation);
	//���f�����X�V����B
	m_modelRender2.Update();

	m_modelRender3.Init("Assets/modelData/tower2/allbreaktower.tkm");
	m_modelRender3.SetPosition(m_position);
	//�傫����ݒ肷��B
	m_modelRender3.SetScale(m_scale);
	//��]��ݒ肷��B
	m_modelRender3.SetRotation(m_rotation);
	//���f�����X�V����B
	m_modelRender3.Update();


	m_charaCon.Init(
		400.0f,
		300.0f,
		m_position
	);
	//TowerEffect();
	m_enemy = FindGO<Enemy>("enemy");
	m_game = FindGO<Game>("game");

	return true;
}


void Tower::Collision()
{
	//�_���[�W�����X�e�[�g��������
	if (m_towerState == enTowerState_DamageInvalid)
	{
		//�������Ȃ�
		return;
	}

	//�G�̍U���̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//Hp��10���炷
			float towerdurability = GetDurability();
			towerdurability -= ENEMYDAMAGE;
			SetDurability(towerdurability);

			//�ϋv�l��0.0f��菬����������
			if (GetDurability() <= 0.0f) 
			{
				//SE���Đ�����
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(44);
				se->SetVolume(0.4f);
				se->Play(false);
				//�_���[�W�����X�e�[�g��
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
			//�ϋv�l��0.0f���傫��������
			else {
				//�Q�[���I�[�o�[��ʒm����
				m_game->GameOverNotice();
				//�^���[�j��t���O��true�ɂ���
				m_towerBreakFrag = true;
				return;
			}
		}
	}

	//�G�̖��@�U���̓����蔻��
	const auto& collisions2 = g_collisionObjectManager->FindCollisionObjects("enemy_magic");
	for (auto collision : collisions2)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//Hp�����炷
			float towerdurability = GetDurability();
			towerdurability -= ENEMYDAMAGE;
			SetDurability(towerdurability);

			//�^���[�̑ϋv�l��0.0f��菬����������
			if (GetDurability() <= 0.0f) 
			{
				//�Q�[���I�[�o�[��ʒm����
				m_game->GameOverNotice();
				//�^���[�j��t���O��true�ɂ���
				m_towerBreakFrag = true;
				return;
			}
			//�^���[�̑ϋv�l��0.0f���傫��������
			else
			{
				//SE���Đ�����
				SoundSource* se = NewGO<SoundSource>(0);
				se->Init(44);
				se->SetVolume(0.4f);
				se->Play(false);
				//�_���[�W�����X�e�[�g��
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}

	//�{�X�̑����U���̓����蔻��
	const auto& collisions3 = g_collisionObjectManager->FindCollisionObjects("quick_attack");
	for (auto collision : collisions3)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//Hp�����炷
			float towerdurability = GetDurability();
			towerdurability -= 15;
			SetDurability(towerdurability);
			if (GetDurability() <= 0.0f) {
				m_game->GameOverNotice();
				m_towerBreakFrag = true;
				return;
			}
			else
			{
				SoundSource* m_se = NewGO<SoundSource>(0);
				m_se->Init(44);
				m_se->SetVolume(0.4f);
				m_se->Play(false);
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}

	//�{�X�̒x���U���̓����蔻��
	const auto& collisions4 = g_collisionObjectManager->FindCollisionObjects("heavy_attack");
	for (auto collision : collisions4)
	{
		//�R���W�����ƃL�����R�����Փ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//Hp�����炷
			float towerdurabiity = GetDurability();
			towerdurabiity -= 15.0f;
			SetDurability(towerdurabiity);

			if ( <= 0) {
				m_game->GameOverNotice();
				TowerBreak = true;
				return;
			}
			else
			{
				SoundSource* m_se = NewGO<SoundSource>(0);
				m_se->Init(44);
				m_se->SetVolume(0.4f);
				m_se->Play(false);
				m_towerState = enTowerState_DamageInvalid;
				return;
			}
		}
	}
}

void Tower::BreakEffect()
{
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(8);
	effectEmitter->SetScale(Vector3(5.0f, 10.0f, 5.0f));
	Vector3 effectPosition = m_position;
	effectEmitter->SetPosition(effectPosition);
	effectPosition.y += 100.0f;
	effectEmitter->Play();

	SoundSource* m_se = NewGO<SoundSource>(0);
	m_se->Init(6);
	m_se->SetVolume(0.5f);
	m_se->Play(false);

}

void  Tower::TowerEffect()
{
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(9);
	effectEmitter->SetScale(Vector3(10.0f, 10.0f, 10.0f));
	Vector3 effectPosition = m_position;
	effectPosition.y += 1800.0f;
	effectEmitter->SetPosition(effectPosition);
	effectEmitter->Play();
}

void Tower::DamageInvalid()
{
	//�_���[�W�����X�e�[�g�ȊO��������
	if (m_towerState != enTowerState_DamageInvalid)
	{
		//�������Ȃ�
		return;
	}
	//�_���[�W�����^�C�}�[������������
	damageinvalidtimer -= g_gameTime->GetFrameDeltaTime();
	//�_���[�W�����^�C�}�[��0.0f��菬����������
	if (damageinvalidtimer <= 0.0f)
	{
		//�^�C�}�[������������
		damageinvalidtimer = 1.0f;
		//�ҋ@�X�e�[�g��
		m_towerState = enTowerState_Idle;
	}
}

void Tower::ManageState()
{
	switch (m_BreakState)
	{
		//�j�󂳂�Ă��Ȃ��X�e�[�g�̎�
	case enBreakState_NoBreak:
		//�ϋv�l��100.0f��菬����������
		if (GetDurability() <= 100)
		{
			//����X�e�[�g��
			m_BreakState = enBreakState_HelfBreak;
			//�j��G�t�F�N�g���Đ�
			BreakEffect();
		}
		break;
		//����X�e�[�g�̎�
	case enBreakState_HelfBreak:
		//�ϋv�l��0.0f��菬����������
		if (GetDurability() <= 0)
		{
			//�S��X�e�[�g��
			m_BreakState = enBreakState_AllBreak;
			//�j��G�t�F�N�g���Đ�����
			BreakEffect();
		}
		//�ϋv�l��100.0f���傫��������
		else if (GetDurability() > 100)
		{
			//�j�󂳂�Ă��Ȃ��X�e�[�g��
			m_BreakState = enBreakState_NoBreak;
		}
		break;
		//�S��X�e�[�g�̎�
	case Tower::enBreakState_AllBreak:
		break;
	}
}

void Tower::Update()
{
	/// <summary>
	/// �e�X�e�[�g�̑J�ڏ���
	/// </summary>
	ManageState();
	/// <summary>
	/// �_���[�W��������
	/// </summary>
	DamageInvalid();
	/// <summary>
	/// �G�̍U�������蔻�菈��
	/// </summary>
	Collision();

	//���f���̍X�V����
	m_modelRender.Update();
	m_modelRender2.Update();
	m_modelRender3.Update();
}

void Tower::Render(RenderContext& rc)
{
	switch (m_BreakState)
	{
		//�j�󂳂�Ă��Ȃ��X�e�[�g�̎�
	case Tower::enBreakState_NoBreak:
		//�j�󂳂�Ă��Ȃ����f����`�悷��
		m_modelRender.Draw(rc);
		break;
		//����X�e�[�g�̎�
	case Tower::enBreakState_HelfBreak:
		//���󂵂Ă��郂�f����`�悷��
		m_modelRender2.Draw(rc);
		break;
		//�S��X�e�[�g�̎�
	case Tower::enBreakState_AllBreak:
		//�S�󂵂Ă��郂�f����`�悷��
		m_modelRender3.Draw(rc);
		break;
	}
}
