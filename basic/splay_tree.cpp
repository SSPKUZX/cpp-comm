#include <iostream>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>

class SplayTree
{
    
};

template< class _Tp >
using sp = std::shared_ptr< _Tp >;

template< class OperandType >
class Command
{
    public:
        Command( sp<OperandType> op ) : m_op( op ){}
        virtual ~Command(){}
        virtual void execute() = 0;
    
    protected:
        sp< OperandType > m_op;
};

template< class OperandType >
class InsertCommand final: public Command< OperandType >
{
    public:
        InsertCommand( sp< OperandType > op, uint32_t id, uint32_t val)
            : Command( op ), m_id( id ), m_val( val ){}
        ~InsertCommand(){}
        void execute()
        {
            // operate on operand
        }
        
    private:
        uint32_t    m_id;
        uint32_t    m_val;
};

template< class OperandType > 
class RangeCommand : public Command< OperandType >
{
    public:
        RangeCommand( sp< OperandType> op, uint32_t from, uint32_t to ) : 
            Command( op ), m_from( from ), m_to( to ){}
        ~RangeCommand(){}
        
        virtual void execute() = 0;
    
    protected:
        uint32_t    m_from;
        uint32_t    m_to;
};

template< class OperandType >
class SumCommand final: public RangeCommand< OperandType >
{
    public:
        SumCommand( sp< OperandType > op, uint32_t from, uint32_t to) 
            : RangeCommand( op, from, to){}
        ~SumCommand(){}
        void execute() 
        {
            // operate on operand
        }
};

template< class OperandType >
class ModifyCommand final: public RangeCommand< OperandType >
{
    public:
        ModifyCommand( sp< OperandType > op, uint32_t from, uint32_t to, int32_t delta) 
            : RangeCommand( op, from, to), m_delta( delta ){}
        ~ModifyCommand(){}
        void execute() 
        {
            // operate on operand
        }
        
    private:
        int32_t     m_delta;
};

template< class OperandType >
class DeleteCommand : public RangeCommand< OperandType >
{
    public:
        DeleteCommand( sp< OperandType > op, uint32_t from, uint32_t to) 
            : RangeCommand( op, from, to){}
        ~DeleteCommand(){}
        void execute() final
        {
            // operate on operand
        }
};

template< class CommandType >
class CommandFactory
{
    public:
        static sp< CommandType > create( std::string cmdInfo )
        {
            // parse the cmdInfo
            // create corresponding command
            return  
        };
    /*    
    static
    {
        s_mapCmdKind2Type.insert( std::make_pair( 'I', std::make_shared< InsertCommand >() ) );        
        s_mapCmdKind2Type.insert( std::make_pair( 'Q', std::make_shared< SumCommand >()    ) );        
        s_mapCmdKind2Type.insert( std::make_pair( 'M', std::make_shared< ModifyCommand >() ) );        
        s_mapCmdKind2Type.insert( std::make_pair( 'D', std::make_shared< DeleteCommand >() ) );        
    }
    
    private:
        std::unordered_map< CmdKind, sp< CommandType> > s_mapCmdKind2Type;
    */
};

int main()
{
    auto op = std::make_shared< SplayTree >();    
            
    
}
