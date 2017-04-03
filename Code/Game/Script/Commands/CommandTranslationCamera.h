#pragma once
#include "Script/Command/AbstractFunctionCommand.h"
class CommandTranslationCamera :
	public ska::AbstractFunctionCommand
{
public:
	CommandTranslationCamera(ska::EntityManager& entityManager);
	virtual ~CommandTranslationCamera();

	virtual std::string execute(ska::ScriptComponent& script, std::vector<std::string>& args) override;
	virtual int argumentsNumber();
};
