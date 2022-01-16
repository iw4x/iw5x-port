#pragma once
#include "game/game.hpp"

namespace game::scripting
{
	class variable_value
	{
	public:
		variable_value() = default;
		variable_value(const native::VariableValue& value);
		variable_value(const variable_value& other) noexcept;
		variable_value(variable_value&& other) noexcept;

		variable_value& operator=(const variable_value& other) noexcept;
		variable_value& operator=(variable_value&& other) noexcept;

		~variable_value();

		const native::VariableValue& get() const;

	private:
		void assign(const native::VariableValue& value);
		void release();

		native::VariableValue value_{{0}, native::SCRIPT_NONE};
	};
}
