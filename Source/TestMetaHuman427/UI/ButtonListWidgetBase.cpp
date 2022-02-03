// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonListWidgetBase.h"
#include "../DataStruct.h"

#include "Components/ScrollBox.h"

void UButtonListWidgetBase::NativeConstruct()
{
	ButtonList = Cast<UScrollBox>(UUserWidget::GetWidgetFromName(TEXT("ButtonList")));
	
	InitButtonList();
}

void UButtonListWidgetBase::InitButtonList()
{
	if (!TargetDataTable->IsValidLowLevel()) {
		UE_LOG(LogTemp, Warning, TEXT("Null DataTable"));
		return;
	}

	if (!ButtonList) {
		UE_LOG(LogTemp, Warning, TEXT("Null ButtonList"));
		return;
	}

	if (!ButtonList) {
		UE_LOG(LogTemp, Warning, TEXT("Null TargetButtonWidget"));
		return;
	}

	int buttonsCount = ButtonList->GetChildrenCount();
	int dataRowCount = TargetDataTable->GetRowNames().Num();
	
	int loopCount;
	if (buttonsCount < dataRowCount) {
		loopCount = dataRowCount - buttonsCount;
		for (int i = 0; i < loopCount; i++) ButtonList->AddChild(CreateWidget<UUserWidget>(this, TargetButtonWidget));
	}
	else {
		loopCount = buttonsCount - dataRowCount;
		for (int i = 0; i < loopCount; i++) ButtonList->RemoveChildAt(0);
	}
}
