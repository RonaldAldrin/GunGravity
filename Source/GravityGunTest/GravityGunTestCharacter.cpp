// Copyright Epic Games, Inc. All Rights Reserved.

#include "GravityGunTestCharacter.h"
#include "GravityGunTestProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "AbilityActor/AbilityActorBase.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGravityGunTestCharacter

AGravityGunTestCharacter::AGravityGunTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

}

//////////////////////////////////////////////////////////////////////////// Input

void AGravityGunTestCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGravityGunTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGravityGunTestCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGravityGunTestCharacter::Look);

		// Use Skill 1 HotBar
		EnhancedInputComponent->BindAction(SkillAction1, ETriggerEvent::Started, this, &AGravityGunTestCharacter::UseSkill1);

		// Use Skill 2 HotBar
		EnhancedInputComponent->BindAction(SkillAction2, ETriggerEvent::Started, this, &AGravityGunTestCharacter::UseSkill2);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGravityGunTestCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGravityGunTestCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGravityGunTestCharacter::UseSkill1()
{
	// return when the skill cooldown is not yet finish.
	if (bPressedSkill1) return;

	// Try and use skill 1
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		if (AbilitySmokeGrenadeClass)
		{
			const FRotator SpawnRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
			const FVector SpawnLocation = GetFirstPersonCameraComponent()->GetComponentLocation() + (GetFirstPersonCameraComponent()->GetForwardVector() * 100.f);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			// Spawn the projectile at  front
			SmokeGrenade = World->SpawnActor<AAbilityActorBase>(AbilitySmokeGrenadeClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			SmokeGrenade->SetOwner(this);


			bPressedSkill1 = true;
			OnSkill1Use.Broadcast(bPressedSkill1);

			
			float Cooldown = SmokeGrenade->AbilityDetailInfo.Cooldown;
			GetWorld()->GetTimerManager().SetTimer(Skill1TimerHandle, this, &AGravityGunTestCharacter::SkillCooldown1, Cooldown);
		}
	}

}

void AGravityGunTestCharacter::SkillCooldown1()
{
	bPressedSkill1 = false;
	OnSkill1Use.Broadcast(bPressedSkill1);
	GetWorld()->GetTimerManager().ClearTimer(Skill1TimerHandle);
}


void AGravityGunTestCharacter::UseSkill2()
{
	// return when the skill cooldown is not yet finish.
	if (bPressedSkill2) return;

	// Try and use skill 2
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		const FRotator SpawnRotation = GetFirstPersonCameraComponent()->GetComponentRotation();
		FVector SpawnLocation = GetFirstPersonCameraComponent()->GetComponentLocation() + (GetFirstPersonCameraComponent()->GetForwardVector() * 400.f);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at  front
		SummonTotem = World->SpawnActor<AAbilityActorBase>(AbilitySummonTotemClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		//SummonTotem->SetOwner(this);


		bPressedSkill2 = true;
		OnSkill2Use.Broadcast(bPressedSkill2);

		
		float Cooldown = SummonTotem->AbilityDetailInfo.Cooldown;
		GetWorld()->GetTimerManager().SetTimer(Skill2TimerHandle, this, &AGravityGunTestCharacter::SkillCooldown2, Cooldown);


	}
}

void AGravityGunTestCharacter::SkillCooldown2()
{
	bPressedSkill2 = false;
	OnSkill2Use.Broadcast(bPressedSkill2);
	GetWorld()->GetTimerManager().ClearTimer(Skill2TimerHandle);
}

float AGravityGunTestCharacter::GetCooldownPercentage(FTimerHandle SkillTimerHandle)
{
	float ElapsedTime = UKismetSystemLibrary::K2_GetTimerElapsedTimeHandle(this, SkillTimerHandle);
	float RemainingTime = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(this, SkillTimerHandle);
	float Timer = ElapsedTime + RemainingTime;
	return UKismetMathLibrary::NormalizeToRange(RemainingTime, 0.0f, Timer);
}


void AGravityGunTestCharacter::SpawnSkill(UWorld* World, TSubclassOf<AAbilityActorBase> SkilltoSpawnClass, AAbilityActorBase* SkilltoSpawn, const FVector Location, const FRotator Rotation)
{

	if (SkilltoSpawnClass != nullptr)
	{


		
	}

}



