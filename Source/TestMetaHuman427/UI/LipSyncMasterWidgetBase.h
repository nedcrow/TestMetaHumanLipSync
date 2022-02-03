// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LipSyncMasterWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API ULipSyncMasterWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterListWidgetBase* CharacterList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UAnimationListWidgetBase* AnimationList;
};
