// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityActor/AbilitySummonTotem.h"

AAbilitySummonTotem::AAbilitySummonTotem()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetSimulatePhysics(true);
	// Die after 8 seconds by default
	InitialLifeSpan = 8.0f;
}
