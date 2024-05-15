#include <exception>
#include <string>

class InvalidFormatException : public std::exception {
private:
    std::string message;
public:
    explicit InvalidFormatException(const std::string& str)
    {
        message = str;
    }

    explicit InvalidFormatException(const char* str)
            : message(str) {
    }

    const char * what () const noexcept override {
        return message.c_str();
    }
};