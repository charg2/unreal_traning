// Fill out your copyright notice in the Description page of Project Settings.


#include "AFountain.h"

// Sets default values
AAFountain::AAFountain()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BODY"));
	water = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WATER"));
	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("LIGHT"));
	splash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SPLASH"));

	RootComponent = body;
	water->SetupAttachment(body);
	light->SetupAttachment(body);
	splash->SetupAttachment(body);

	water->SetRelativeLocation(FVector{0.0f, 0.0f, 135.0f});// 상대 좌표 설정.
	light->SetRelativeLocation(FVector{ 0.0f, 0.0f, 195.0f });
	splash->SetRelativeLocation(FVector{ 0.0f, 0.0f, 195.0f });

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BODY(TEXT("/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Castle_Fountain_01.SM_Plains_Castle_Fountain_01"));

	if (SM_BODY.Succeeded())
	{
		body->SetStaticMesh(SM_BODY.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Meshes/Env/SM_Plains_Fountain_02.SM_Plains_Fountain_02"));
	if (SM_WATER.Succeeded())
	{
		water->SetStaticMesh(SM_WATER.Object);
	}


	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_SPLASH(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Ambient/Water/P_Water_Fountain_Splash_Base_01.P_Water_Fountain_Splash_Base_01'"));
	if (PS_SPLASH.Succeeded())
	{
		splash->SetTemplate(PS_SPLASH.Object);
	}


}

// Called when the game starts or when spawned
void AAFountain::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAFountain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

