// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor/AbilitySmokeGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GravityGunTest/GravityGunTestCharacter.h"

AAbilitySmokeGrenade::AAbilitySmokeGrenade()
{
	// Use a sphere as a simple collision representation
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->BodyInstance.SetCollisionProfileName("Projectile");
	StaticMesh->OnComponentHit.AddDynamic(this, &AAbilitySmokeGrenade::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	StaticMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	
	// Set as root component
	SetRootComponent(StaticMesh);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = StaticMesh;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AAbilitySmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AGravityGunTestCharacter>(GetOwner());
	
}


void AAbilitySmokeGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (GetOwner())
	{
		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("FirePressed: %s"), *GetOwner()->GetName()));
	}
}

