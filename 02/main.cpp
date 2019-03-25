#include <iostream>
#include <regex>
#include <string>
#include <stdint.h>


int64_t calc(const std::string &str)
{

    int ind_mul_div = 0;
    bool flag = true;

    for(int i = str.length() - 1; i > 0; i--)
    {
        if(str[i] == '-')
            return calc(str.substr(0, i)) - calc(str.substr(i + 1, str.length()));
        if(str[i] == '+')
            return calc(str.substr(0, i)) + calc(str.substr(i + 1, str.length()));
        if(((str[i] == '*') || (str[i] == '/')) && flag)
        {
            ind_mul_div = i;
            flag = false;
        }
    }

    //Return digit if it's digit))0)
    if(ind_mul_div == 0)
    {
        if(str[0] == '-')
            return -1 * int64_t(std::stoi(str.substr(1, str.length())));
        else
            return int64_t(std::stoi(str));
    }

    //First division or multiply, if we not found "-" or "+"
    if(str[ind_mul_div] == '*') 
        return calc(str.substr(0, ind_mul_div)) * calc(str.substr(ind_mul_div + 1, str.length()));
    else
        return calc(str.substr(0, ind_mul_div)) / calc(str.substr(ind_mul_div + 1, str.length()));
}

bool check(const std::string &str)
{
    //Check division by zero
    if (str.find("/0") != std::string::npos) {
        return true;
    }

    //Now check the (number of digits) == (the number of operations + 1), but str[0] can be == '-'
    int c_num = 0, c_op = 0;
    bool flag = false;
    if(str[0] == '-')
        c_op--;

    for(int i = 0; i < str.length(); i++)
    {
        if(std::isdigit(str[i]))
        {
            flag = true;
            if(i == str.length() - 1)
                c_num++;
        }
        else
            if(str[i] == '-' || str[i] == '+' || str[i] == '*' || str[i] == '/')
            {
                c_op++;
                if(flag)
                {
                    c_num++;
                    flag = false;
                }
            }
            else
            {
                return true;
            }
    }
    if(c_num != c_op + 1)
    {
        return true;
    }
    return false;
}

bool transform(std::string &str)
{
    //REGEX for "--" -> "+", "+-" -> "-"
    str.erase(std::remove(str.begin(),str.end(),' '),str.end());
    str = std::regex_replace(str, std::regex("--"), "+");
    str = std::regex_replace(str, std::regex("\\+-"), "-");

    return check(str);
}

int main(int argc, char* argv[])
{
    if(argc == 2)
    {
        std::string str(argv[1]);
        if( !transform(str))
        {
            std::cout << calc(str) << std::endl;
            return 0;
        }
    }
    std::cout << "error" << std::endl;
    return 1;
}