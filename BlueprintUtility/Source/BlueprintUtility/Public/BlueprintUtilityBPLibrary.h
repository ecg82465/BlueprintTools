// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "DDSLoader.h"
#include "Core.h"
#include "Engine.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Runtime/Engine/Classes/Sound/SoundWave.h"

#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"

#include "BlueprintUtilityBPLibrary.generated.h"


//DECLARE_LOG_CATEGORY_EXTERN(CategoryName, Log, All)

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/

UENUM(BlueprintType)
enum class DirType : uint8
{
	GameDir,
	ContentDir
};


UCLASS()
class UBlueprintUtilityBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TEST", Keywords = "BlueprintUtility sample test testing"), Category = "BlueprintUtility")
	static float BlueprintUtilitySampleFunction(float Param);


	static FString GetFullPath(FString FilePath);

	/** Load a Texture2D from a file!  */
	UFUNCTION(BlueprintPure, Category = "BlueprintUtility", meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"))
	static UTexture2D* LoadTexture2DFromFile(const FString& FilePath, bool& IsValid, int32& Width, int32& Height);

	/** Load a Sound from a file!  */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static class USoundWave* LoadSoundWaveFromFile(const FString& FilePath);


	/** Read Engine Config */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static void ReadConfig(const FString& SectionName,const FString& ValueName, FString &ReturnValue);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static void WriteConfig(const FString& SectionName, const FString& ValueName, const FString &ReturnValue);


	/** Read Path Config */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static bool ReadCustomPathConfig(const FString&FilePath,const FString& SectionName, const FString& ValueName, FString &ReturnString);

	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static void WriteCustomPathConfig(const FString&FilePath, const FString& SectionName, const FString& ValueName,const FString &WriteString);

	/** RefrashAllSkeletal Trnasfrom In This Frame (Fix Animation Dither Error)*/
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static void RefrashAllSkeletallAnimation();


	/** Read Or Write Custom Text */
	UFUNCTION(BlueprintCallable, Category = "BlueprintUtility", meta = (Keywords = ""))
	static bool ReadFile(const FString FilePath, FString& ReturnString);

	UFUNCTION(BlueprintCallable)
	static bool WriteFile(const FString FilePath, const FString ReturnString);


	/** Get Path */
	UFUNCTION(BlueprintPure, Category = "BlueprintUtility", meta = (Keywords = ""))
	static FString GetGamePath(DirType E);


private:

	static bool ReadWavWaveData(USoundWave* sw, TArray<uint8>* rawFile);

	static bool ReadOggWaveData(USoundWave* sw, TArray<uint8>* rawFile);

};
