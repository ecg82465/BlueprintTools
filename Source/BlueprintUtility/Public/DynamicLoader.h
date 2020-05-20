
#pragma once


#include "Core.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "PixelFormat.h"
#include "DynamicLoader.generated.h"

// Forward declarations
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnImageLoadCompleted, UTexture2D*, Texture, FString, ID);

UCLASS()
class  UImageLoader : public UObject
{
	GENERATED_BODY()

public:

	static UTexture2D* LoadImageFromDisk(const FString& ImagePath);

public:


	UPROPERTY(BlueprintAssignable, Category = ImageLoader, meta = (AllowPrivateAccess = true))
	FOnImageLoadCompleted LoadCompleted;

	/** This accessor function allows C++ code to bind to the event. */
	//FOnImageLoadCompleted& OnLoadCompleted()
	//{
	//	return LoadCompleted;
	//}


	/** Helper function that initiates the loading operation and fires the event when loading is done. */
	void LoadImageAsync(const FString& ImagePath,const FString& ID);


private:
	/**
	Holds the load completed event delegate.
	Giving Blueprint access to this private variable allows Blueprint scripts to bind to the event.
	*/
	FString LoadID;

	/** Holds the future value which represents the asynchronous loading operation. */
	TFuture<UTexture2D*> Future;


};

UCLASS()
class AExeActor : public AActor
{
	GENERATED_BODY()

public:
	AExeActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, meta = (Keywords = "FFMPEGPort sample test testing"), Category = "FFMPEGPortTesting")
		bool Getstat();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProcShutdown, bool, IsOver);

	UPROPERTY(BlueprintAssignable, Category = "BlueprintUtility|OS", meta = (AllowPrivateAccess = true))
		FOnProcShutdown ProcShutdown;

public:

	bool bisShutDown = false;

	FProcHandle CheckProc;

};
