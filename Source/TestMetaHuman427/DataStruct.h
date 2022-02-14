// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "DataStruct.generated.h"

USTRUCT(BlueprintType)
struct FCharacterIconStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> Body;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> Face;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UMaterialInstance> FaceMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Hair_Groom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Hair_Binding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Eyebrows_Groom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Eyebrows_Binding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Eyelashes_Groom;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSoftObjectPtr<class UObject> Eyelashes_Binding;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> Torso;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class UMaterialInstance> TorsoMaterial;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> Legs;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TAssetPtr<class USkeletalMesh> Feet;
};

USTRUCT(BlueprintType)
struct FAnimationIconStruct : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int ItemIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<TAssetPtr<class UAnimationAsset>> Animations;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FString> AnimNames;
};


/**
 * 
 */
UCLASS()
class TESTMETAHUMAN427_API UDataStruct : public UObject
{
	GENERATED_BODY()
	
};
