// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LipSyncMasterWidgetBase.h"
#include "../DataStruct.h"
#include "CharacterButtonWidgetBase.generated.h"

/**
 * 해당 버튼을 클릭하면, ALipSyncModelMetaHuman 모델을 찾아서 외형 프로퍼티를 변경한다.
 */
UCLASS()
class TESTMETAHUMAN427_API UCharacterButtonWidgetBase : public UUserWidget
{
	GENERATED_BODY()
/* Property */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UBorder* TargetBorder;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UButton* TargetButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UTextBlock* TargetText;

public:
	virtual void NativeConstruct() override;

/* Change function */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULipSyncMasterWidgetBase* TargetLipSyncMasterWidget;

	FCharacterIconStruct* TargetCharacterRowData;

	UFUNCTION()
	void OnClickedCharacterButton();
};
