// Fill out your copyright notice in the Description page of Project Settings.

#include "avatar.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "TimerManager.h"


Aavatar::Aavatar()
{
	bUseControllerRotationPitch = false;

	//sets the rotaionrate to the character.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//sets up the spring arm for the camera.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	//sets up the follow camera to the character.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//makes the speed multiplier when you sprint.
	SprintSpeedMultiplier = 2.0f;

	//sets the value for dashing.
	CanDash = true;
	DashDistance = 6000.f;
	DashDown = 1.f;
	DashStop = 0.1f;
}

void Aavatar::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//all the input for the character.
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAxis("MoveForward", this, &Aavatar::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways", this, &Aavatar::MoveSideways);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &Aavatar::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &Aavatar::StopSprinting);

	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &Aavatar::Dash);
}

//the dash function.
void Aavatar::Dash()
{
	if (CanDash)
	{
		//sets that you can dash and if you have dashed its a delay between the next time you can dash.
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera-> GetForwardVector().Y, 0), true, true);
		CanDash = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &Aavatar::StopDashing, DashStop, false);
	}
}

void Aavatar::StopDashing()
{
	//makes the character stop dashing.
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &Aavatar::ResetDash, DashDown, false);
}

void Aavatar::ResetDash()
{
	//makes the can dash to true.
	CanDash = true;
}


void Aavatar::MoveForward(float Value)
{
	//makes the character be able to move forward and backwards.
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void Aavatar::MoveSideways(float Value)
{
	//makes the character be able to move sideways.
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void Aavatar::Sprint()
{
	//makes the character sprint at the speed i set when i hold shift.
	GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
}

void Aavatar::StopSprinting()
{
	//stops sprinting when you stop holding shift.
	GetCharacterMovement()->MaxWalkSpeed /= SprintSpeedMultiplier;
}

