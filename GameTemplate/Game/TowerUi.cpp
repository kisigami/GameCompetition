#include "stdafx.h"
#include "TowerUi.h"

#include "Tower.h"

namespace
{
	const float SIDE_SIZE = 1920.0f;                      //�摜�̉���
	const float LENGTH_SIZE = 1080.0f;                    //�摜�̗���
	const float TOWER_ICON_POSITION_X = -380.0f;          //�^���[�A�C�R����X���W
	const float TOWER_ICON_POSITION_Y = 212.0f;           //�^���[�A�C�R����Y���W
	const float TOWER_ICON_SCALE = 0.6f;                  //�^���[�A�C�R���̑傫��
	const float TOWER_DURABILITY_MAX = 200.0f;            //�^���[�̑ϋv�l�̍ő�l
	const float DURABILITY_POSITION_X = -825.0f;          //�ϋv�l��X���W
	const float DURABILITY_POSITION_Y = -63.0f;           //�ϋv�l��Y���W
	const float DURABILITY_FRAME_POSITION_X = 130.0f;     //�ϋv�l�̘g��X���W
	const float DURABILITY_FRAME_POSITION_Y = 477.0f;     //�ϋv�l�̘g��Y���W
	const float DURABILITY_BUTTOM_POSITION_X = 136.0f;    //�ϋv�l�̒��X���W
	const float DURABILITY_BUTTOM_POSITION_Y = 478.0f;    //�ϋv�l�̒��Y���W
	const float DURABILITY_BUTTOM_ALPHA = 0.7f;           //�ϋv�l�̒�̃A���t�@�l
	const float DURABILITY_FONT_POSITION_X = -150.0f;     //�ϋv�l�̕����̉摜��X���W
	const float DURABILITY_FONT_POSITION_Y = 65.0f;       //�ϋv�l�̕����̉摜��Y���W
	const float DURABILITY_FONT_SCALE = 0.9f;             //�ϋv�l�̕����̉摜�̑傫��
}

TowerUi::TowerUi()
{

}

TowerUi::~TowerUi()
{

}

bool TowerUi::Start()
{
	//�^���[�A�C�R���̉摜�̓ǂݍ���
	m_towerIcon.Init("Assets/sprite/UI/towericon2.dds", SIDE_SIZE, LENGTH_SIZE);
	m_towerIcon.SetPosition(Vector3(TOWER_ICON_POSITION_X, TOWER_ICON_POSITION_Y, 0.0f));
	m_towerIcon.SetScale(Vector3(TOWER_ICON_SCALE, TOWER_ICON_SCALE,1.0f));
	//�ϋv�l�̉摜�̓ǂݍ���
	m_durability.Init("Assets/sprite/UI/towerhpmidori.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durability.SetPosition(Vector3(DURABILITY_POSITION_X, DURABILITY_POSITION_Y, 0.0f));
	m_durability.SetPivot(Vector2(0.0f, 0.0f));
	//�ϋv�l�̘g�̉摜�̓ǂݍ���
	m_durabilityFrame.Init("Assets/sprite/UI/towerhpwaku.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityFrame.SetPosition(Vector3(DURABILITY_FRAME_POSITION_X, DURABILITY_FRAME_POSITION_Y, 0.0f));
	m_durabilityFrame.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	//�ϋv�l�̒�̉摜�̓ǂݍ���
	m_durabilityButtom.Init("Assets/sprite/UI/towerhpbuttom.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityButtom.SetPosition(Vector3(DURABILITY_BUTTOM_POSITION_X, DURABILITY_BUTTOM_POSITION_Y, 0.0f));
	m_durabilityButtom.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, DURABILITY_BUTTOM_ALPHA));
	//�ϋv�l�̕����̉摜�̓ǂݍ���
	m_durabilityFont.Init("Assets/sprite/UI/towerfont.dds", SIDE_SIZE, LENGTH_SIZE);
	m_durabilityFont.SetScale(Vector3(DURABILITY_FONT_SCALE, DURABILITY_FONT_SCALE, DURABILITY_FONT_SCALE));
	m_durabilityFont.SetPosition(Vector3(DURABILITY_FONT_POSITION_X, DURABILITY_FONT_POSITION_Y, 0.0f));
	m_durabilityFont.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_tower = FindGO<Tower>("tower");
	return true;
}

void TowerUi::DurabilityScale()
{
	//�ϋv�l�̉摜�̑傫��
	m_durabilityScale = m_tower->m_hp / TOWER_DURABILITY_MAX;
	//�ϋv�l�̉摜�̑傫����0.0f��菬����������
	if (m_durabilityScale < 0.0f)
	{
		//�ϋv�l�̉摜�̑傫����0.0f�ɂ���
		m_durability.SetScale(Vector3(0.0f, 1.0f, 1.0f));
		return;
	}
	m_durability.SetScale(Vector3(m_durabilityScale, 1.0f, 1.0f));
}

void TowerUi::Update()
{
	/// <summary>
	/// �ϋv�l�̉摜�̑傫���̏���
	/// </summary>
	DurabilityScale();

	//�摜�̍X�V����
	m_durabilityButtom.Update();
	m_towerIcon.Update();
	m_durabilityFrame.Update();
	m_durability.Update();
	m_durabilityFont.Update();
}

void TowerUi::Render(RenderContext& rc)
{
	//�摜�̕`�揈��
	m_durabilityButtom.Draw(rc);
	m_towerIcon.Draw(rc);
	m_durability.Draw(rc);
	m_durabilityFrame.Draw(rc);
	m_durabilityFont.Draw(rc);
}