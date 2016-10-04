#include<string>
#include<vector>

bool StringItemsGetter(void* data, int index, const char** outText)
{
  *outText = (*(std::vector<std::string>*)data)[index].c_str();

  return true;
}
