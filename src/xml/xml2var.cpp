#include "nkit/xml2var.h"

namespace nkit
{
  namespace detail
  {
    const bool Options::TRIM_DEFAULT = false;
    const bool Options::UNICODE_DEFAULT = true;
  }
  //----------------------------------------------------------------------------
  std::ostream & operator <<(std::ostream & stream, const Path & path)
  {
    std::vector<size_t>::const_iterator element_id = path.elements().begin(),
        end = path.elements().end();
    for (; element_id != end; ++element_id)
    {
      stream << *element_id << " ";
    }

    return stream;
  }

  //----------------------------------------------------------------------------
  const char * find_attribute_value(const char ** attrs,
      const char * attribute_name)
  {

    for (size_t i = 0; attrs[i] && attrs[i + 1]; ++(++i))
    {
      if (strcmp(attrs[i], attribute_name) == 0)
        return attrs[i + 1];
    }

    return NULL;
  }

} // namespace nkit