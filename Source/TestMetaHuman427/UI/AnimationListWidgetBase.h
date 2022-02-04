// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonListWidgetBase.h"
#include "AnimationListWidgetBase.generated.h"

/**
 * �ش� ������ ���Ե� ��ư�� ĳ���� ���� �����͸� �Ҵ� �Ѵ�.
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