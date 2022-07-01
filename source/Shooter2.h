// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(Shooter2, Log, All);//로그 카테고리 정의

UENUM(BlueprintType)//블루프린트와 열거형의 호환
enum class ECharacterState : uint8 //캐릭터 스테이트 설정
{
	READY, //애셋및 UI 설정완료 대미지도 입음
	DEAD //캐릭터 사망
};