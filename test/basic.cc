#include "reflection.h"

#define REGISTER(T) typeof(T); // Just call typeof for type registering
template <typename T>
class Array {
BEGIN_TYPE(Array)
    FIELDS(FIELD(&Array::value))
    CTORS(CTOR(Array, T))
END_TYPE
public:
    Array(T value) : value(value) {}
    T value;
};
class PhoneNumber{
BEGIN_TYPE(PhoneNumber)
    FIELDS(FIELD(&PhoneNumber::areaCode),
            FIELD(&PhoneNumber::number))
    CTORS(DEFAULT_CTOR(PhoneNumber),
            CTOR(PhoneNumber, const std::string&, const std::string&))
    METHODS(METHOD(&PhoneNumber::ToString, PhoneNumber, std::string),
            METHOD(&PhoneNumber::ToString, PhoneNumber, std::string, int))
END_TYPE
public:
    std::string areaCode;
    std::string number;

    PhoneNumber() {}
    PhoneNumber(const std::string& areaCode, const std::string& number) : areaCode(areaCode), number(number) {}
    std::string ToString() const{
        return areaCode + " " + number;
    }
    std::string ToString(int a) const{
        return areaCode + " " + number + " " + std::to_string(a);
    }
};

REFLECT_ENUM(Sex, Secret, Male, Female)

class Person{
BEGIN_TYPE(Person)
    FIELDS(FIELD(&Person::name),
            FIELD(&Person::height),
            FIELD(&Person::sex),
            FIELD(&Person::phoneNumber))
    CTORS(DEFAULT_CTOR(Person),
            CTOR(Person, const std::string&, float, Sex))
    METHODS(METHOD(&Person::GetPhoneNumber),
            METHOD(&Person::SetPhoneNumber))
END_TYPE
protected:
    std::string name;
    float height;
    Sex sex;
    PhoneNumber phoneNumber;
public:
    Person(const std::string& name, float height, Sex sex)
        : name(name), height(height), sex(sex) {
    }
    Person() : Person("Unnamed", 0, Sex::Secret) { }
    const std::string& GetName() const{
        return name;
    }
    const PhoneNumber& GetPhoneNumber(){
        return phoneNumber;
    }
    void SetPhoneNumber(const PhoneNumber& phoneNumber){
        this->phoneNumber = phoneNumber;
    }
};

int main() {
    REGISTER(PhoneNumber);
    REGISTER(Sex);
    REGISTER(Person);
    REGISTER(Array<int>);
    try {
        auto newArray = Type::GetType("Array", true)->GetConstructor({
                Type::GetType("int")})->Invoke(10);
        std::cout << newArray.Cast<Array<int>*>()->value << std::endl;
        auto newPhone = Type::GetType("PhoneNumber")->GetConstructor({
                Type::GetType("std::string"),
                Type::GetType("std::string")})->
            Invoke(std::string("+86"), std::string("18510753679"));

        auto newPerson = Type::GetType("Person")->GetConstructor({
                Type::GetType("std::string"),
                Type::GetType("float"),
                Type::GetType("Sex")})->
            Invoke(std::string("zhengankun"), 1.75f,
                    Enum::GetValue(Type::GetType("Sex"), "Male"));
        Type::GetType("Person")->GetMethod("SetPhoneNumber")->
            Invoke(newPerson, newPhone);
        auto phone = Type::GetType("Person")->GetMethod("GetPhoneNumber")->
            Invoke(newPerson);
        //auto str = Type::GetType("PhoneNumber")->GetMethod("ToString")->
        //    Invoke(phone.Cast()); // Cast used here for getting address of the object
        //std::cout << str.Cast<std::string>() << std::endl;
    }
    catch (const Exception& ex){
        std::cout << "error: " << ex.What() << "  at: " << ex.Where() << std::endl;
    }
    return 0;
}
