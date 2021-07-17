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

    template<typename r, typename c, typename... args>
    std::function<r (args... )> bind(c* instance, r (c::*f) (args...)){
        return [=](args... as) {
            return (*instance.*f)(as...);
        };
    }
}