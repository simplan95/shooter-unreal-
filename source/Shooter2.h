// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Shooter2, Log, All);//�α� ī�װ� ����

UENUM(BlueprintType)//�������Ʈ�� �������� ȣȯ
enum class ECharacterState : uint8 //ĳ���� ������Ʈ ����
{
	READY, //�ּ¹� UI �����Ϸ� ������� ����
	DEAD //ĳ���� ���
};