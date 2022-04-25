#pragma once
#include <utils/concurrency.hpp>

namespace game::scripting
{
	class context;

	class task_handle
	{
	public:
		std::uint64_t id = 0;
	};

	class task final : public task_handle
	{
	public:
		std::chrono::steady_clock::time_point last_execution{};
		std::function<void()> callback{};
		std::chrono::milliseconds delay{};
		bool is_volatile = false;
	};

	class scheduler final
	{
	public:
		explicit scheduler(context* context);

		void run_frame();
		void clear();

	private:
		context* context_;

		using task_list = std::vector<task>;
		utils::concurrency::container<task_list> tasks_;
		std::atomic_int64_t current_task_id_ = 0;

		task_handle add(const std::function<void()>& callback, long long milliseconds, bool is_volatile);
		task_handle add(const std::function<void()>& callback, std::chrono::milliseconds delay, bool is_volatile);

		void remove(const task_handle& handle);
	};
}
