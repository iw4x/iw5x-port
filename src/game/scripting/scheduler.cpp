#include <std_include.hpp>

#include "context.hpp"

namespace game::scripting
{
	scheduler::scheduler(context* context) : context_(context)
	{
		const auto chai = this->context_->get_chai();

		chai->add(chaiscript::user_type<task_handle>(), "_task_handle");
		chai->add(chaiscript::constructor<task_handle()>(), "_task_handle");
		chai->add(chaiscript::constructor<task_handle(const task_handle&)>(), "_task_handle");

		chai->add(chaiscript::fun([](task_handle& lhs, const task_handle& rhs) -> task_handle&
		{
			return lhs = rhs;
		}), "=");

		chai->add(chaiscript::fun(
			[this](const std::function<void()>& callback, const long long milliseconds) -> task_handle
			{
				return this->add(callback, milliseconds, true);
			}), "setTimeout");

		chai->add(chaiscript::fun(
			[this](const std::function<void()>& callback, const long long milliseconds) -> task_handle
			{
				return this->add(callback, milliseconds, false);
			}), "setInterval");

		const auto clear = [this](const task_handle& handle)
		{
			this->remove(handle);
		};

		chai->add(chaiscript::fun(clear), "clear");
		chai->add(chaiscript::fun(clear), "clearTimeout");
		chai->add(chaiscript::fun(clear), "clearInterval");
	}

	void scheduler::run_frame()
	{
		this->tasks_.access([&](task_list& tasks)
		{
			for (auto i = tasks.begin(); i != tasks.end();)
			{
				const auto now = std::chrono::steady_clock::now();
				const auto diff = now - i->last_execution;

				if (diff < i->delay)
				{
					++i;
					continue;
				}

				i->last_execution = now;

				if (i->is_volatile)
				{
					i = tasks.erase(i);
				}
				else
				{
					i->callback();
					++i;
				}
			}
		});
	}

	void scheduler::clear()
	{
		this->tasks_.access([&](task_list& tasks)
		{
			tasks.clear();
		});
	}

	task_handle scheduler::add(const std::function<void()>& callback, const long long milliseconds,
	                           const bool is_volatile)
	{
		return this->add(callback, std::chrono::milliseconds(milliseconds), is_volatile);
	}

	task_handle scheduler::add(const std::function<void()>& callback, const std::chrono::milliseconds delay,
	                           const bool is_volatile)
	{
		task task;
		task.is_volatile = is_volatile;
		task.callback = callback;
		task.delay = delay;
		task.last_execution = std::chrono::steady_clock::now();
		task.id = ++this->current_task_id_;

		this->tasks_.access([&task](task_list& tasks)
		{
			tasks.emplace_back(std::move(task));
		});

		return {task.id};
	}

	void scheduler::remove(const task_handle& handle)
	{
		this->tasks_.access([&](task_list& tasks)
		{
			for (auto i = tasks.begin(); i != tasks.end();)
			{
				if (i->id == handle.id)
				{
					i = tasks.erase(i);
					break;
				}

				++i;
			}
		});
	}
}
