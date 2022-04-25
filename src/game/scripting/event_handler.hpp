#pragma once
#include <utils/concurrency.hpp>
#include "entity.hpp"
#include "event.hpp"

namespace game::scripting
{
	class context;

	class event_listener_handle
	{
	public:
		unsigned long long id = 0;
	};

	class event_listener final : public event_listener_handle
	{
	public:
		std::string event = {};
		unsigned int entity_id = 0;
		std::function<void(std::vector<chaiscript::Boxed_Value>)> callback = {};
		bool is_volatile = false;
	};

	class generic_event_listener final : public event_listener_handle
	{
	public:
		std::string event = {};
		std::function<void(entity, std::vector<chaiscript::Boxed_Value>)> callback = {};
		bool is_volatile = false;
	};

	class event_handler final
	{
	public:
		explicit event_handler(context* context);

		void dispatch(event* event);

		event_listener_handle add_event_listener(event_listener listener);
		event_listener_handle add_event_listener(generic_event_listener listener);

		void clear();

	private:
		context* context_;
		std::atomic_int64_t current_listener_id_ = 0;

		using task_list = std::vector<event_listener>;
		utils::concurrency::container<task_list> event_listeners_;

		using generic_task_list = std::vector<generic_event_listener>;
		utils::concurrency::container<generic_task_list> generic_event_listeners_;

		void dispatch_to_specific_listeners(event* event, const std::vector<chaiscript::Boxed_Value>& arguments);
		void dispatch_to_generic_listeners(event* event, const std::vector<chaiscript::Boxed_Value>& arguments);

		void remove(const event_listener_handle& handle);
	};
}
