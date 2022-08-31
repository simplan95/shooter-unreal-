// Fill out your copyright notice in the Description page of Project Settings.


#include "FireCameraShake.h"

//ī�޶� �ݵ�
UFireCameraShake::UFireCameraShake()
{
	// ���ӽð�
	OscillationDuration = 0.5f;

	// Y�� ����, ���ļ�
	LocOscillation.Y.Amplitude = 1.f;
	LocOscillation.Y.Frequency = 100.f;

	// Z�� ����, ���ļ�
	LocOscillation.Z.Amplitude = 1.f;
	LocOscillation.Z.Frequency = 100.f;

	// ī�޶� ���� ����
	FOVOscillation.Amplitude = 1.f;//1ȸ ������ �����̴� �Ÿ�
	FOVOscillation.Frequency = 100.f;//���� �ֱ�

	// �ִ� �ѹ��� ī�޶���ũ -> ��� ȣ�� �����ϸ� ���
	bSingleInstance = true;
}