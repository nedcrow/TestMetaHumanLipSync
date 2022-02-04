// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonListWidgetBase.h"
#include "../DataStruct.h"

#include "Components/ScrollBox.h"

void UButtonListWidgetBase::NativeConstruct()
{
	ButtonList = Cast<UScrollBox>(UUserWidget::GetWidgetFromName(TEXT("ButtonList")));
}

void UButtonListWidgetBase::InitButtonList(int ButtonCount)
{
	if (!ButtonList) {
		UE_LOG(LogTemp, Warning, TEXT("Null TargetButtonWidget"));
		return;
	}

	int childrenCount = ButtonList->GetChildrenCount();
	
	int loopCount;
	if (childrenCount < ButtonCount) {
		loopCount = ButtonCount - childrenCount;
		for (int i = 0; i < loopCount; i++) ButtonList->AddChild(CreateWidget<UUserWidget>(this, TargetButtonWidget));
	}
	else {
		loopCount = childrenCount - ButtonCount;
		for (int i = 0; i < loopCount; i++) ButtonList->RemoveChildAt(0);
	}
}
