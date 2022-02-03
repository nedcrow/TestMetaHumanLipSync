// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonListWidgetBase.h"
#include "CharacterButtonWidgetBase.h"
#include "CharacterListWidgetBase.generated.h"

/**
 * �ش� ������ ���Ե� ��ư�� ĳ���� ���� �����͸� �Ҵ� �Ѵ�.
 */
UCLASS()
class TESTMETAHUMAN427_API UCharacterListWidgetBase : public UButtonListWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct()override;

/* Init data table */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULipSyncMasterWidgetBase* TargetLipSyncMasterWidget;

	virtual void InitButtonList() override;

	void InitCharacterButtons();
};
