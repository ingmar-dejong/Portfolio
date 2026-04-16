// Copyright Unchained.

#pragma once

#include "CoreMinimal.h"
#include "ExternalAIIntegrationTypes.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ExternalAISubsystem.generated.h"

class FJsonObject;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnExternalAIResponseReceived,
	const FString&, RequestId,
	const FExternalAIResponse&, Response,
	bool, bSuccess,
	const FString&, ErrorMessage);

UCLASS()
class UNCHAINED_API UExternalAISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Unchained|External AI")
	FOnExternalAIResponseReceived OnResponseReceived;

	UFUNCTION(BlueprintCallable, Category = "Unchained|External AI")
	FString RequestNPCDecision(const FExternalAIRequest& RequestData);

	UFUNCTION(BlueprintPure, Category = "Unchained|External AI")
	bool IsUsingMockResponses() const;

private:
	void CompleteMockRequest(const FString& RequestId, const FExternalAIRequest RequestData);
	void HandleHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, FString RequestId);
	void BroadcastResult(const FString& RequestId, const FExternalAIResponse& ResponseData, bool bSuccess, const FString& ErrorMessage);
	TSharedPtr<FJsonObject> BuildRequestJson(const FExternalAIRequest& RequestData) const;
	bool TryParseResponseJson(const FString& ResponseString, FExternalAIResponse& OutResponse) const;
	FExternalAIResponse BuildMockResponse(const FExternalAIRequest& RequestData) const;
};
