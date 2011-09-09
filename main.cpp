
#include <iostream>
#include <sstream>
#include <map>
#include <tr1/functional>

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

    Field_Base(const size_t id,const string& name):
        id_(id),name_(name),empty_(true)
    {
    }

    virtual ~Field_Base() {}

    void registerFuncionPreSerialize(tr1::function<void ()> func)
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
            if (preDeserializeFunc_)
            {
                cout << "Call lazy deserialization on field [" << this->getId() << "]" << endl;
                preDeserializeFunc_();
            }
            else
            {
                cout << "Will throw on empty field [" << this->getId() << "]" << endl;
            }
        }

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
        cout << "Deserialization of [" << this->getId() << "]" << endl;
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

    string getName() const
    {
        return name_;
    }

private:
    size_t id_;
    DataType data_;
    string name_;
    bool empty_;
    tr1::function<void ()> preDeserializeFunc_;

    Field_Base();
};

class Field_A: public Field_Base<int>
{
public:
    Field_A(const size_t id):
        Field_Base<int>(id,"A")
    {

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

private:
    Field_C();
};

template <int ID>
class Field_A_Concrete
{
public:
    Field_A a;

    Field_A_Concrete():
        a(ID)
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

    void registerFuncionPreSerialize(tr1::function<void ()> func)
    {
        a.registerFuncionPreSerialize(func);
    }
};

template <int ID>
class Field_B_Concrete
{
public:
    Field_B b;

    Field_B_Concrete():
        b(ID)
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

    void registerFuncionPreSerialize(tr1::function<void ()> func)
    {
        b.registerFuncionPreSerialize(func);
    }
};

template <int ID>
class Field_C_Concrete
{
public:
    Field_C c;

    Field_C_Concrete():
        c(ID)
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

    void registerFuncionPreSerialize(tr1::function<void ()> func)
    {
        c.registerFuncionPreSerialize(func);
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

class DKV
{
public:
    typedef string FieldDefinition;
    string getName() const
    {
        return "DKV";
    }
};

class DKV_MESSAGECONROL
{
public:
    typedef string FieldDefinition;
    string getName() const
    {
        return "DKV_MESSAGECONROL";
    }
};

template <class T>
class IfsfMessage: public T
{
public:
    typedef map<int,typename T::FieldDefinition> Fields;
    mutable Fields fields_;

    Fields getFields() const
    {
        return fields_;
    }
};

template <class T, class Base>
class CompositeField: public Base, public T
{
public:
    void serialize() const
    {
        Base::fields_[T::getId()]=T::serializeField();

        Base::serialize();
    }

    void deserialize()
    {
        deserializeOne();

        Base::deserialize();
    }

    void deserializeOne()
    {
        cout << "Deserialize field [" << T::getId() << "]" << endl;
        typename Base::Fields::iterator fields_it=Base::fields_.find(T::getId());
        if (fields_it!=Base::fields_.end())
        {
            T::deserializeField(fields_it->second);
        }
    }

    CompositeField()
    {
        tr1::function<void ()> func=tr1::bind(&CompositeField<T,Base>::deserializeOne,this);
        T::registerFuncionPreSerialize(func);
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

    void deserializeOne()
    {
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

template <int ID>
class Field_D_Concrete
{
public:
    typedef GenLinearHierarchy<
        LOKI_TYPELIST_4(
            Field_A_Concrete<0>,
            Field_B_Concrete<1>,
            Seriliazible,
            IfsfMessage<DKV_MESSAGECONROL>
        ),
        CompositeField>
    Field_D;

    Field_D d;

    Field_D_Concrete()
    {

    }

    string serializeField() const
    {
        d.serialize();

        ostringstream oss;
        map<int,typename DKV::FieldDefinition> fields=d.getFields();
        map<int,typename DKV::FieldDefinition>::iterator fields_it=fields.begin();
        for (;fields_it!=fields.end();++fields_it)
        {
            oss << fields_it->second;
        }

        return oss.str();
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

    void registerFuncionPreSerialize(tr1::function<void ()> func)
    {
        //d.registerFuncionPreSerialize(func);
    }
};

typedef GenLinearHierarchy<
    LOKI_TYPELIST_6(Field_A_Concrete<0>,
               Field_B_Concrete<1>,
               Field_C_Concrete<2>,
               Field_D_Concrete<16>,
               Seriliazible,
               IfsfMessage<DKV>
                    ),
    CompositeField>
Message;

int main()
{
    {
        Message message;
        string tmp=message.b();

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
    //message.deserialize();

    map<int,typename DKV::FieldDefinition> fields=message.getFields();
    map<int,typename DKV::FieldDefinition>::iterator fields_it=fields.begin();
    for (;fields_it!=fields.end();++fields_it)
    {
        cout << "Id=" << fields_it->first << "  value=(" << fields_it->second << ")" << endl;
    }

    return 0;
}
