// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncCameraPawn.h"
#include "LipSync/LipSyncModelBase.h"

#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALipSyncCameraPawn::ALipSyncCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Body = CreateDefaultSubobject<USphereComponent>(TEXT("Body"));
	RootComponent = Body;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Body);
	SpringArm->SetRelativeRotation(FRotator(-0.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called every frame
void ALipSyncCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanRotateAround) RotateAroundCamera();
}

// Called to bind functionality to input
void ALipSyncCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("RotateAround"), IE_Pressed, this, &ALipSyncCameraPawn::CallRotateAround);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), IE_Pressed, this, &ALipSyncCameraPawn::LeftClick_Pressed);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), IE_Released, this, &ALipSyncCameraPawn::LeftClick_Released);
	PlayerInputComponent->BindAction(TEXT("ZoomIn"), IE_Released, this, &ALipSyncCameraPawn::ZoomIn);
	PlayerInputComponent->BindAction(TEXT("ZoomOut"), IE_Released, this, &ALipSyncCameraPawn::ZoomOut);
}

void ALipSyncCameraPawn::LeftClick_Pressed()
{
	SetStartCursorTransform();
}

void ALipSyncCameraPawn::LeftClick_Released()
{
	EndCameraRotating();
}

void ALipSyncCameraPawn::LockZLocation(float TargetZ)
{
	if(!bUseZLocationLocking) return;
	ALipSyncModelBase* model = Cast<ALipSyncModelBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ALipSyncModelBase::StaticClass()));
	if (model->Body) {
		SetActorRelativeLocation(FVector(
			GetActorLocation().X,
			GetActorLocation().Y,
			TargetZ
		));
	}
}

void ALipSyncCameraPawn::CallRotateAround()
{
	SetStartCursorTransform();
	bCanRotateAround = true;
}

void ALipSyncCameraPawn::RotateAroundCamera()
{
	APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PC) {
		int32 scrX;
		int32 scrY;
		FVector2D tempMousePosition;

		PC->GetViewportSize(scrX, scrY);
		PC->GetLocalPlayer()->ViewportClient->GetMousePosition(tempMousePosition);

		float addRotationPitch = ((tempMousePosition.Y - StartMousePosition.Y) / scrY * MaxRotation);
		float addRotationYaw = ((tempMousePosition.X - StartMousePosition.X) / scrX * MaxRotation);

		if (addRotationPitch != 0 && addRotationYaw != 0) {
			RootComponent->SetRelativeRotation(FRotator(
				StartRootRotator.Pitch - addRotationPitch,
				StartRootRotator.Yaw + addRotationYaw,
				0.0f
			));
		}
	}
}

void ALipSyncCameraPawn::EndCameraRotating()
{
	if (bCanRotateAround) bCanRotateAround = false;
}

void ALipSyncCameraPawn::ZoomIn()
{
	if (SpringArm->TargetArmLength < 40) {
		SpringArm->TargetArmLength = 40;
		return;
	}
	else {
		SpringArm->TargetArmLength -= WheelSpeed;
	}
}

void ALipSyncCameraPawn::ZoomOut()
{
	if (SpringArm->TargetArmLength > 500) {
		SpringArm->TargetArmLength = 800;
		return;
	}
	else {
		SpringArm->TargetArmLength += WheelSpeed;
	}
}

void ALipSyncCameraPawn::SetStartCursorTransform()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC) {
		PC->GetLocalPlayer()->ViewportClient->GetMousePosition(StartMousePosition);
		StartRootRotator = RootComponent->GetRelativeTransform().Rotator();
	}
}

