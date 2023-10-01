#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain
{
public:
    explicit Domain(const string& domain)
    {
        //Если приписать к началу всех доменов точку, домен обязан быть суффиксом поддомена.
        std::string full_domain_name = "." + domain;
        //записываю в обратном порадке, чтобы было проще проверять поддомены (по префиксу, а не по суффиксу)
        std::reverse(full_domain_name.begin(), full_domain_name.end());
        domain_ = full_domain_name;
    }

    bool operator==(const Domain& another) const
    {
        return std::lexicographical_compare(domain_.begin(), domain_.end(),
            another.domain_.begin(), another.domain_.end());
    }

    //метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
    bool IsSubdomain(const Domain& another) const
    {
        return std::equal(another.domain_.begin(),
            another.domain_.end(),
            domain_.begin());
    }

    const std::string& GetDomain() const
    {
        return domain_;
    }

private:
    string domain_;
};

class DomainChecker
{
public:
    template<typename It>
    DomainChecker(const It begin, const It end)
    {

        forbidden_domains_.reserve(distance(begin, end));
        std::vector<Domain> domains(begin, end);

        std::sort(domains.begin(), domains.end(), [](const Domain& lhs, const Domain& rhs)
            {
                return lhs == rhs;
            });

        It last = std::unique(domains.begin(), domains.end(),
            [](const Domain& lhs, const Domain& rhs)
            {
                return rhs.IsSubdomain(lhs);
            });

        domains.erase(last, domains.end());
        forbidden_domains_ = domains;
    }

    bool IsForbidden(const Domain& domain_to_check)
    {
        if (!forbidden_domains_.empty())
        {
            auto upper = std::upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(),
                domain_to_check, [](const Domain& lhs, const Domain& rhs)
                {
                    return lhs.GetDomain() < rhs.GetDomain();
                });

            if (forbidden_domains_.begin() == upper)
            {
                return domain_to_check.IsSubdomain(forbidden_domains_[upper - forbidden_domains_.begin()]);
            }
            else
            {
                return domain_to_check.IsSubdomain(forbidden_domains_[upper - forbidden_domains_.begin() - 1]);
            }
        }
        return false;
    }

private:
    std::vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
std::vector<Domain> ReadDomains(std::istream& input, size_t amount)
{
    std::vector<Domain> domains;
    std::string domain;
    while (amount--)
    {
        getline(input, domain);
        domains.emplace_back(domain);
    }
    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input)
{
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}


void SubdomainTest()
{
    std::string str1 = "gdz.ru";
    std::string str2 = "math.gdz.ru";
    std::string str3 = "history.gdz.ru";
    std::string str4 = "gdz.ua";
    std::string str5 = "freegdz.ru";

    Domain domain1(str1);
    Domain domain2(str2);
    Domain domain3(str3);
    Domain domain4(str4);
    Domain domain5(str5);


    assert(!(domain1 == domain4));
    assert(domain2.IsSubdomain(domain1));
    assert(domain3.IsSubdomain(domain1));
    assert(!(domain1.IsSubdomain(domain2)));
    assert(!(domain1.IsSubdomain(domain3)));
    assert(domain1 == domain5);
    assert(!(domain1.IsSubdomain(domain5)));

    std::cout << "SubDomainTest complete" << std::endl;
}


void IsForbiddenTest()
{
    std::string str1 = "gdz.ru";
    std::string str2 = "maps.me";
    std::string str3 = "m.gdz.ru";
    std::string str4 = "com";

    Domain forbiden_domain1(str1);
    Domain forbiden_domain2(str2);
    Domain forbiden_domain3(str3);
    Domain forbiden_domain4(str4);

    std::vector<Domain> forbidden_domains;
    forbidden_domains.push_back(forbiden_domain1);
    forbidden_domains.push_back(forbiden_domain2);
    forbidden_domains.push_back(forbiden_domain3);
    forbidden_domains.push_back(forbiden_domain4);

    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    {
        std::string str5 = "gdz.ua";
        Domain unforbiden_domain1(str5);
        assert(!(checker.IsForbidden(unforbiden_domain1)));
    }

    {
        assert(checker.IsForbidden(forbiden_domain1));
    }
    std::cout << "IsForbiddenTest complete" << std::endl;
}

int main()
{
    // SubdomainTest();
    // IsForbiddenTest();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains)
    {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
