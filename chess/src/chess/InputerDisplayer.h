#pragma once

#include "logic/Inputer.h"

#include <string>

namespace Chess
{
	class InputerDisplayer
	{
	public:
		InputerDisplayer(const std::shared_ptr<Inputer>& Inputer);

	private:
		void Show(std::string toPrint) const;
	};
}
