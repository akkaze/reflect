# reflect
A c++ type reflection system  
### Introduction  
**reflect** is a very simple but powerful c++ reflection system, you may find it useful in you projection,espcially in projects which need codegen or serialization(codegen python code from cplusclpus)  
### Build  
Please use cmake having version at least 3.0  
For build, just run the flowing cmd  
***mkdir build && cd build && cmake .. && make -j8***  
### Tutorial  
A very simple c++ example is presented.  
```cpp
#include "reflection.h"

#define REGISTER(T) typeof(T); // Just call typeof for type registering

class PhoneNumber{
BEGIN_TYPE(PhoneNumber)
    FIELDS(FIELD(&PhoneNumber::areaCode), FIELD(&PhoneNumber::number))
    CTORS(DEFAULT_CTOR(PhoneNumber), CTOR(PhoneNumber, const std::string&, const std::string&))
    METHODS(METHOD(&PhoneNumber::ToString))
END_TYPE
public:
    std::string areaCode;
    std::string number;

    PhoneNumber() {}
    PhoneNumber(const std::string& areaCode, const std::string& number) : areaCode(areaCode), number(number) {}
    std::string ToString() const{
        return areaCode + " " + number;
    }
};

REFLECT_ENUM(Sex, Secret, Male, Female)

class Person{
BEGIN_TYPE(Person)
    FIELDS(FIELD(&Person::name), FIELD(&Person::height), FIELD(&Person::sex), FIELD(&Person::phoneNumber))
    CTORS(DEFAULT_CTOR(Person), CTOR(Person, const std::string&, float, Sex))
    METHODS(METHOD(&Person::GetPhoneNumber), METHOD(&Person::SetPhoneNumber))
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
    try {
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
        auto str = Type::GetType("PhoneNumber")->GetMethod("ToString")->
            Invoke(phone.Cast()); // Cast used here for getting address of the object
        std::cout << str.Cast<string>() << std::endl;
    }
    catch (const Exception& ex){
        std::cout << "error: " << ex.What() << "  at: " << ex.Where() << std::endl;
    }
    return 0;
}
```


