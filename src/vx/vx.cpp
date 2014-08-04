#include "nkit/vx.h"

namespace nkit
{

  //----------------------------------------------------------------------------
  std::ostream & operator <<(std::ostream & stream, const Path & path)
  {
    BOOST_FOREACH(size_t element_id, path.elements_)
    {
      stream << element_id << " ";
    }

    return stream;
  }

  //----------------------------------------------------------------------------
  const char * find_attribute_value(const char ** attrs,
      const char * attribute_name)
  {

    for (size_t i = 0; attrs[i] && attrs[i + 1]; ++i)
    {
      if (strcmp(attrs[i], attribute_name) == 0)
        return attrs[i + 1];
    }

    return NULL;
  }

} // namespace nkit
