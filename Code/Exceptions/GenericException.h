#pragma once
#include <exception>
#include <string>

class GenericException : public std::exception
{
public:
	GenericException(std::string message, std::string type) throw();
	const char* what() const throw();
	virtual ~GenericException() throw();

private:
	std::string m_message, m_typedMessage;
};
