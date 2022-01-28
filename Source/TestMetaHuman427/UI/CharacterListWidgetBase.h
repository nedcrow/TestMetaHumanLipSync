// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterButtonWidgetBase.h"
#include "CharacterListWidgetBase.generated.h"

/**
 * �ش� ������ ���Ե� ��ư�� ĳ���� ���� �����͸� �Ҵ� �Ѵ�.
 */
UCLASS()
class TESTMETAHUMAN427_API UCharacterListWidgetBase : public UUserWidget
{
	GENERATED_BODY()

/* Property */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterButtonWidgetBase* CharacterButtonWidget_A;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterButtonWidgetBase* CharacterButtonWidget_B;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterButtonWidgetBase* CharacterButtonWidget_C;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Widget")
	class UCharacterButtonWidgetBase* CharacterButtonWidget_D;


public:
	virtual void NativeConstruct()override;

/* Init data table */
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
	class UDataTable* CharacterDataTable;

	TArray<UCharacterButtonWidgetBase*> CharacterButtons;

	void InitCharacterButtons();
};
