#include "nkit/test.h"
#include "nkit/logger_brief.h"
#include "nkit/dynamic/dynamic_builder.h"
#include "nkit/dynamic_xml.h"

namespace nkit_test
{
  using namespace nkit;

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_wrong_xml)
  {
    Dynamic mapping = DLIST(
        "/person" << DLIST("/*/city" << "string"));

    std::string error;
    Dynamic var = DynamicFromXml("xml", mapping, &error);
    NKIT_TEST_ASSERT(!var && !error.empty());
  }

  //---------------------------------------------------------------------------
  _NKIT_TEST_CASE(xml2var_star_pref_test)
  {
    std::string error;
    std::string xml_path("../../data/commerce.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("../../data/commerce1.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    TimeMeter tm;
    tm.Start();
    Dynamic var = DynamicFromXml(xml, mapping, &error);
    tm.Stop();
    CINFO(tm.GetTotal());
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);
    //CINFO(nkit::json_hr << var);

    mapping = "";
    mapping_path = "../../data/commerce2.json";
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    tm.Clear();
    tm.Start();
    var = DynamicFromXml(xml, mapping, &error);
    tm.Stop();
    CINFO(tm.GetTotal());
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);
    //CINFO(nkit::json_hr << var);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_list_of_lists)
  {
    //CINFO(__FILE__);
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    Dynamic mapping = DLIST(
        "/person" << DLIST("/phone" << "string"));

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
           DLIST("+122233344550" << "+122233344551")
        << DLIST("+122233344553" << "+122233344554"));

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_list)
  {
    //CINFO(__FILE__);
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    Dynamic mapping = DLIST("/person/phone" << "string");

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
           "+122233344550" << "+122233344551"
        << "+122233344553" << "+122233344554");

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_list_of_lists_with_mask)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    Dynamic mapping = DLIST(
        "/person" << DLIST("/*/city" << "string"));

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    //CINFO(nkit::json_hr << var);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
           DLIST("New York" << "Boston")
        << DLIST("Moscow" << "Tula"));

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_list_of_objects_with_mask)
  {
    //CINFO(__FILE__);
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    Dynamic mapping = //DLIST("/person" << DDICT("/*" << "string") );
        DLIST("/person" << DDICT(
                  "/married/@firstTime" << "boolean" <<
                  "/photos" << DLIST("/*" << "string")
                )
        );

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);
   // CINFO(nkit::json_hr << var);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_list_of_objects_with_list)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/list_of_objects_with_list.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(

        DDICT("birthday" << Dynamic(1979, 3, 28, 12, 13, 14)
            << "phones" << DLIST("+122233344550" << "+122233344551")
            << "firstTime" << false) <<

        DDICT("birthday" << Dynamic(1970, 8, 31, 2, 3, 4)
            << "phones" << DLIST("+122233344553" << "+122233344554")
            << "firstTime" << true)
        );

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_default_values)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/default_values.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    Dynamic var = DynamicFromXml(xml, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
        DDICT("key_for_default_value" << "default_value") <<
        DDICT("key_for_default_value" << "default_value")
        );

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_without_trim)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/persons_with_star.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    std::string options;
    std::string options_path("./data/options_no_trim.json");
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(options_path, &options, &error), error);

    Dynamic var = DynamicFromXml(xml, options, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
        DDICT(
            "name" << "Jack" <<
            "photos" << "\n\t\t\t\n\t\t\t\n\t\t\t\n\t\t" <<
            "age" << "33" <<
            "married" << "Yes" <<
            "phone" << "+122233344551" <<
            "birthday" << "Wed, 28 Mar 1979 12:13:14 +0300" <<
            "address" << "\n\t\t\t\n\t\t\t\n\t\t\t\n\t\t\t\n\t\t"
            ) <<
        DDICT(
            "name" << "Boris" <<
            "photos" << "\n\t\t\t\n\t\t\t\n\t\t" <<
            "age" << "34" <<
            "married" << "Yes" <<
            "phone" << "+122233344554" <<
            "birthday" << "Mon, 31 Aug 1970 02:03:04 +0300" <<
            "address" << "\n\t\t\t\n\t\t\t\n\t\t\t\n\t\t\t\n\t\t"
            )
        );

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_with_trim)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/persons_with_star.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    std::string options;
    std::string options_path("./data/options_trim.json");
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(options_path, &options, &error), error);

    Dynamic var = DynamicFromXml(xml, options, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    Dynamic etalon = DLIST(
        DDICT(
            "name" << "Jack" <<
            "photos" << "" <<
            "age" << "33" <<
            "married" << "Yes" <<
            "phone" << "+122233344551" <<
            "birthday" << "Wed, 28 Mar 1979 12:13:14 +0300" <<
            "address" << ""
            ) <<
        DDICT(
            "name" << "Boris" <<
            "photos" << "" <<
            "age" << "34" <<
            "married" << "Yes" <<
            "phone" << "+122233344554" <<
            "birthday" << "Mon, 31 Aug 1970 02:03:04 +0300" <<
            "address" << ""
            )
        );

    NKIT_TEST_EQ(var, etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_multi_mappings)
  {
    std::string error;
    std::string xml_path("./data/sample.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/multi_mapping.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    std::string options;
    std::string options_path("./data/options_trim.json");
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(options_path, &options, &error), error);

    Xml2VarBuilder<DynamicBuilder>::Ptr builder = Xml2VarBuilder<
        DynamicBuilder>::Create(options, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(builder, error);
    NKIT_TEST_ASSERT_WITH_TEXT(
        builder->Feed(xml.c_str(), xml.length(), true, &error), error);

    Dynamic academy = builder->var("academy mapping");
    Dynamic persons = builder->var("persons mapping");

    Dynamic academy_etalon = DDICT(
              "link" << "http://www.damsdelhi.com/dams.php"
          <<  "title" << "Delhi Academy Of Medical Sciences"
        );

    Dynamic persons_etalon = DLIST(
        DDICT(
            "name" << "Jack" <<
            "photos" << "" <<
            "age" << "33" <<
            "married" << "Yes" <<
            "phone" << "+122233344551" <<
            "birthday" << "Wed, 28 Mar 1979 12:13:14 +0300" <<
            "address" << ""
            ) <<
        DDICT(
            "name" << "Boris" <<
            "photos" << "" <<
            "age" << "34" <<
            "married" << "Yes" <<
            "phone" << "+122233344554" <<
            "birthday" << "Mon, 31 Aug 1970 02:03:04 +0300" <<
            "address" << ""
            )
        );

    NKIT_TEST_EQ(academy, academy_etalon);
    NKIT_TEST_EQ(persons, persons_etalon);
  }

  //---------------------------------------------------------------------------
  NKIT_TEST_CASE(xml2var_sandbox)
  {
    std::string error;
    std::string xml_path("./data/sample1251.xml");
    std::string xml;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(xml_path, &xml, &error), error);

    std::string mapping_path("./data/academi_mapping.json");
    std::string mapping;
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(mapping_path, &mapping, &error), error);

    std::string options;
    std::string options_path("./data/options_attrkey.json");
    NKIT_TEST_ASSERT_WITH_TEXT(
        nkit::text_file_to_string(options_path, &options, &error), error);

    Dynamic var = DynamicFromXml(xml, options, mapping, &error);
    NKIT_TEST_ASSERT_WITH_TEXT(var, error);

    CINFO(nkit::json_hr << var);
  }

} // namespace nkit_test

