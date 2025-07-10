#pragma once

#include "core/logic/Inputer.h"

#include <memory>
#include <string>

namespace Chess
{
    class LabelShower
    {
    public:
        LabelShower(const std::shared_ptr<Inputer>& inputer);
        ~LabelShower();

        static void Show(const std::string& toPrint);
    };
} // namespace Chess
