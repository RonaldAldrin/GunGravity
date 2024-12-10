// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor/AbilityActorBase.h"

// Sets default values
AAbilityActorBase::AAbilityActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a Static mesh representation
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->BodyInstance.SetCollisionProfileName("Projectile");

	// Set as root component
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AAbilityActorBase::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AAbilityActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

