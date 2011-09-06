
#include <iostream>
#include <sstream>
#include <map>

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

    string asString() const
    {
        ostringstream oss;
        oss << data_;
        return oss.str();
    }

    size_t getId() const
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

    string serializeField() const
    {
        return a.serialize();
    }

    void deserializeField(string& buffer)
    {
        a.deserialize(buffer);
    }

    string asString() const
    {
        return a.asString();
    }

    size_t getId() const
    {
        return a.getId();
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

    string serializeField() const
    {
        return b.serialize();
    }

    void deserializeField(string& buffer)
    {
        b.deserialize(buffer);
    }

    string asString() const
    {
        return b.asString();
    }

    size_t getId() const
    {
        return b.getId();
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

    string serializeField() const
    {
        return c.serialize();
    }

    void deserializeField(string& buffer)
    {
        c.deserialize(buffer);
    }

    string asString() const
    {
        return c.asString();
    }

    size_t getId() const
    {
        return c.getId();
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

    string asString() const
    {
        return string();
    }
};

class DKV
{
public:
    typedef string FieldDefinition;
};

template <class T>
class IfsfMessage
{
public:
    typedef map<int,typename T::FieldDefinition> Fields;
    Fields fields_;

    Fields getFields() const
    {
        return fields_;
    }
};

template <class T, class Base>
class CompositeField: public Base, public T
{
public:
    void serialize()
    {
        cout << T::serializeField() << endl;
        Base::fields_[T::getId()]=T::serializeField();

        Base::serialize();
    }

    void deserialize()
    {
        string buffer;
        T::deserializeField(buffer);

        Base::deserialize();
    }

    string asString() const
    {
        string s=T::asString();
        s.append(Base::asString());

        return s;
    }
};

template <>
class CompositeField<Seriliazible,EmptyType>: public EmptyType, public Seriliazible
{
public:
    void serialize()
    {
    }

    void deserialize()
    {
    }

    string asString() const
    {
        return string();
    }
};


template <class Base>
class CompositeField<Seriliazible,Base>: public Base, public Seriliazible
{
public:
    void serialize()
    {
    }

    void deserialize()
    {
    }

    string asString() const
    {
        return string();
    }
};

template <class T>
class CompositeField<T,EmptyType>: public EmptyType, public T
{
public:
    void serialize()
    {
    }

    void deserialize()
    {
    }

    string asString() const
    {
        return string();
    }
};

template <typename T,class Base>
class CompositeField<IfsfMessage<T>,Base>: public Base, public IfsfMessage<T>
{
public:
};

template <typename T>
class CompositeField<IfsfMessage<T>,EmptyType>: public EmptyType, public IfsfMessage<T>
{
public:
};

typedef GenLinearHierarchy<
    LOKI_TYPELIST_5(Field_A_Concrete,
               Field_B_Concrete,
               Field_C_Concrete,
               Seriliazible,
               IfsfMessage<DKV>
                    ),
    CompositeField>
Message;

int main()
{
    Message message;
    message.a(6);
    message.b("dads");
    message.c(67.88);

    message.serialize();
    message.deserialize();

    string s=message.asString();
    cout << s << endl;

    map<int,typename DKV::FieldDefinition> fields=message.getFields();
    map<int,typename DKV::FieldDefinition>::iterator fields_it=fields.begin();
    for (;fields_it!=fields.end();++fields_it)
    {
        cout << "Id=" << fields_it->first << "  value=(" << fields_it->second << ")" << endl;
    }

    return 0;
}
