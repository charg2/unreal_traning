// Fill out your copyright notice in the Description page of Project Settings.


#include "ABCharacter.h"
#include "ABAnimInstance.h"

// Sets default values
AABCharacter::AABCharacter() : IsAttacking{ false }, MaxCombo{4}
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);


	GetMesh()->SetRelativeLocationAndRotation(FVector{ 0.0f, 0.0f, -88.0f }, FRotator{ 0.0f, -90.0f, 0.0f });
	//SpringArm->TargetArmLength = 400.0f;
	//SpringArm->SetRelativeRotation(FRotator{-15.0f, 0.0f, 0.0f});

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("SkeletalMesh'/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard'"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}


	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("AnimBlueprint'/Game/Book/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C'"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(EControlMode::DIABLO);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;


	AttackEndComboState();
}

// Called when the game starts or when spawned
void AABCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AABCharacter::SetControlMode(EControlMode ControlMode)
{
	this->CurrentControlMode = ControlMode;

	switch (ControlMode)
	{
	case EControlMode::GTA :
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator{0.0f, 720.0f, 0.0f};

		break;

	case EControlMode::DIABLO:

		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator{-45.0f, 0.0f, 0.0f});
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator{ -45.0f, 0.0f, 0.0f };
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch  = false;
		SpringArm->bInheritRoll   = false;
		SpringArm->bInheritYaw	  = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator{ 0.0f, 720.0f, 0.0f };

		break;
	}
}

// Called every frame
void AABCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
			break;

		case EControlMode::DIABLO:
		{
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
			break;
		}


	}

	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
			break;

		case EControlMode::DIABLO:
		{
			if (DirectionToMove.SizeSquared() > 0.0f)
			{
				GetController()->SetControlRotation(DirectionToMove.ToOrientationRotator());
				AddMovementInput(DirectionToMove);
				break;
			}
		}
	}
}

void AABCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UABAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	ABAnim->OnMontageEnded.AddDynamic(this, &AABCharacter::OnAttackMontageEnded);

	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void 
										{
											ABLOG(Warning, TEXT("OnNextAttackCheck"));
											CanNextCombo = false;

											if (true == IsComboInputOn)
											{
												AttackStartComboState();
												ABAnim->JumpToAttackMontageSection(CurrentCombo);
											}
										});

}

// Called to bind functionality to input
void AABCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AABCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AABCharacter::Attack);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AABCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AABCharacter::LeftRight);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AABCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AABCharacter::Turn);
}

void AABCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
		{
			// 회전 값으로부터 시선 방향을 가져오는 코드
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
			break;
		}
		case EControlMode::DIABLO:
		{
			DirectionToMove.X = NewAxisValue;
			break;
		}
	}

}

void AABCharacter::LeftRight(float NewAxisValue)
{

	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
		{
			// 회전 값으로부터 우측 방향을 가져오는 코드
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
			break;
		}
		case EControlMode::DIABLO:
		{
			DirectionToMove.Y = NewAxisValue;
			break;
		}
	}

}

void AABCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
		{
			AddControllerPitchInput(NewAxisValue);
			break;
		}
		/*case EControlMode::DIABLO:
		{
			break;
		}*/

	}


}

void AABCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
		{
			AddControllerYawInput(NewAxisValue);
			break;
		}
		/*case EControlMode::DIABLO:
		{
			break;
		}*/
	}
}

void AABCharacter::ViewChange()
{
	switch (CurrentControlMode)
	{
		case EControlMode::GTA:
		{
			// 회전 값으로부터 시선 방향을 가져오는 코드
			SetControlMode(EControlMode::DIABLO);
			break;
		}
		case EControlMode::DIABLO:
		{
			SetControlMode(EControlMode::GTA);
			break;
		}
	}

}

void AABCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterruted)
{
	ABCHECK(IsAttacking);
	ABCHECK(CurrentCombo > 0);

	IsAttacking = false;
	AttackEndComboState();
}

void AABCharacter::Attack()
{
	if (true == IsAttacking)
	{
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (true == CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AABCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}


void AABCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}