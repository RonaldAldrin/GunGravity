// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor/AbilitySmokeGrenade.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GravityGunTest/GravityGunTestCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

AAbilitySmokeGrenade::AAbilitySmokeGrenade()
{

	StaticMesh->OnComponentHit.AddDynamic(this, &AAbilitySmokeGrenade::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	StaticMesh->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	StaticMesh->CanCharacterStepUpOn = ECB_No;
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = StaticMesh;
	ProjectileMovement->InitialSpeed = 1000.f;
	ProjectileMovement->MaxSpeed = 1000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 5 seconds by default
	InitialLifeSpan = 5.0f;
}

void AAbilitySmokeGrenade::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AGravityGunTestCharacter>(GetOwner());
}


void AAbilitySmokeGrenade::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	if (SmokeSystem)
	{
		Smoke = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SmokeSystem, GetActorLocation(), GetActorRotation());
		Smoke->SetAutoDestroy(true);
	}
	
}

void AAbilitySmokeGrenade::Destroyed()
{
	Super::Destroyed();

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, FString::Printf(TEXT("Dead")));
	if (Smoke)
	{
		Smoke->DestroyComponent();
	}

}