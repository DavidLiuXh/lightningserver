#ifndef LUTIL_STRUTIL_HXX
#define LUTIL_STRUTIL_HXX

#include <string>
//----------------------------------------
namespace LUtil
{
    class StrUtil
    {
        public:
            static bool notEmptyOrNull(const char*);
            static const char* getSafeStr(const char*);

        public:
            static std::string sEmpty;
    };//class StrUtil
}//namespace LUtil
#endif//#ifndef LUTIL_STRUTIL_HXX
