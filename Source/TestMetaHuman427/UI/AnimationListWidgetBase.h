// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnimationListWidgetBase.generated.h"

/**
 * �ش� ������ ���Ե� ��ư�� ĳ���� ���� �����͸� �Ҵ� �Ѵ�.
 */
UCLASS()
class TESTMETAHUMAN427_API UAnimationListWidgetBase : public UUserWidget
{
	GENERATED_BODY()

/* Property */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UScrollBox* ButtonList;

public:
	virtual void NativeConstruct()override;

/* Init data table */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UDataTable* TargetDataTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	TSubclassOf<UUserWidget> TargetButtonWidget;

	UFUNCTION()
	void InitAnimationButtons(int TargetRowIndex);
};