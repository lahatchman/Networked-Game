// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GAP6003_A1P1_1808896GameMode.h"
class ACollectionPoint;
#include "GAP6003_A1P1_1808896Character.generated.h"

UCLASS(config=Game)
class AGAP6003_A1P1_1808896Character : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AGAP6003_A1P1_1808896Character();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Grab)
		class UPhysicsHandleComponent* physicsHandle;

	class ACollectable* grabCollectable;
	UPrimitiveComponent* grabComp;
	ECollisionEnabled::Type compCollisionType;

	void grab(ACollectable* collectable, FName bone);
	void dropObject();
	void detachObject();

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = Health)
		float health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = PlayerPosition)
		int playerPosition;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerChangeHealth(float newHealth);

	void ServerChangeHealth_Implementation(float newHealth);
	bool ServerChangeHealth_Validate(float newHealth);

	void ChangeHealth(float newHealth); //change attribute value on Server
	UFUNCTION(BlueprintCallable)
		void UpdateHealth(float newHealth);

	UFUNCTION(NetMulticast, Reliable)
		void MC_Collection(int playerPos);
	void MC_Collection_Implementation(int playerPos);
	UFUNCTION(BlueprintImplementableEvent)
		void CollectionBPImplemEvent(int playerPos);

	void Collection(ACollectionPoint* collectionPoint);
	void CollectionAsClientOrServer(ACollectionPoint* collectionPoint);

	void DestroyCollectable(ACollectable* collectable, int playerPos);
	UFUNCTION(Server, Reliable) //Note this is a Server function!
		void ServerDestroyCollectable(ACollectable* collectable, int playerPos);
	void ServerDestroyCollectable_Implementation(ACollectable* collectable, int playerPos);

	UFUNCTION(Server, Reliable) //Note this is a Server function!
		void ServerCollectableDamage(ACollectable* collectable, int playerPos);
	void ServerCollectableDamage_Implementation(ACollectable* collectable, int playerPos);

	virtual void BeginPlay() override;

	UFUNCTION(NetMulticast, Reliable)
		void MC_GameOver(int playerPos);
	void MC_GameOver_Implementation(int playerPos);
	UFUNCTION(BlueprintImplementableEvent)
		void gameOverBPImplemEvent(int playerPos);

	void CheckForWin(int playerPos);
	UFUNCTION(Server, Reliable) //Note this is a Server function!
		void ServerCheckForWin(int playerPos);
	void ServerCheckForWin_Implementation(int playerPos);
};

