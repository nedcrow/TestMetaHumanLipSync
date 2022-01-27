// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncModelMetaHuman.h"
#include "Components/SkeletalMeshComponent.h"
#include "HairStrandsCore/Public/GroomComponent.h"

ALipSyncModelMetaHuman::ALipSyncModelMetaHuman() {
	//PrimaryActorTick.
	bTriggerd = false;

	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(Body);
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
		Hair->SetupAttachment(Face);
		Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
		Eyebrows->SetupAttachment(Face);
		Fuzz = CreateDefaultSubobject<UGroomComponent>(TEXT("Fuzz"));
		Fuzz->SetupAttachment(Face);
		Eyelashes = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyelashes"));
		Eyelashes->SetupAttachment(Face);
		Mustache = CreateDefaultSubobject<UGroomComponent>(TEXT("Mustache"));
		Mustache->SetupAttachment(Face);
		Bread = CreateDefaultSubobject<UGroomComponent>(TEXT("Bread"));
		Bread->SetupAttachment(Face);
	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(Body);
	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(Body);
	Torso = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Torso"));
	Torso->SetupAttachment(Body);
}

void ALipSyncModelMetaHuman::BeginPlay() {
	Super::BeginPlay();
}

void ALipSyncModelMetaHuman::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	SetCursorLocations();
}

// 중앙 기준 : Y= 0, Z= 180
void ALipSyncModelMetaHuman::SetCursorLocations()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		FHitResult hitResult;
		PC->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);
		CursorLocationHead = CursorLocationEyes = hitResult.Location;

		// Head
		CursorLocationHead = FVector(
			CursorLocationHead.X,
			CursorLocationHead.Y - (CursorLocationHead.Y / RotationSpeed),
			CursorLocationHead.Z - ((CursorLocationHead.Z - 180) / RotationSpeed)
		);

		// Eyes
		float minY = -50.f;
		float maxY = 50.f;
		float minZ = 120.f;
		float maxZ = 180.f;
		CursorLocationEyes = ClampVector(CursorLocationEyes, FVector(150.0f, minY, minZ), FVector(150.0f, maxY, maxZ));

		// test
		FVector outLocation;
		FVector outDirection;
		PC->DeprojectMousePositionToWorld(OutLocationHead, outDirection);
	}
}