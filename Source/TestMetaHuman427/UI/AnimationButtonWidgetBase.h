// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../DataStruct.h"
#include "AnimationButtonWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API UAnimationButtonWidgetBase : public UUserWidget
{
	GENERATED_BODY()

/* Properties */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* TargetButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* TargetText;

public:
	virtual void NativeConstruct() override;

/* Change */
public:
	UAnimationAsset* TargetAnimation;
	bool bIsLoopTargetAnim;

	UFUNCTION()
	void OnClickedAnimationButton();
};
