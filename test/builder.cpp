#include <iostream>
#include <memory>

class Phone
{
	private:
		std::string	m_name;
		int			m_id;

	public:
		void SetName( std::string const& name){
			m_name = name;
		}

		std::string GetName() const{
			return m_name;
		}

		void SetId( int const id){
			m_id = id;	
		}

		int GetId() const{
			return m_id;
		}

		class Builder
		{
			public:	
				Builder() : m_phone( new Phone()){}

				std::unique_ptr<Phone>	Build(){
					return std::move(m_phone);
				}

				Builder& Name( std::string const& name)
				{
					m_phone->SetName(name);	
					return *this;
				}

				Builder& Id( int const id)
				{
					m_phone->SetId(id);	
					return *this;
				}

			private:
				std::unique_ptr<Phone>	m_phone;
		};
};

int main()
{
	Phone::Builder builder;
	auto phone = builder.Name("ravine").Id(897).Build();

	std::cout << "name=" << phone->GetName() << "\tid=" << phone->GetId() << '\n';

	return 0;
}
