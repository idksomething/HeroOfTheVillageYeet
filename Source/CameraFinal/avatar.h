// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "avatar.generated.h"

UCLASS()
class CAMERAFINAL_API Aavatar : public ACharacter
{
	GENERATED_BODY()

		//Sets up the camera boom and follow camera.
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	Aavatar();

	//decides the turn rate and sprint speed multiplier that is set in the cpp file.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character Movement: Walking")
	float SprintSpeedMultiplier;

protected:
	void MoveForward(float Value);
	void MoveSideways(float Value);

	void Sprint();
	void StopSprinting();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	//class for follow camera and camera boom.
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	//sets the void.
	UFUNCTION()
		void Dash();

	UPROPERTY(EditAnywhere)
		float DashDistance;

	UPROPERTY(EditAnywhere)
		float DashDown;

	UPROPERTY(EditAnywhere)
		bool CanDash;

	UPROPERTY(EditAnywhere)
		float DashStop;

	//used for delay.
	UPROPERTY()
		FTimerHandle UnusedHandle;

	UFUNCTION()
		void StopDashing();

	UFUNCTION()
		void ResetDash();
};

