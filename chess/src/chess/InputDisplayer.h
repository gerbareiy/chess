#pragma once

#include "InputHandler.h"

#include <string>

namespace Chess
{
	class InputDisplayer
	{
	public:
		InputDisplayer(const std::shared_ptr<InputHandler>& handler);

	private:
		void Show(std::string toPrint) const;
	};
}
