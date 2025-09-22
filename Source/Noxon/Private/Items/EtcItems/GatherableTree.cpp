// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/EtcItems/GatherableTree.h"


// Sets default values
AGatherableTree::AGatherableTree()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGatherableTree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGatherableTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

