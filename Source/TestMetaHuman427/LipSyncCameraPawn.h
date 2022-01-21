// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LipSyncCameraPawn.generated.h"

UCLASS()
class TESTMETAHUMAN427_API ALipSyncCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALipSyncCameraPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* Camera;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


/* Common Cursor Event */
public:
	void LeftClick_Pressed();
	void LeftClick_Released();


/* RotateAround */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool bUseRotateAround = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	float MaxRotation = 180.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	bool bCanRotateAround = false;
	FVector2D StartMousePosition;
	FRotator StartRootRotator;
	void CallRotateAround();
	void RotateAroundCamera();
	void EndCameraRotating();


/* ZoomIn Out */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn")
	float WheelSpeed = 10.0f;
	void ZoomIn();
	void ZoomOut();


/* SelectCharacter */
	UFUNCTION(BlueprintCallable)
	void RecordScreen();


/* SelectCharacter */
private:
	AController* HitController;
	TArray<FHitResult> HitsOfTraceCursor();


/* Common */
private:
	void SetStartCursorTransform();
};
