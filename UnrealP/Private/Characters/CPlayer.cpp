// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CPlayer.h"
#include "Player/CPlayerState.h"
#include "AbilitySystem/CAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CCombatComponent.h"
#include "Components/CTargetComponent.h"

#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DataSets/CInputDataAsset.h"
#include "Interface/IComboInterface.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/Abilities/CGameplayAbility_Block.h"
#include "Components/CItemComponent.h"
#include "Items/CWeapon.h"
#include "AbilitySystemBlueprintLibrary.h"

ACPlayer::ACPlayer()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 140));
	SpringArmComponent->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArmComponent->TargetArmLength = 250.0f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	TargetComponent = CreateDefaultSubobject<UCTargetComponent>("TargetComponent");



	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
}
void ACPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
}

void ACPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitAbilityActorInfo();
	SetupActionInput();
}

void ACPlayer::InitAbilityActorInfo()
{
	ACPlayerState* CPlayerState = GetPlayerState<ACPlayerState>();
	check(CPlayerState);
	CPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CPlayerState, this);
	Cast<UCAbilitySystemComponent>(CPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = CPlayerState->GetAbilitySystemComponent();
	AttributeSet = CPlayerState->GetAttributeSet();
	CombatComponent->SetAblitySystemComponent(AbilitySystemComponent);
	InitializeDefaultAttributes();
	AddCharacterStartupAbilities();
}

ACharacter* ACPlayer::GetTarget()
{
	IIStatusInterface* TargetStatus = Cast<IIStatusInterface>(TargetComponent->Target);
	if (!TargetStatus)
	{
		return nullptr;
	}
	if (!TargetStatus->GetIsAlive())
	{
		return nullptr;
	}
	return TargetComponent->Target;
}

void ACPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACharacter::Crouch, false);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACharacter::UnCrouch, false);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACPlayer::Look);
		SetupActionInput();
	}
}

void ACPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ACPlayer::Look(const FInputActionValue& Value)
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

void ACPlayer::SetupActionInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		if (InputDataAsset)
		{

			for (auto& ActionInput : InputDataAsset->ActionInput)
			{
				EnhancedInputComponent->BindAction(ActionInput.Value, ETriggerEvent::Started, this, &ACPlayer::TriggerAction, ActionInput.Key);
				EnhancedInputComponent->BindAction(ActionInput.Value, ETriggerEvent::Completed, this, &ACPlayer::ReleaseAction, ActionInput.Key);
			}
			for (auto& EquipmentInput : InputDataAsset->EquipmentInput)
			{
				EnhancedInputComponent->BindAction(EquipmentInput.Value, ETriggerEvent::Triggered, this, &ACPlayer::InputEquipment, EquipmentInput.Key);
			}
		}
	}
}

void ACPlayer::TriggerAction(const FInputActionValue& Value, TSubclassOf<UGameplayAbility> GameplayAbility)
{
	if (GameplayAbility->ImplementsInterface(UIComboInterface::StaticClass()))
	{
		if (CombatComponent->GetIsAttacking()&&AbilitySystemComponent->FindAbilitySpecFromClass(GameplayAbility))
		{
			CombatComponent->ComboCall();
		}
		else
		{
			AbilitySystemComponent->TryActivateAbilityByClass(GameplayAbility);
		}
	}
	else
	{
		AbilitySystemComponent->TryActivateAbilityByClass(GameplayAbility);
	}
}

void ACPlayer::ReleaseAction(const FInputActionValue& Value, TSubclassOf<UGameplayAbility> GameplayAbility)
{
	if (GameplayAbility->IsChildOf(UCGameplayAbility_Block::StaticClass()))
	{
		if (CombatComponent->GetIsBlocking())
		{
			CombatComponent->BlockEnd();
		}
	}
}

void ACPlayer::InputEquipment(const FInputActionValue& Value, int EquipmentSlot)
{
	TArray<ACItem*> Items = ItemComponent->GetItems();
	if (Items.Num() < EquipmentSlot)
	{
		return;
	}
	if (ACEquipment* Equipment = Cast<ACEquipment>(Items[EquipmentSlot-1]))
	{
		if (ItemComponent->MainEquipment!=nullptr)
		{
			ItemComponent->UnequipEquipment(ItemComponent->MainEquipment);
		}
		else
		{
			ItemComponent->EquipEquipment(Equipment);
		}
	}
}

