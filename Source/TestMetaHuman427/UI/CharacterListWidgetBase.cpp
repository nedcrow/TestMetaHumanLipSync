// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterListWidgetBase.h"
#include "../DataStruct.h"

#include "Kismet/GameplayStatics.h"

void UCharacterListWidgetBase::NativeConstruct()
{
	CharacterButtonWidget_A = Cast<UCharacterButtonWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterButtonWidget_A")));
	CharacterButtons.Add(CharacterButtonWidget_A);

	CharacterButtonWidget_B = Cast<UCharacterButtonWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterButtonWidget_B")));
	CharacterButtons.Add(CharacterButtonWidget_B);

	CharacterButtonWidget_C = Cast<UCharacterButtonWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterButtonWidget_C")));
	CharacterButtons.Add(CharacterButtonWidget_C);

	CharacterButtonWidget_D = Cast<UCharacterButtonWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterButtonWidget_D")));
	CharacterButtons.Add(CharacterButtonWidget_D);

	InitCharacterButtons();
}

void UCharacterListWidgetBase::InitCharacterButtons() {
	if(!CharacterDataTable->IsValidLowLevel()) {
		UE_LOG(LogTemp,Warning,TEXT("Null CharacterDataTable"));
		return;
	}

	int buttonsCount = CharacterButtons.Num();
	int dataRowCount = CharacterDataTable->GetRowNames().Num();
	int loopCount = buttonsCount > dataRowCount ? dataRowCount : buttonsCount;

	for (int i = 0; i < loopCount; i++) {
		FName rowName = FName(*FString().FromInt(i));
		CharacterButtons[i]->TargetCharacterRowData = CharacterDataTable->FindRow<FCharacterIconStruct>(rowName, TEXT(""));
	}	
}
