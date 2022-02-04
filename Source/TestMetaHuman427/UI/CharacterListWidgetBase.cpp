// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterListWidgetBase.h"
#include "CharacterButtonWidgetBase.h"
#include "../DataStruct.h"

#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"

void UCharacterListWidgetBase::NativeConstruct()
{
	ButtonList = Cast<UScrollBox>(UUserWidget::GetWidgetFromName(TEXT("ButtonList")));

	if (!TargetDataTable->IsValidLowLevel()) {
		UE_LOG(LogTemp, Warning, TEXT("Null DataTable"));
		return;
	}

	InitButtonList(TargetDataTable->GetRowNames().Num());
}

void UCharacterListWidgetBase::InitButtonList(int ButtonCount)
{
	Super::InitButtonList(ButtonCount);

	InitCharacterButtons();
}

void UCharacterListWidgetBase::InitCharacterButtons() {

	if (ButtonList) {
		for (int i = 0; i < ButtonList->GetChildrenCount(); i++) {
			FName rowName = FName(*FString().FromInt(i));
			UCharacterButtonWidgetBase*	characterButton = Cast<UCharacterButtonWidgetBase>(ButtonList->GetChildAt(i));

			characterButton->TargetCharacterRowData = TargetDataTable->FindRow<FCharacterIconStruct>(rowName, TEXT(""));

			characterButton->TargetLipSyncMasterWidget = TargetLipSyncMasterWidget;
		}
	}
}