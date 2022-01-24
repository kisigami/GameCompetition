#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

GameCamera::GameCamera()
{

}
GameCamera::~GameCamera()
{

}
bool GameCamera::Start()
{
	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(0.0f, 180.0f, -470.0f);

	//�΂˃J�����̏�����
	m_springCamera.Init(
		*g_camera3D,       //�΂˃J�����̏������s���J�������w�肷��
		2000.0f,           //�J�����̈ړ����x�̍ő�l
		true,              //�J�����ɐݒ肳��鋅�̃R���W����
		5.0f               //�J�����ƒn�`�Ƃ̓����蔻�����邩�ǂ����̃t���O
	);

	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");
	return true;
}
void GameCamera::UpdatePositionAndTarget()
{
	//�J�������X�V
	//�����_���v�Z����
	Vector3 target = m_player->GetPosition();
	//�v���C���[�̏�����𒍎��_�Ƃ���
	target.y += 320.0f;
	target += g_camera3D->GetForward() * 60.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//�p�b�h�̓��͂��g���ăJ��������
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y�����̉�
	m_rotation.SetRotationDeg(Vector3::AxisY, 1.5f * x);
	m_rotation.Apply(m_toCameraPos);
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();

	//X�����̉�
	m_rotation.SetRotationDeg(axisX, 1.5f * y);
	m_rotation.Apply(m_toCameraPos);
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();

	if (toPosDir.y < -0.5f)
	{
		//�J���������������
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.6f)
	{
		//�J����������������
		m_toCameraPos = toCameraPosOld;
	}

	//���_���v�Z����
	Vector3 pos = target + m_toCameraPos;

	//�΂˃J�����ɒ����_�Ǝ��_��ݒ肷��
	m_springCamera.SetPosition(pos);
	m_springCamera.SetTarget(target);
}

void GameCamera::Update()
{
	/// <summary>
	/// ���_�ƒ����_���X�V
	/// </summary>
	UpdatePositionAndTarget();
	//�J�����̍X�V
	m_springCamera.Update();
}
