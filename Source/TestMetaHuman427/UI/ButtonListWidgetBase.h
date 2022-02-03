// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonListWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API UButtonListWidgetBase : public UUserWidget
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

	virtual void InitButtonList();;
};
