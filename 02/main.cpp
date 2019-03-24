#include <iostream>
#include <regex>
#include <string>
#include <stdint.h>


int64_t calc(std::string str)
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


int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "error" << '\n';
        return 1;
    }

    //REGEX for "--" -> "+", "+-" -> "-"
    std::string str(argv[1]);
    str.erase(std::remove(str.begin(),str.end(),' '),str.end());
    str = std::regex_replace(str, std::regex("--"), std::string("+"));
    str = std::regex_replace(str, std::regex("\\+-"), std::string("-"));

    //Check division by zero
    if (str.find(std::string("/0")) != std::string::npos) {
        std::cout << "error" << '\n';
        return 1;
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
                std::cout << "error" << '\n';
                return 1;
            }
    }
    if(c_num != c_op + 1)
    {
        std::cout << "error" << '\n';
        return 1;
    }

    std::cout << calc(str) << std::endl;
    return 0;
}