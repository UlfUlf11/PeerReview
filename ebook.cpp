#include <iostream>
#include <sstream>
#include <vector>


class ReadingManager
{
public:

    ReadingManager() : users_count_for_page(1000, 0),
        pages_to_user_id_(100001, 0) {}

    void Read(int user_id, int pages)
    {
        if (pages_to_user_id_.size() < user_id)
        {
            pages_to_user_id_.resize(user_id * 2);
        }

        //сколько страниц было прочитано до этого момента
        int read_pages = 0;

        //если поле хранит не 0, то pages_before придаем значение поля
        if (pages_to_user_id_[user_id])
        {
            read_pages = pages_to_user_id_[user_id];
        }
        else
        {
            ++amount_of_users_;
        }

        //сколько прочитано теперь
        pages_to_user_id_[user_id] = pages;

        for (int i = read_pages; i < pages; ++i)
        {
            //колличество пользователей дочитавших до страницы
            users_count_for_page[i] += 1;
        }
    }

    double Cheer(int user_id)
    {
        //если пользователя нет или он не прочитал ни одной страницы...
        if (!pages_to_user_id_[user_id])
        {
            return 0.0;
            //если читатель у книги всего один...
        }
        else if (amount_of_users_ == 1)
        {
            return 1.0;
        }
        else
        {
            //до какой страницы дочитал пользователь с id...
            int page_number = pages_to_user_id_[user_id];
            //если все пользователи дочитали до этой страницы...
            if (amount_of_users_ == users_count_for_page[page_number - 1])
            {
                return 0.0;
            }
            //...иначе...
            else
            {
                //...из общего кол-ва пользователей вычитаем тех, кто дочитал до этой же страницы,
                //и делим на всех других пользователей, кроме нашего
                return (amount_of_users_ - users_count_for_page[page_number - 1]) / (amount_of_users_ * 1.0 - 1.0);
            }
        }
    }

    void Output(std::ostream& output)
    {
        std::string command;

        for (const std::string request : requests_)
        {
            std::istringstream ss(request);
            ss >> command;
            if (command == "READ")
            {
                int user_id;
                int page;
                ss >> user_id >> page;
                Read(user_id, page);
            }
            else if (command == "CHEER")
            {
                int user_id;
                ss >> user_id;
                output << Cheer(user_id) << std::endl;
            }
        }
    }

    void Input(std::istream& input)
    {
        std::string str;
        getline(input, str);

        int requests_amount = std::stoi(str);
        std::vector<std::string> requests;
        requests_.reserve(requests_amount);

        for (int i = 0; i < requests_amount; ++i)
        {
            std::string request;
            getline(input, request);
            requests_.push_back(request);
        }
    }

private:
    //кол-во людей, дочитавших как минимум до страницы №...
    std::vector<std::uint32_t> users_count_for_page;
    //до какой страницы дочитал пользователь с id...
    std::vector<std::int16_t> pages_to_user_id_;
    //счетчик кол-ва читателей
    std::uint32_t amount_of_users_ = 0;
    //вектор запросов, вызывается командой Output
    std::vector<std::string> requests_;
};


int main()
{
    ReadingManager manager;
    manager.Input(std::cin);
    manager.Output(std::cout);
}
