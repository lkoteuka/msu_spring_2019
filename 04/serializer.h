#pragma once
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};


class Serializer
{
private:
    std::ostream &out_;
    static constexpr char Separator = ' ';

    Error write()
    {
        return Error::NoError;
    }

    Error write(const uint64_t value)
    {
        out_ << value << Separator;
        return Error::NoError;
    }

    Error write(const bool value)
    {
        value ? out_ << "true" : out_ <<"false ";
        out_ << Separator;
        return Error::NoError;
    }

    template <typename T>
    Error write(const T& value)
    {
        return Error::CorruptedArchive;
    }

    template <typename T, typename... Args>
    Error process(T &&value, Args&&... args)
    {
        if(write(value) == Error::NoError && process(std::forward<Args>(args)...) == Error::NoError)
        {
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }
    
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <typename T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <typename... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }
    
};


class Deserializer
{
private:
    std::istream &in_;
    static constexpr char Separator = ' ';

    Error read()
    {
        return Error::NoError;
    }

    Error read(uint64_t& value)
    {
        std::string val;
        in_ >> val;

        if(val[0] == '-')
            return Error::CorruptedArchive;
        try 
        {
            value = std::stoull(val);
        } 
        catch (const std::invalid_argument &ex)
        {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error read(bool& value)
    {
        std::string val;
        in_ >> val;
        if(val == "false")
            value = false;
        else if(val == "true")
            value = true;
        else 
            return Error::CorruptedArchive;
        
        return Error::NoError;
    }

    template <typename T>
    Error read(T& value)
    {
        return Error::CorruptedArchive;
    }

    template <typename T, typename... Args>
    Error process(T &&value, Args&&... args)
    {
        if(read(value) == Error::NoError && process(std::forward<Args>(args)...) == Error::NoError)
        {
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }

    Error process()
    {
        return Error::NoError;
    }

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <typename T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <typename... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }
};
