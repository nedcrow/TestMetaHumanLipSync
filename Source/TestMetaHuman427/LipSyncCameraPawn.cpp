// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncCameraPawn.h"
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

	//PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("PawnMovement"));
}

// Called when the game starts or when spawned
void ALipSyncCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALipSyncCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bCanRotateAround) RotateAroundCamera();
	//SetActorLocation(MCM->CurrentModel->GetActorLocation() + FVector(0.f, 0.f, 78.f));
	//SpringArm->TargetArmLength = 60;
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
	TArray<FHitResult> hits = HitsOfTraceCursor();
	if (hits.Num() > 0) {
		for (auto hit : hits) {
			if (hit.GetActor() && hit.GetActor()->Tags.IndexOfByKey(TEXT("Model")) >= 0) {
				//AModelCharacter* model = Cast<AModelCharacter>(hit.GetActor());
				//AModelCharacterManager* MCM = Cast<AModelCharacterManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AModelCharacterManager::StaticClass()));
				//TArray<AActor*> displayPoints;
				//FVector DisplayPointLocation;
				//UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("DisplayPoint"), displayPoints);
				//DisplayPointLocation = displayPoints.Num() > 0 ? displayPoints[0]->GetActorLocation() : model->StartPointLocation;

				///* Target Speed */
				//model->SetWalkSpeed(model->MaxSpeed);

				break;
			}
		}

	}
}

void ALipSyncCameraPawn::CallRotateAround()
{
	SetStartCursorTransform();
	bCanRotateAround = true;

	//if (FaceCamera) {
	//	FaceCamera->
	//}
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

void ALipSyncCameraPawn::RecordScreen()
{
	//android
}

TArray<FHitResult> ALipSyncCameraPawn::HitsOfTraceCursor()
{
	TArray<FHitResult> OutHits;
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && PC->IsLocalController()) {
		// tracing 준비
		float MouseX;
		float MouseY;
		FVector CursorWorldPosition;
		FVector CursorWorldDirection;

		FVector CameraLocation;
		FRotator CameraRotation;

		PC->GetMousePosition(MouseX, MouseY);
		PC->DeprojectScreenPositionToWorld(MouseX, MouseY, CursorWorldPosition, CursorWorldDirection);

		PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + (CursorWorldDirection * 99999.f);

		TArray<TEnumAsByte<EObjectTypeQuery>> Objects;
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
		Objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

		TArray<AActor*> ActorToIgnore;

		// 장애물을 대비한 multi tracing
		bool Result = UKismetSystemLibrary::LineTraceMultiForObjects(
			GetWorld(),
			TraceStart,
			TraceEnd,
			Objects,
			true,
			ActorToIgnore,
			EDrawDebugTrace::None,
			OutHits,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			100.0f
		);
		return OutHits;
	}
	return OutHits;
	return TArray<FHitResult>();
}

void ALipSyncCameraPawn::SetStartCursorTransform()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC) {
		PC->GetLocalPlayer()->ViewportClient->GetMousePosition(StartMousePosition);
		StartRootRotator = RootComponent->GetRelativeTransform().Rotator();
	}
}

