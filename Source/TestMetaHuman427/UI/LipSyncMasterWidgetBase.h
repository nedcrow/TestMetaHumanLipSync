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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterListWidgetBase* CharacterList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UAnimationListWidgetBase* AnimationList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* InputButtonText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UMultiLineEditableText* InputTextBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* HideUIButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCanvasPanel* MainCanvasPanel;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void UnlockingWidge();

	UFUNCTION()
	void LockingWidge();

	UFUNCTION(BlueprintCallable)
	void BLockingWidget();

	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION(BlueprintCallable)
	void UnHideWidget();

	UFUNCTION(BlueprintCallable)
	void CallLastAnimation();
};
