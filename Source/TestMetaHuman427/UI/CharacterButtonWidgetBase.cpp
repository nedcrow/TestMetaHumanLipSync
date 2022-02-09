// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterButtonWidgetBase.h"
#include "AnimationListWidgetBase.h"
#include "LipSyncMasterWidgetBase.h"
#include "../LipSyncModelMetaHuman.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Engine/StreamableManager.h"
#include "HairStrandsCore/Public/GroomAsset.h"
#include "HairStrandsCore/Public/GroomComponent.h"
#include "Kismet/GameplayStatics.h"

void UCharacterButtonWidgetBase::NativeConstruct()
{
	TargetBorder = Cast<UBorder>(UUserWidget::GetWidgetFromName(TEXT("TargetBorder")));
	
	TargetButton = Cast<UButton>(UUserWidget::GetWidgetFromName(TEXT("TargetButton")));
	if (TargetButton) TargetButton->OnClicked.AddDynamic(this, &UCharacterButtonWidgetBase::OnClickedCharacterButton);
	
	TargetText = Cast<UTextBlock>(UUserWidget::GetWidgetFromName(TEXT("TargetText")));
}

void UCharacterButtonWidgetBase::ApplyButtonText(FString Text)
{
	if (TargetText) TargetText->SetText(FText::FromString(Text));
}

void UCharacterButtonWidgetBase::OnClickedCharacterButton()
{
	FStreamableManager loader;
	if (!TargetCharacterRowData) {
		UE_LOG(LogTemp, Warning, TEXT("Null CharacterDataTable"));
		return;
	}

	/* 캐릭터 외형 정보 획득 */
	USkeletalMesh* face = loader.LoadSynchronous<USkeletalMesh>(TargetCharacterRowData->Face);
	USkeletalMesh* torso = loader.LoadSynchronous<USkeletalMesh>(TargetCharacterRowData->Torso);
	UMaterialInstance* torsoMaterial = loader.LoadSynchronous<UMaterialInstance>(TargetCharacterRowData->TorsoMaterial);
	USkeletalMesh* legs = loader.LoadSynchronous<USkeletalMesh>(TargetCharacterRowData->Legs);
	USkeletalMesh* feet = loader.LoadSynchronous<USkeletalMesh>(TargetCharacterRowData->Feet);
	UGroomAsset* hair = Cast<UGroomAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Hair_Groom));
	UGroomBindingAsset* hair_bind = Cast<UGroomBindingAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Hair_Binding));
	UGroomAsset* brows = Cast<UGroomAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Eyebrows_Groom));
	UGroomBindingAsset* brows_bind = Cast<UGroomBindingAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Eyebrows_Binding));
	UGroomAsset* lashes = Cast<UGroomAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Eyelashes_Groom));
	UGroomBindingAsset* lashes_bind = Cast<UGroomBindingAsset>(loader.LoadSynchronous<UObject>(TargetCharacterRowData->Eyelashes_Binding));

	/* 캐릭터 외형 변경 */
	ALipSyncModelMetaHuman* model = Cast<ALipSyncModelMetaHuman>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelMetaHuman::StaticClass()));

	if (!model) {
		UE_LOG(LogTemp, Warning, TEXT("Null model"));
		return;
	}

	model->Face->SetSkeletalMesh(face);
	model->Torso->SetSkeletalMesh(torso);
	model->Torso->SetMaterial(0, torsoMaterial);
	model->Legs->SetSkeletalMesh(legs);
	model->Feet->SetSkeletalMesh(feet);
	model->Hair->SetGroomAsset(hair);
	model->Eyebrows->SetGroomAsset(brows);
	model->Eyelashes->SetGroomAsset(lashes);
	model->Hair->SetBindingAsset(hair_bind);
	model->Eyebrows->SetBindingAsset(brows_bind);
	model->Eyelashes->SetBindingAsset(lashes_bind);

	if(TargetLipSyncMasterWidget) TargetLipSyncMasterWidget->AnimationList->InitAnimationButtons(TargetCharacterRowData->ItemIndex);
}
