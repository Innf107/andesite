#include "Util.h"
#include <string>
#include <functional>
#include <sstream>

namespace Util {
using namespace std;

    string message(function<void (ostringstream&)> cb){
        std::ostringstream msg;
        cb(msg);
        return msg.str();
    }
}
