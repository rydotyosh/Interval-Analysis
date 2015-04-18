#ifndef EXCEPTION
#define EXCEPTION

namespace Interval{

	class interval_error
	{
	public:
		interval_error(char* string)
			:mes(std::string(string))
		{}
		interval_error(std::string string)
			:mes(string)
		{}

		const char* what()
		{
			return mes.c_str();
		}
	private:
		std::string mes;

	};
	class interval_except : public interval_error
	{
	public:
		interval_except(std::string& cause)
			:interval_error("Error massage : " + cause)
		{}
		interval_except(const char *cause)
			:interval_error(std::string("Error massage : ") + std::string(cause))
		{}
	};

	class range_error : public interval_except
	{
	public:
		range_error(std::string& cause)
			:interval_except(cause)
		{}
		range_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class domain_error : public interval_except
	{
	public:
		domain_error(std::string& cause)
			:interval_except(cause)
		{}
		domain_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class logic_error : public interval_except
	{
	public:
		logic_error(std::string& cause)
			:interval_except(cause)
		{}
		logic_error(const char *cause)
			:interval_except(cause)
		{}
	};

	class invalid_argument : public interval_except
	{
	public:
		invalid_argument(std::string& cause)
			:interval_except(cause)
		{}
		invalid_argument(const char *cause)
			:interval_except(cause)
		{}
	};




}
#endif