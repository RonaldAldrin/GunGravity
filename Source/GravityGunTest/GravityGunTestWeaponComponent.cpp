// Copyright Epic Games, Inc. All Rights Reserved.


#include "GravityGunTestWeaponComponent.h"
#include "GravityGunTestCharacter.h"
#include "GravityGunTestProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Camera/CameraComponent.h"


// Sets default values for this component's properties
UGravityGunTestWeaponComponent::UGravityGunTestWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	PrimaryComponentTick.bCanEverTick = true;
}

void UGravityGunTestWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bIsObjectPickedUp)
	{
		const FVector Location = Character->GetFirstPersonCameraComponent()->GetComponentLocation() + (Character->GetActorForwardVector() * 150.f);
		GravityGunHitResult.GetActor()->SetActorLocation(Location,true);
	}

	FString BoolCheck = bIsFirePressed ? TEXT("True") : TEXT("False");
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("FirePressed: %s"), *BoolCheck));
}


void UGravityGunTestWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	bIsFirePressed = true;

	GunSphereTrace();

	
	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UGravityGunTestWeaponComponent::FireRelease()
{
	if (PickedUpProjectile == nullptr) return;
	FVector ImpulseForce = Character->GetFirstPersonCameraComponent()->GetForwardVector() * FiringForce;
	PickedUpProjectile->AddImpulse(ImpulseForce,NAME_None,true);
	SetPickupProjectile(nullptr);
	bIsObjectPickedUp = false;
	bIsFirePressed = false;

}

void UGravityGunTestWeaponComponent::GunSphereTrace()
{
	if (PickedUpProjectile) return;
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		const FVector StartLoc = Character->GetFirstPersonCameraComponent()->GetComponentLocation();
		const FVector EndLoc = StartLoc + (Character->GetActorForwardVector() * PickUpRadius);

		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(GetOwner());
		ActorToIgnore.Add(Character);

		UKismetSystemLibrary::SphereTraceSingle(this, StartLoc, EndLoc, 10.f, ETraceTypeQuery::TraceTypeQuery1,
			false, ActorToIgnore, EDrawDebugTrace::ForDuration, GravityGunHitResult, true);
		if (GravityGunHitResult.bBlockingHit)
		{
			if (GravityGunHitResult.GetComponent()->GetCollisionObjectType() != ECollisionChannel::ECC_PhysicsBody) return;
			bIsObjectPickedUp = true;
			SetPickupProjectile(GravityGunHitResult.GetComponent());
		}
	}
}

void UGravityGunTestWeaponComponent::SetPickupProjectile(UPrimitiveComponent* ProjectileComponent)
{
	PickedUpProjectile = ProjectileComponent;
	if (PickedUpProjectile)
	{
		PickedUpProjectile->SetSimulatePhysics(true);
		PickedUpProjectile->SetAngularDamping(5.f);
	}
}

bool UGravityGunTestWeaponComponent::AttachWeapon(AGravityGunTestCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	// Check that the character is valid, and has no weapon component yet
	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UGravityGunTestWeaponComponent>())
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UGravityGunTestWeaponComponent::Fire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UGravityGunTestWeaponComponent::FireRelease);
		}
	}

	return true;
}

void UGravityGunTestWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// ensure we have a character owner
	if (Character != nullptr)
	{
		// remove the input mapping context from the Player Controller
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	// maintain the EndPlay call chain
	Super::EndPlay(EndPlayReason);
}