/*
 * LuaScriptModule.cpp, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */
#include "StdInc.h"

#include "LuaScriptModule.h"
#include "LuaScriptingContext.h"
#include "LuaSpellEffect.h"

const char *g_cszAiName = "Lua interpreter";

extern "C" DLL_EXPORT void GetAiName(char * name)
{
	strcpy_s(name, strlen(g_cszAiName) + 1, g_cszAiName);
}

extern "C" DLL_EXPORT void GetNewModule(std::shared_ptr<scripting::Module> & out)
{
	out = std::make_shared<scripting::LuaScriptModule>();
}

namespace scripting
{

LuaScriptModule::LuaScriptModule() = default;
LuaScriptModule::~LuaScriptModule() = default;

std::string LuaScriptModule::compile(const std::string & name, const std::string & source) const
{
	//TODO: pre-compile to byte code
	return source;
}

std::shared_ptr<ContextBase> LuaScriptModule::createContextFor(const Script * source, const scripting::Environment * env) const
{
	auto ret = std::make_shared<LuaContext>(env->logger(), source);
	ret->init(env->game(), env->battle());
	return ret;
}

void LuaScriptModule::registerSpellEffect(spells::effects::Registry * registry, const Script * source) const
{
	registry->add(source->getName(), std::make_shared<spells::effects::LuaSpellEffectFactory>(source));
}


}
