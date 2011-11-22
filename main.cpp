#include <iostream>
#include <sstream>
#include <map>
#include <tr1/functional>

#include <loki/Typelist.h>
#include <loki/HierarchyGenerators.h>

using namespace std;
using namespace Loki;
using namespace Loki::TL;

struct FieldData
{
    string data;
    bool empty;

    FieldData():
        empty(true)
    {

    }
};

template <typename T>
class Field_Base
{
public:
    typedef T DataType;

    Field_Base(const size_t id,const string& name):
        id_(id),name_(name),empty_(true)
    {
    }

    virtual ~Field_Base()
    {
    }

    void registerFuncionPreDeserialize(tr1::function<bool ()> func)
    {
        preDeserializeFunc_=func;
    }

    void operator ()(const DataType& data)
    {
        data_=data;
    }

    DataType operator ()()
    {
        if (empty_)
        {
            bool willThrow=true;
            if (preDeserializeFunc_)
            {
                cout << "Call lazy deserialization on field [" << this->getId() << "]" << endl;
                willThrow=!preDeserializeFunc_();
            }

            if (willThrow)
            {
                cout << "Will throw on empty field [" << this->getId() << "]" << endl;
            }
        }

        return data_;
    }

    size_t getId() const
    {
        return id_;
    }

    string getName() const
    {
        return name_;
    }

    bool empty() const
    {
        bool empty=empty_;

        if (empty)
        {
            if (preDeserializeFunc_)
            {
                empty=preDeserializeFunc_();
            }
        }

        return empty;
    }

protected:
    size_t id_;
    DataType data_;
    string name_;
    bool empty_;
    tr1::function<bool ()> preDeserializeFunc_;

private:
    Field_Base();

};

class Field_A: public Field_Base<int>
{
public:
    Field_A(const size_t id):
        Field_Base<int>(id,"A")
    {

    }

    FieldData serialize() const
    {
        FieldData fieldData;
        fieldData.empty=empty_;
        if (!empty_)
        {
            ostringstream oss;
            oss << data_;
            fieldData.data=oss.str();
        }

        return fieldData;
    }

    void deserialize(string& buffer)
    {
        cout << "Deserialization of [" << this->getId() << "]" << endl;
        istringstream iss(buffer);
        iss >> data_;

        empty_=false;
    }

    string asString() const
    {
        ostringstream oss;
        oss << data_;
        return oss.str();
    }

private:
    Field_A();
};

class Field_B: public Field_Base<string>
{
public:
    Field_B(const size_t id):
        Field_Base<string>(id,"B")
    {
    }

    FieldData serialize() const
    {
        FieldData fieldData;
        fieldData.empty=empty_;
        if (!empty_)
        {
            ostringstream oss;
            oss << data_;
            fieldData.data=oss.str();
        }

        return fieldData;
    }

    void deserialize(string& buffer)
    {
        cout << "Deserialization of [" << this->getId() << "]" << endl;
        data_=buffer;

        empty_=false;
    }

    string asString() const
    {
        ostringstream oss;
        oss << data_;
        return oss.str();
    }

private:
    Field_B();
};

class Field_C: public Field_Base<double>
{
public:
    Field_C(const size_t id):
        Field_Base<double>(id,"C")
    {

    }

    FieldData serialize() const
    {
        FieldData fieldData;
        fieldData.empty=empty_;
        if (!empty_)
        {
            ostringstream oss;
            oss << data_;
            fieldData.data=oss.str();
        }

        return fieldData;
    }

    void deserialize(string& buffer)
    {
        cout << "Deserialization of [" << this->getId() << "]" << endl;
        istringstream iss(buffer);
        iss >> data_;

        empty_=false;
    }

    string asString() const
    {
        ostringstream oss;
        oss << data_;
        return oss.str();
    }

private:
    Field_C();
};

template <size_t ID>
class Field_A_Concrete
{
public:
    Field_A a;

    Field_A_Concrete():
        a(ID)
    {

    }

    FieldData serializeField() const
    {
        return a.serialize();
    }

    void deserializeField(string& buffer)
    {
        a.deserialize(buffer);
    }

    string asString(const int padding) const
    {
        ostringstream oss;
        for (int i=0;i<padding;++i)
        {
            oss << "\t";
        }
        oss << "[" << this->getId() << "] [" << a.getName() <<"]  " << a.asString() << endl;

        return oss.str();
    }

    size_t getId() const
    {
        return a.getId();
    }

    void registerFuncionPreDeserialize(tr1::function<bool ()> func)
    {
        a.registerFuncionPreDeserialize(func);
    }
};

template <size_t ID>
class Field_B_Concrete
{
public:
    Field_B b;

    Field_B_Concrete():
        b(ID)
    {

    }

    FieldData serializeField() const
    {
        return b.serialize();
    }

    void deserializeField(string& buffer)
    {
        b.deserialize(buffer);
    }

    string asString(const int padding) const
    {
        ostringstream oss;
        for (int i=0;i<padding;++i)
        {
            oss << "\t";
        }
        oss << "[" << this->getId() << "] [" << b.getName() <<"]  " << b.asString() << endl;

        return oss.str();
    }

    size_t getId() const
    {
        return b.getId();
    }

    void registerFuncionPreDeserialize(tr1::function<bool ()> func)
    {
        b.registerFuncionPreDeserialize(func);
    }
};

template <size_t ID>
class Field_C_Concrete
{
public:
    Field_C c;

    Field_C_Concrete():
        c(ID)
    {

    }

    FieldData serializeField() const
    {
        return c.serialize();
    }

    void deserializeField(string& buffer)
    {
        c.deserialize(buffer);
    }

    string asString(const int padding) const
    {
        ostringstream oss;
        for (int i=0;i<padding;++i)
        {
            oss << "\t";
        }
        oss << "[" << this->getId() << "] [" << c.getName() <<"]  " << c.asString() << endl;

        return oss.str();
    }

    size_t getId() const
    {
        return c.getId();
    }

    void registerFuncionPreDeserialize(tr1::function<bool ()> func)
    {
        c.registerFuncionPreDeserialize(func);
    }
};

class Seriliazible
{
public:
    void serialiaze() const
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

class DIALECT
{
public:
    typedef string FieldDefinition;
    string getName() const
    {
        return "DIALECT";
    }
};

class DIALECT_MESSAGECONROL
{
public:
    typedef string FieldDefinition;
    string getName() const
    {
        return "DIALECT_MESSAGECONROL";
    }
};

template <class T>
class IfsfMessage: public T
{
public:
    typedef map<size_t,typename T::FieldDefinition> Fields;
    mutable Fields fields_;

    Fields getFields() const
    {
        return fields_;
    }

    FieldData get() const
    {
        FieldData fieldData;
        fieldData.empty=fields_.empty();

        ostringstream oss;
        typename Fields::const_iterator fields_it=fields_.begin();
        for (;fields_it!=fields_.end();++fields_it)
        {
            oss << fields_it->second;
        }

        fieldData.data=oss.str();

        return fieldData;
    }
};

template <class T, class Base>
class CompositeField: public Base, public T
{
public:
    void serialize() const
    {
        FieldData fieldData=T::serializeField();
        if (! fieldData.empty)
        {
            Base::fields_[T::getId()]=fieldData.data;
        }

        Base::serialize();
    }

    void deserialize()
    {
        deserializeOne();

        Base::deserialize();
    }

    bool deserializeOne()
    {
        cout << "Deserialize field [" << T::getId() << "]" << endl;
        typename Base::Fields::iterator fields_it=Base::fields_.find(T::getId());
        if (fields_it!=Base::fields_.end())
        {
            T::deserializeField(fields_it->second);
            return true;
        }

        return false;
    }

    CompositeField()
    {
        tr1::function<bool ()> func=tr1::bind(&CompositeField<T,Base>::deserializeOne,this);
        T::registerFuncionPreDeserialize(func);
    }

    string asString(const int padding) const
    {
        string s=T::asString(padding);
        s.append(Base::asString(padding));

        return s;
    }
};

template <>
class CompositeField<Seriliazible,EmptyType>: public EmptyType, public Seriliazible
{
public:
    void serialize() const
    {
    }

    void deserialize()
    {
    }

    bool deserializeOne()
    {
        return true;
    }

    string asString(const int padding) const
    {
        return string();
    }
};


template <class Base>
class CompositeField<Seriliazible,Base>: public Base, public Seriliazible
{
public:
    void serialize() const
    {
    }

    void deserialize()
    {
    }

    string asString(const int padding) const
    {
        return string();
    }
};

template <class T>
class CompositeField<T,EmptyType>: public EmptyType, public T
{
public:
    void serialize() const
    {
    }

    void deserialize()
    {
    }

    string asString(const int padding) const
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

template <size_t ID>
class Field_D_Concrete
{
public:
    typedef GenLinearHierarchy<
        LOKI_TYPELIST_4(
            Field_A_Concrete<0>,
            Field_B_Concrete<1>,
            Seriliazible,
            IfsfMessage<DIALECT_MESSAGECONROL>
        ),
        CompositeField>
    Field_D;

    Field_D d;

    Field_D_Concrete()
    {

    }

    FieldData serializeField() const
    {
        d.serialize();

        FieldData fieldData=d.get();
        return fieldData;
    }

    void deserializeField(string& buffer)
    {

    }

    string asString(const int padding) const
    {
        int pad=padding+1;
        ostringstream oss;
        oss << "[" << this->getId() << "] [" << d.getName() <<"]  " << endl;
        oss << d.asString(pad) << endl;

        return oss.str();
    }

    size_t getId() const
    {
        return ID;
    }

    void registerFuncionPreDeserialize(tr1::function<bool ()> func)
    {
        //d.registerFuncionPreDeserialize(func);
    }
};

typedef GenLinearHierarchy<
    LOKI_TYPELIST_6(Field_A_Concrete<0>,
               Field_B_Concrete<1>,
               Field_C_Concrete<2>,
               Field_D_Concrete<16>,
               Seriliazible,
               IfsfMessage<DIALECT>
                    ),
    CompositeField>
Message;

int main()
{
    {
        Message message;
        message.fields_[16]="dadas";
        string tmp=message.d.b();

        cout << tmp << endl;

        return 0;
    }

    Message message;
    message.a(6);
    message.b("dads");
    message.c(67.88);
    message.d.a(16);
    message.d.b("kkk");

    string s=message.asString(0);
    cout << s << endl;

    message.serialize();
    cout << message.get().data << endl;
    //message.deserialize();

    map<size_t,DIALECT::FieldDefinition> fields=message.getFields();
    map<size_t,DIALECT::FieldDefinition>::iterator fields_it=fields.begin();
    for (;fields_it!=fields.end();++fields_it)
    {
        cout << "Id=" << fields_it->first << "  value=(" << fields_it->second << ")" << endl;
    }

    return 0;
}
