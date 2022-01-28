// Fill out your copyright notice in the Description page of Project Settings.


#include "LipSyncModelBase.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SkeletalMeshComponent.h"
#include "LipSyncComponentWithOVR.h"
#include "OVRLipSyncFrame.h"
#include "OVRLipSyncPlaybackActorComponent.h"
#include "TTSComponentWithMindsLab.h"


// Sets default values
ALipSyncModelBase::ALipSyncModelBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);

	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture2D"));
	SceneCapture2D->SetRelativeLocation(FVector(11.4f, -25.0f, 170.0f));
	SceneCapture2D->SetRelativeRotation(FRotator(.0f, 120.0f, .0f));
	SceneCapture2D->SetupAttachment(Root);

	Audio = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	Audio->SetupAttachment(Root);

	OVRLipSyncPlaybackActor = CreateDefaultSubobject<UOVRLipSyncPlaybackActorComponent>(TEXT("OVRLipSyncPlaybackActor"));
	OVRLipSyncPlaybackActor->OnVisemesReady.AddDynamic(this, &ALipSyncModelBase::CallDeleFunc_VisemesReady);
	LipSyncWithOVR = CreateDefaultSubobject<ULipSyncComponentWithOVR>(TEXT("LipSyncWithOVR"));
	TTSWithMindsLab = CreateDefaultSubobject<UTTSComponentWithMindsLab>(TEXT("TTSWithMindsLab"));
}

// Called when the game starts or when spawned
void ALipSyncModelBase::BeginPlay()
{
	Super::BeginPlay();
	CallDeleFunc_VisemesReady();
	if (TTSWithMindsLab) TTSWithMindsLab->TTSEvent.AddDynamic(this, &ALipSyncModelBase::CallDeleFunc_TTSLipSync);
}

// Called every frame
void ALipSyncModelBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bUseLookAtCursor == true) SetCursorLocations();
	if (bUseLookAtCamera == true) SetCameraLocation();
}

void ALipSyncModelBase::CallDeleFunc_VisemesReady()
{
	LipSyncTargetSkeletalMesh = Body;
	if (LipSyncTargetSkeletalMesh) {
		OVRLipSyncPlaybackActor->AssignVisemesToMorphTargets(LipSyncTargetSkeletalMesh, {});
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Null target skeletal mesh."));
}

void ALipSyncModelBase::StartLipSync(USoundBase* SoundBase)
{
	Audio->SetSound(SoundBase);
	LipSyncWithOVR->OVRLipSyncProcessSoundBase(SoundBase);
	if (LipSyncWithOVR->LipSyncSequence) {
		OVRLipSyncPlaybackActor->Start(Audio, LipSyncWithOVR->LipSyncSequence);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Null target LipSyncSequence - LipSyncWithOVR(ULipSyncComponentWithOVR)."));
}

void ALipSyncModelBase::StartLipSyncFromText(FString TargetText)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *TargetText);
	TTSWithMindsLab->CallTTS(TargetText);
}

void ALipSyncModelBase::CallDeleFunc_TTSLipSync(USoundWave* TargetSoundWave)
{
	if (LipSyncWithOVR->OVRLipSyncProcessSoundWave(TargetSoundWave)) {
		Audio->SetSound(TargetSoundWave);
		OVRLipSyncPlaybackActor->Start(Audio, LipSyncWithOVR->LipSyncSequence);
	}
}

void ALipSyncModelBase::SetCameraLocation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) {
		TargetCameraLocation = PC->PlayerCameraManager->GetCameraLocation();
		TargetCameraLocation = ClampVector(TargetCameraLocation, CameraLocation_Min, CameraLocation_Max);
	}
}

// Áß¾Ó ±âÁØ : Y= 0, Z= 180
void ALipSyncModelBase::SetCursorLocations()
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
		float maxZ = 200;
		CursorLocationEyes = ClampVector(CursorLocationEyes, FVector(150.0f, minY, minZ), FVector(150.0f, maxY, maxZ));

		// test
		FVector outLocation;
		FVector outDirection;
		PC->DeprojectMousePositionToWorld(OutLocationHead, outDirection);
	}
}
