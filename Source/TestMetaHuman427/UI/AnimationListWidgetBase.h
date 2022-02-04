// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonListWidgetBase.h"
#include "AnimationListWidgetBase.generated.h"

/**
 * 해당 위젯에 포함된 버튼에 캐릭터 외형 데이터를 할당 한다.
 */
UCLASS()
class TESTMETAHUMAN427_API UAnimationListWidgetBase : public UButtonListWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;

/* Init data table to buttons */
public:
	void InitButtonList(int ButtonCount) override;

	void InitAnimationButtons(int TargetRowIndex);
};