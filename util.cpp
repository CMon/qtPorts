#include "util.h"

namespace Util {

QString boolToString (bool value)
{
    if (value) return "yes";
    else       return "no";
}

}
