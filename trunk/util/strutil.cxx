#include "strutil.hxx"

using namespace LUtil;
//------------------------------------------------------
std::string StrUtil::sEmpty = "";
//------------------------------------------------------
bool StrUtil::notEmptyOrNull(const char* data) 
{
    return (data != NULL && 0 != data[0]);
}

const char* StrUtil::getSafeStr(const char* data)
{
    return (NULL != data ? data : sEmpty.c_str());
}
//------------------------------------------------------
