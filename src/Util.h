#pragma once
#include <optional>

namespace Util {
    template<typename a>
    std::optional<a> when(bool cond, a value){
        if(cond)
            return value;
        else 
            return{};
    }
}