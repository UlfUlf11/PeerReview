//создаем структуру для части параметров функции LoadPersons
struct Db_prorerties
{
    std::string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
};

//вывод сообщения выносим из функции LoadPersons в отдельную функцию
std::string InfoMesage(const DBHandler& db, int min_age, int max_age, std::string_view name_filter)
{
    std::ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    return query_str.str();
}


vector<Person> LoadPersons(const Db_prorerties& db_properties, int min_age, int max_age, string_view name_filter)
{

    DBConnector connector((db_properties.db_allow_exceptions, db_properties.db_log_level);
                          DBHandler db;

                          if (db_properties.db_name.starts_with("tmp."s))
{
    db = connector.ConnectTmp(db_properties.db_name, db_properties.db_connection_timeout);
    }
    else
    {
        db = connector.Connect(db_properties.db_name, db_properties.db_connection_timeout);
    }
    if (!db_properties.db_allow_exceptions && !db.IsOK())
{
    return {};
}

DBQuery query(InfoMesage(db, min_age, max_age, name_filter));

vector<Person> persons;
for (auto [name, age] : db.LoadRows<string, int>(query))
{
    persons.push_back({ move(name), age });
    }
    return persons;
}
