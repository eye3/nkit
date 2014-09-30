/*
   Copyright 2010-2014 Boris T. Darchiev (boris.darchiev@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "nkit/vx.h"
#include "nkit/dynamic/dynamic_builder.h"
#include "nkit/dynamic_xml.h"

namespace nkit
{
  Dynamic DynamicFromXml(const std::string & xml,
      const std::string & options,
      const std::string & mapping,
      std::string * const error)
  {
    Xml2VarBuilder<DynamicBuilder>::Ptr builder = Xml2VarBuilder<
        DynamicBuilder>::Create(options, error);
    if(!builder)
      return Dynamic();
    if (!builder->AddMapping(mapping, error))
      return Dynamic();
    if (!builder->Feed(xml.c_str(), xml.length(), true, error))
      return Dynamic();
    return builder->var();
  }

  Dynamic DynamicFromXml(const std::string & xml,
      const std::string & mapping,
      std::string * const error)
  {
    return DynamicFromXml(xml, "{}", mapping, error);
  }

  Dynamic DynamicFromXml(const std::string & xml,
      const Dynamic & mapping,
      std::string * const error)
  {
    return DynamicFromXml(xml, D_NONE, mapping, error);
  }

  Dynamic DynamicFromXml(const std::string & xml,
      const Dynamic & options,
      const Dynamic & mapping,
      std::string * const error)
  {
    Xml2VarBuilder<DynamicBuilder>::Ptr builder = Xml2VarBuilder<
        DynamicBuilder>::Create(options, error);
    if(!builder)
      return Dynamic();
    if (!builder->AddMapping(mapping, error))
      return Dynamic();
    if (!builder->Feed(xml.c_str(), xml.length(), true, error))
      return Dynamic();
    return builder->var();
  }

  Dynamic DynamicFromXmlFile(const std::string & path,
        const std::string & options,
        const std::string & mapping,
        std::string * const error)
  {
    std::string xml;
    if (!path.empty() && !text_file_to_string(path, &xml))
    {
      *error = "Could not open file: '" + path + "'";
      return Dynamic();
    }

    if (xml.empty())
    {
      *error = "Could not open file: '" + path + "'";
      return Dynamic();
    }

    return DynamicFromXml(xml, options, mapping, error);
  }
} // namespace nkit
