// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncModelReallusion.h"
#include "Camera/CameraActor.h"

ALipSyncModelReallusion::ALipSyncModelReallusion() {
	//PrimaryActorTick.
	bTriggerd = false;
}

void ALipSyncModelReallusion::BeginPlay() {
	Super::BeginPlay();
}

void ALipSyncModelReallusion::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}


