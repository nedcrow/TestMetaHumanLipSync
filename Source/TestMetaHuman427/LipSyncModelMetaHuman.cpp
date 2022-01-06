// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncModelMetaHuman.h"

ALipSyncModelMetaHuman::ALipSyncModelMetaHuman() {
	//PrimaryActorTick.
	bTriggerd = false;
}

void ALipSyncModelMetaHuman::BeginPlay() {
	Super::BeginPlay();
}

void ALipSyncModelMetaHuman::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	SetCursorLocations();
}

// Áß¾Ó ±âÁØ : Y= 0, Z= 180
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
		float minY = -50;
		float maxY = 50;
		float minZ = 120;
		float maxZ = 180;
		CursorLocationEyes = ClampVector(CursorLocationHead, FVector(150.0f, minY, minZ), FVector(150.0f, maxY, maxZ));

		// test
		FVector outLocation;
		FVector outDirection;
		PC->DeprojectMousePositionToWorld(OutLocationHead, outDirection);
	}
}