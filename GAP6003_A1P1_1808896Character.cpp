// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAP6003_A1P1_1808896Character.h"
#include "GAP6003_A1P1_1808896GameMode.h"
#include "GAP6003_A1P1_1808896_GameState.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Collectable.h"
#include "CollectionPoint.h"
#include "GameFramework/PlayerState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h" //for replication

//////////////////////////////////////////////////////////////////////////
// AGAP6003_A1P1_1808896Character

AGAP6003_A1P1_1808896Character::AGAP6003_A1P1_1808896Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	physicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	grabCollectable = nullptr;

	health = 100.0f;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGAP6003_A1P1_1808896Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AGAP6003_A1P1_1808896Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AGAP6003_A1P1_1808896Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AGAP6003_A1P1_1808896Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AGAP6003_A1P1_1808896Character::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AGAP6003_A1P1_1808896Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AGAP6003_A1P1_1808896Character::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGAP6003_A1P1_1808896Character::OnResetVR);

	// Bind Grab events
	PlayerInputComponent->BindAction("Release", IE_Pressed, this, &AGAP6003_A1P1_1808896Character::dropObject);
}


void AGAP6003_A1P1_1808896Character::OnResetVR()
{
	// If GAP6003_A1P1_1808896 is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in GAP6003_A1P1_1808896.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGAP6003_A1P1_1808896Character::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AGAP6003_A1P1_1808896Character::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AGAP6003_A1P1_1808896Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGAP6003_A1P1_1808896Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGAP6003_A1P1_1808896Character::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGAP6003_A1P1_1808896Character::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AGAP6003_A1P1_1808896Character::grab(ACollectable* collectable, FName bone)
{
	if (!grabCollectable)
	{
		grabCollectable = collectable;
		grabComp = grabCollectable->collisionComponent;
		compCollisionType = grabComp->GetCollisionEnabled(); //get current collision type
		grabComp->SetSimulatePhysics(false);	//switch physics off
		grabComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //set collision off
		physicsHandle->GrabComponentAtLocation(grabCollectable->collisionComponent, bone, grabCollectable->GetActorLocation());

		grabCollectable->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true), FName("CollectableSocket"));
	}
}

void AGAP6003_A1P1_1808896Character::dropObject()
{
	if (grabCollectable) detachObject();
}

void AGAP6003_A1P1_1808896Character::detachObject()
{
	grabCollectable->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepRelative, EDetachmentRule::KeepWorld, true));
	//reset physics & collision on comp
	grabComp->SetSimulatePhysics(true);
	grabComp->SetCollisionEnabled(compCollisionType);
	grabCollectable = nullptr;
}

void AGAP6003_A1P1_1808896Character::Collection(ACollectionPoint* collectionPoint)
{
	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	if (collectionPoint) 
	{
		if (grabCollectable && grabCollectable->CollectableColour == collectionPoint->collectableColourReqd)
		{
			CollectionAsClientOrServer(collectionPoint);
		}
		else if (grabCollectable && grabCollectable->CollectableColour != collectionPoint->collectableColourReqd)
		{
			UpdateHealth(health - 10.0f);

			if (GetLocalRole() == ROLE_Authority)
			{
				DestroyCollectable(grabCollectable, 0);
				if (gameMode)
				{
					for (int i = 0; i < 5; i++)
						gameMode->SpawnObstaclesRight();
				}
			}
			else
			{
				ServerCollectableDamage(grabCollectable, 1); //will call implementation version!
			}

			grabCollectable = nullptr;
		}
		else 
		{
			FString msg = "You need the " + getEnumColoursNameByValue((int32)collectionPoint->collectableColourReqd) + " collectable!";
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, msg);
		}
	}
}

void AGAP6003_A1P1_1808896Character::CollectionAsClientOrServer(ACollectionPoint* collectionPoint)
{
	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	UE_LOG(LogTemp, Warning, TEXT("CollectionAsClientOrServer rem role: %d, loc role: %d"), GetRemoteRole(), GetLocalRole());

	if (GetLocalRole() == ROLE_Authority)
	{
		DestroyCollectable(grabCollectable, 0);
		MC_Collection(1);
		if (gameMode)
		{
			for (int i = 0; i < 5; i++)
				gameMode->SpawnObstaclesLeft();
		}
	}
	else
	{
		ServerDestroyCollectable(grabCollectable, 1); //will call implementation version!
	}

	grabCollectable = nullptr;
}

void AGAP6003_A1P1_1808896Character::DestroyCollectable(ACollectable* collectable, int playerPos)
{
	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	if (collectable)
	{
		collectable->Destroy();

		if (gameMode) 
		{
			if (playerPos == 0) 
			{
				gameMode->SpawnCollectableRight();
			}
			else
			{
				gameMode->SpawnCollectableLeft();
			}
		}
	}

	collectable = nullptr;
}

void AGAP6003_A1P1_1808896Character::ServerDestroyCollectable_Implementation(ACollectable* collectable, int playerPos)
{
	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	DestroyCollectable(collectable, playerPos);  //call server version!
	MC_Collection(0);

	if (gameMode)
	{
		for (int i = 0; i < 5; i++)
			gameMode->SpawnObstaclesRight();
	}
}

void AGAP6003_A1P1_1808896Character::ServerCollectableDamage_Implementation(ACollectable* collectable, int playerPos)
{
	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	DestroyCollectable(collectable, playerPos);  //call server version!
	if (gameMode)
	{
		for (int i = 0; i < 5; i++)
			gameMode->SpawnObstaclesLeft();
	}
}

void AGAP6003_A1P1_1808896Character::BeginPlay()
{
	Super::BeginPlay();

	AGAP6003_A1P1_1808896GameMode* gameMode = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	if (gameMode) 
	{
		playerPosition = 0;
	}
	else 
	{
		playerPosition = 1;
	}
}

void AGAP6003_A1P1_1808896Character::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Tell the engine to update each time variable changes
	DOREPLIFETIME(AGAP6003_A1P1_1808896Character, health);
}

void AGAP6003_A1P1_1808896Character::ChangeHealth(float newHealth)
{
	AGAP6003_A1P1_1808896GameMode* gm = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());

	if (health < 10) 
	{
		if (gm)
		{
			UCharacterMovementComponent* charMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent());

			if (charMoveComp)
			{
				charMoveComp->StopMovementImmediately();
				charMoveComp->DisableMovement();
				charMoveComp->SetComponentTickEnabled(false);
			}

			CheckForWin(1);

		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("in changeHealth, health: %f"), newHealth);
		health = newHealth;
	}
}

void AGAP6003_A1P1_1808896Character::UpdateHealth(float newHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("in updateHealth rem role: %d, loc role: %d"), GetRemoteRole(), GetLocalRole());

	if (GetLocalRole() == ROLE_Authority) //Get needed for v4.25!
	{	
		ChangeHealth(newHealth);
	}
	else 
	{
		ServerChangeHealth(newHealth); //will call implementation version!
	}
}

void AGAP6003_A1P1_1808896Character::MC_Collection_Implementation(int playerPos)
{
	CollectionBPImplemEvent(playerPos);
}


void AGAP6003_A1P1_1808896Character::ServerChangeHealth_Implementation(float newHealth) 
{
	UE_LOG(LogTemp, Warning, TEXT("in ServerChangeHealth_Implementation, health: %f"), newHealth);

	if (health < 1) 
	{
		ServerCheckForWin(0);
	}
	else 
	{
		ChangeHealth(newHealth);
	}
}

bool AGAP6003_A1P1_1808896Character::ServerChangeHealth_Validate(float newHealth)
{
	return true;
}

void AGAP6003_A1P1_1808896Character::MC_GameOver_Implementation(int playerPos)
{
	gameOverBPImplemEvent(playerPos);
}

void AGAP6003_A1P1_1808896Character::CheckForWin(int playerPos)
{
	AGAP6003_A1P1_1808896GameMode* gm = Cast<AGAP6003_A1P1_1808896GameMode>(GetWorld()->GetAuthGameMode());
	gm->gameOver(playerPos);
}

void AGAP6003_A1P1_1808896Character::ServerCheckForWin_Implementation(int playerPos)
{
	CheckForWin(playerPos);
}

