// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectivesActor/WeirdBoxActor.h"
#include "Components/BoxComponent.h"
#include "GravityGunTest/GravityGunTestCharacter.h"

// Sets default values
AWeirdBoxActor::AWeirdBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a Static mesh representation
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	// Set as root component
	SetRootComponent(StaticMesh);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMesh);
	BoxCollision->SetBoxExtent(FVector(200.f, 200.f, 200.f));

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeirdBoxActor::BoxOverlap);

}

// Called when the game starts or when spawned
void AWeirdBoxActor::BeginPlay()
{
	Super::BeginPlay();

	
	
}

// Called every frame
void AWeirdBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeirdBoxActor::BoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (AGravityGunTestCharacter* Character = Cast<AGravityGunTestCharacter>(OtherActor))
		{
			Character->OnObjective.Broadcast();
		}
	}
}