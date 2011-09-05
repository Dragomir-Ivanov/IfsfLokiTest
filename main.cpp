
#include <iostream>
#include <sstream>
#include <loki/Typelist.h>
#include <loki/HierarchyGenerators.h>

using namespace std;
using namespace Loki;
using namespace Loki::TL;

template <typename T>
class Field_Base
{
public:
    typedef T DataType;

    Field_Base(const size_t id):
        id_(id)
    {
    }

    virtual ~Field_Base() {}

    void operator ()(const DataType& data)
    {
        data_=data;
    }

    DataType operator ()()
    {
        return data_;
    }

    string serialize() const
    {
        ostringstream oss;
        oss << data_;
        return oss.str();
    }

    void deserialize(string& buffer)
    {

    }

    size_t getId()
    {
        return id_;
    }

private:
    size_t id_;
    DataType data_;

    Field_Base();
};

class Field_A: public Field_Base<int>
{
public:
    Field_A(const size_t id):
        Field_Base<int>(id)
    {

    }

private:
    Field_A();
};

class Field_B: public Field_Base<string>
{
public:
    Field_B(const size_t id):
        Field_Base<string>(id)
    {

    }

private:
    Field_B();
};

class Field_C: public Field_Base<double>
{
public:
    Field_C(const size_t id):
        Field_Base<double>(id)
    {

    }

private:
    Field_C();
};

class Field_A_Concrete
{
public:
    //static const int Id=0;
    Field_A a;

    Field_A_Concrete():
        a(0)
    {

    }

    string serialize() const
    {
        return a.serialize();
    }

    void deserialize(string& buffer)
    {
        a.deserialize(buffer);
    }
};

class Field_B_Concrete
{
public:
    //static const int Id=1;
    Field_B b;

    Field_B_Concrete():
        b(1)
    {

    }

    string serialize() const
    {
        return b.serialize();
    }

    void deserialize(string& buffer)
    {
        b.deserialize(buffer);
    }
};

class Field_C_Concrete
{
public:
    Field_C c;

    Field_C_Concrete():
        c(2)
    {

    }

    string serialize() const
    {
        return c.serialize();
    }

    void deserialize(string& buffer)
    {
        c.deserialize(buffer);
    }
};

class Seriliazible
{
public:
    void serialiaze()
    {

    }

    void deserialize()
    {

    }
};

template <class T, class Base>
class CompositeField: public Base, public T
{
public:
    void serialize()
    {
        Base::serialize();
        cout << T::serialize() << endl;
    }

    void deserialize()
    {
        Base::deserialize();
        string buffer;
        T::deserialize(buffer);
    }
};


template <class T>
class CompositeField<T,EmptyType>: public EmptyType, public T
{
public:
};


typedef GenLinearHierarchy<
    LOKI_TYPELIST_4(Field_A_Concrete,
               Field_B_Concrete,
               Field_C_Concrete,
               Seriliazible),
    CompositeField>
Message;

int main()
{
    Message message;
    message.a(6);
    message.b("dads");
    message.c(67.88);

    message.serialize();

    return 0;
}
