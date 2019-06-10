
#include "DynamicLoader.h"

#include "Runtime/Engine/Classes/Engine/Engine.h"
#include "Runtime/Core/Public/Async/Async.h"

#include "IImageWrapper.h"
#include "ImageUtils.h"
#include "IImageWrapperModule.h"

#include "BlueprintUtilityBPLibrary.h"

#include "Engine/Texture2D.h"


//UTexture2D* LoadTexture2DFromFile(const FString& FilePath, bool &IsValid);


 //Module loading is not allowed outside of the main thread, so we load the ImageWrapper module ahead of time.
//static IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(TEXT("ImageWrapper"));


void UImageLoader::LoadImageAsync(const FString& ImagePath)
{
	// The asynchronous loading operation is represented by a Future, which will contain the result value once the operation is done.
	// We store the Future in this object, so we can retrieve the result value in the completion callback below.
	Future = LoadImageFromDiskAsync (ImagePath, [this]()
	{
		// This is the same Future object that we assigned above, but later in time.
		// At this point, loading is done and the Future contains a value.
		if (Future.IsValid())
		{
			// Notify listeners about the loaded texture on the game thread.
			AsyncTask(ENamedThreads::GameThread, [this]() { LoadCompleted.Broadcast(Future.Get()); });
		}
	});
}

TFuture<UTexture2D*> UImageLoader::LoadImageFromDiskAsync(const FString& ImagePath, TFunction<void()> CompletionCallback)
{
	// Run the image loading function asynchronously through a lambda expression, capturing the ImagePath string by value.
	// Run it on the thread pool, so we can load multiple images simultaneously without interrupting other tasks.

	return Async<UTexture2D*>(EAsyncExecution::ThreadPool, [=]() { return LoadImageFromDisk(ImagePath); }, CompletionCallback);
}

UTexture2D* UImageLoader::LoadImageFromDisk( const FString& ImagePath)
{

	bool Succss;	int32 Weight;
	int32 Highting;
	
	return UBlueprintUtilityBPLibrary::LoadTexture2DFromFile(ImagePath,Succss, Weight,Highting);
}




AExeActor::AExeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

bool AExeActor::Getstat()
{

	bool a = FPlatformProcess::IsProcRunning(CheckProc);

	return a;

	//return true;
}


// Called when the game starts or when spawned
void AExeActor::BeginPlay()
{

	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("EXE Begin"));

}

void AExeActor::Tick(float DeltaTime)
{
	//Super::Tick();

	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("I AM IN There"));

	bool a = FPlatformProcess::IsProcRunning(CheckProc);

	if (!a && !bisShutDown)
	{

		UE_LOG(LogTemp, Warning, TEXT("EXE End"));

		bisShutDown = true;
		ProcShutdown.Broadcast(true);
		PrimaryActorTick.bCanEverTick = false;

		Destroy();

	}
	//return true;
}

