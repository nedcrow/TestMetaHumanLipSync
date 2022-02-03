// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterListWidgetBase.h"
#include "../DataStruct.h"

#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

void UCharacterListWidgetBase::NativeConstruct()
{
	ButtonList = Cast<UScrollBox>(UUserWidget::GetWidgetFromName(TEXT("ButtonList")));

	InitButtonList();
}

void UCharacterListWidgetBase::InitButtonList()
{
	Super::InitButtonList();

	InitCharacterButtons();
}

void UCharacterListWidgetBase::InitCharacterButtons() {
	if (!TargetDataTable->IsValidLowLevel()) {
		UE_LOG(LogTemp, Warning, TEXT("Null TargetDataTable"));
		return;
	}

	if (ButtonList) {
		for (int i = 0; i < ButtonList->GetChildrenCount(); i++) {
			FName rowName = FName(*FString().FromInt(i));
			UCharacterButtonWidgetBase*	characterButton = Cast<UCharacterButtonWidgetBase>(ButtonList->GetChildAt(i));

			characterButton->TargetCharacterRowData = TargetDataTable->FindRow<FCharacterIconStruct>(rowName, TEXT(""));

			characterButton->TargetLipSyncMasterWidget = TargetLipSyncMasterWidget;
		}
	}
}