#include "singleton.h"

#include <string>

#include <iostream>

using namespace cmn;

class Printer : public Singleton<Printer>
{
	public:
		void Print()
		{
			std::cout << "print in " << __PRETTY_FUNCTION__ << '\n';	
		}

	private:
		uint32_t m_count = 0;

		friend class Singleton<Printer>;
		Printer(){ std::cout << "pod_count=" << ++m_count << '\n';}
};

class FileSystem : public Singleton<FileSystem, false>
{
	public:
		void Read()
		{
			std::cout << "read " << m_file_name << " from " << __PRETTY_FUNCTION__ << '\n';
		}

		void Write()
		{
			std::cout << "write " << m_file_name << " from " << __PRETTY_FUNCTION__ << '\n';
		}

	private:
		friend class Singleton<FileSystem, false>;
		FileSystem( std::string file_name) : m_file_name( file_name){}

		std::string m_file_name;
};


int main()
{
	auto& printer = Printer::Instance();
	printer.Print();
	auto& s_printer = Singleton<Printer>::Instance();  
	s_printer.Print();

	auto& fs_initor = FileSystem::Instance("customized_fs");
	fs_initor.Read();
	fs_initor.Write();

	auto& fs_initor2 = FileSystem::Instance("fs2");  // fs can only inited once, so this won't trigger initialization again
	fs_initor2.Read();
	fs_initor2.Write();

	auto& fs = FileSystem::Instance(); //Singleton<FileSystem>::Instance() also works 	
	fs.Read();
	fs.Write();

	return 0;
}
