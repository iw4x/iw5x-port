#include "std_include.hpp"
#include "context_initializer.hpp"

#include "utils/string.hpp"

namespace game::scripting::context_initializer
{
	void initialize_entity(context* context)
	{
		auto* const chai = context->get_chai();

		chai->add(chaiscript::user_type<entity>(), "_entity");
		chai->add(chaiscript::constructor<entity()>(), "_entity");
		chai->add(chaiscript::constructor<entity(const entity&)>(), "_entity");

		chai->add(chaiscript::fun([](entity& lhs, const entity& rhs) -> entity&
		{
			return lhs = rhs;
		}), "=");

		chai->add(chaiscript::fun(&entity::get), "get");
		chai->add(chaiscript::fun(&entity::set), "set");

		chai->add(chaiscript::fun(&entity::on_notify), "onNotify");
		chai->add(chaiscript::fun([](const entity& ent, const std::string& event,
		                             const std::function<void(
			                             std::vector<chaiscript::Boxed_Value>)>&
		                             callback)
		{
			return ent.on_notify(event, callback, false);
		}), "onNotify");

		chai->add(chaiscript::fun([context](const std::string& event,
		                                    const std::function<void(
			                                    entity, std::vector<chaiscript::Boxed_Value>)>&
		                                    callback)
		{
			generic_event_listener listener;
			listener.event = event;
			listener.is_volatile = false;
			listener.callback = callback;

			return context->get_event_handler()->add_event_listener(listener);
		}), "onNotify");

		chai->add(chaiscript::fun([context](const std::string& event,
		                                    const std::function<void(
			                                    entity, std::vector<chaiscript::Boxed_Value>)>&
		                                    callback, const bool is_volatile)
		{
			generic_event_listener listener;
			listener.event = event;
			listener.is_volatile = is_volatile;
			listener.callback = callback;

			return context->get_event_handler()->add_event_listener(listener);
		}), "onNotify");

		chai->add_global(chaiscript::Boxed_Value(0), "gsc");

		chai->add(chaiscript::fun([context](const entity& entity, const std::string& function,
		                                    const std::vector<chaiscript::Boxed_Value>& arguments)
		{
			const auto function_lower = utils::string::to_lower(function);

			if (function_lower == "notify" && !arguments.empty())
			{
				const auto real_arguments = std::vector<chaiscript::Boxed_Value>(
					arguments.begin() + 1, arguments.end());

				entity.notify(chaiscript::boxed_cast<std::string>(arguments[0]), real_arguments);
			}
			else if (context->get_executer()->function_exists(function_lower, false))
			{
				return entity.call(function_lower, arguments);
			}

			return chaiscript::Boxed_Value(0);
		}), "method_missing");

		chai->add(chaiscript::fun([context](const chaiscript::Boxed_Value&/*object*/,
		                                    const std::string& function,
		                                    const std::vector<chaiscript::Boxed_Value>& arguments)
		{
			const auto function_lower = utils::string::to_lower(function);

			if (context->get_executer()->function_exists(function_lower, true))
			{
				return context->get_executer()->call(function_lower, 0, arguments);
			}

			return chaiscript::Boxed_Value(0);
		}), "method_missing");
	}

	void initialize(context* context)
	{
		initialize_entity(context);

		auto* const chai = context->get_chai();

		chai->add(chaiscript::fun([](const std::string& string)
		{
			printf("%s\n", string.data());
		}), "print");

		chai->add(chaiscript::fun([](const std::string& string)
		{
			MessageBoxA(nullptr, string.data(), nullptr, 0);
		}), "alert");

		const auto level_id = *native::levelEntityId;
		chai->add_global(chaiscript::var(entity(context, level_id)), "level");
	}
}
