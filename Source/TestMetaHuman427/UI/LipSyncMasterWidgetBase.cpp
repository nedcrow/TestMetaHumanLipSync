// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncMasterWidgetBase.h"
#include "CharacterListWidgetBase.h"
#include "AnimationListWidgetBase.h"

void ULipSyncMasterWidgetBase::NativeConstruct() {
	CharacterList = Cast<UCharacterListWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("CharacterList")));
	AnimationList = Cast<UAnimationListWidgetBase>(UUserWidget::GetWidgetFromName(TEXT("AnimationList")));
}