//создаем класс CityBuilder, чтобы иметь возможность строить объект класса City с помощью сеттеров
class CityBuilder
{
    CityBuilder& SetCityName(std::string city_name)
    {
        city_name_ = city_name;
    }

    CityBuilder& SetCityIsoCode(std::string city_iso_code)
    {
        city_iso_code_ = city_iso_code;
    }

    CityBuilder& SetPhoneCode(std::string phone_code)
    {
        phone_code_ = phone_code;
    }

    CityBuilder& SetCountry(Country country)
    {
        country_ = move(country);
    }

    //оператор неявного преобразования
    operator City() const
    {
        return { city_name_, city_iso_code_, phone_code_, country_};
    }

private:
    std::string city_name_;
    std::string city_iso_code_;
    std::string phone_code_;
    Country country_;
};


// В ParseCitySubjson теперь передается структура Country (она уже изначально использовалась в ParseCountryJson),
//а не пять отдельных параметров
void ParseCitySubjson(vector<City>& cities, const Json& json, Country& country)
{
    for (const auto& city_json : json.AsList())
    {
        const auto& city_obj = city_json.AsObject();

        //теперь обьект city конструируется вызовом сеттеров
        City city = CityBuilder().
                    SetCityName(city_obj["name"s].AsString()).
                    SetCityIsoCode(city_obj["iso_code"s].AsString()).
                    SetPhoneCode(country.phone_code + city_obj["phone_code"s].AsString()).
                    SetCountry(country);

        cities.push_back(city);
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json)
{
    for (const auto& country_json : json.AsList())
    {
        const auto& country_obj = country_json.AsObject();
        countries.push_back(
        {
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();

        for (const auto& lang_obj : country_obj["languages"s].AsList())
        {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        //теперь передается структура Country, а не пять отдельных параметров
        ParseCitySubjson(cities, country_obj["cities"s], country);
    }
}
