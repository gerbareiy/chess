To make project works you need to execute `build.by` script by executing `python3 build.py` command.

# Code convention:

## Порядок членов класса и стиль наименования

Короче говоря, тут то, что .clang-format отформатировать не может
Также никаких свободный функций быть не должно. Если они нужны, то можно их поместить в отдельный класс и сделать статическими.

```cpp
module; // если в файле #include-ы отсутствуют, то это ключевое слово использовать не надо
#include "AnyOtherIncludeFile.h" // пусть тут решает .clang-format в какой последовательности они будут
export module MyNamespace.MyClass; // имя модуля - это имя namespace-а и класса
import OtherNamespace.OtherClass; // импорты идут по алфавиту

namespace MyNamespace
{
    export class MyClass
    {
        friend class OtherNamespace::OtherClass; // лучше friend-ов избегать

        class MySubclass
        {
        };

        struct MySubstruct
        {
        };

        enum class eMyEnumClass
        {
        };

        static constexpr int m_constexprFieldName = 5;
        static const int m_staticConstFieldNameOne = 5; // константы с inline инициализацией идут раньше
        static const int m_staticConstFieldNameTwo;
        // статических неконстантных полей быть не должно

        constexpr int m_constexprFieldName = 5;
        int m_constFieldNameOne = 5; // константы с inline инициализацией идут раньше
        int m_constFieldNameTwo;
        int m_fieldName = 5; // не константные поля нужно инициализировать, если это не делается по умолчанию

        static int GetMyConstStaticFieldNameOne() // в таком же порядке, что и объявления самих полей
        {
            return m_staticConstFieldNameOne;
        }

        // лучше избегать создание .cpp файлов и реализацию делать прямо в модуле
        static int MyStaticMethod(int parameterOne, int paremeterTwo)
        {
            // ...
        }

        MyClass()
        {
            // ...
        }

        int GetMyFieldName() const // в таком же порядке, что и объявления самих полей
        {
            return m_fieldName;
        }
    
        // лучше избегать создание .cpp файлов и реализацию делать прямо в модуле
        // если метод, что-то возвращает, то он не должен менять состояние класса
        // исключения: может возвращать bool, если он означает, получилось ли изменить состояние класса или нет и название метода должно начинатся на Try
        int MyConstMethod(int parameterOne, int paremeterTwo) const
        {
            // ...
        }
    
        // лучше избегать создание .cpp файлов и реализацию делать прямо в модуле
        void MyMethod(int parameterOne, int paremeterTwo)
        {
            // ...
        }

    protected:
        // все поля находятся только в private части, если так нужно получить/изменить, то допускается делать соотетствующие GetMyField и SetMyField - методы,
        // но тогда и внутри класса все взаимодействия с полем стоит заменить на взаимодействие через методы
        // исключение: структуры, но в них, желательно избегать методы вообще, но не обязательно
    public:
    };
}
```

## Скобки

```cpp
    for (auto const& object : objects) // не так
        // ...

    for (auto const& object : objects) // так
    {
        // ...
    }
```

## Кострукторы

```cpp
class MyClass
{
    // ...
    explicit MyClass(int value); // Конструкторы с одним аргументом лучше писать с ключевым словом explicit

    MyClass(int value1, int value2, int value3)
        : m_value1(value1)
        , m_value2(value2)
    {
         m_value3 = value3 + 4; // если происходят какие-то вычисления перед инициализацией, то лучше это делать в теле конструктора
    }
};
```

## Во всех файлах отступы по **4 пробела**
```
something
[
    something
    {
        something
    }
]
```

## Передача умных указателей в лямбды
Умные указатели захватывать только через weak_ptr, иначе могут быть неочивидные утечки памяти
```cpp
const auto shared = std::make_shared<T>();
const auto lambda = [weak = std::weak_ptr(shared)]()
{
    // something
}
```

# Как играть

Взаимодействие с игрой через консоль

Те фигуры, которые доступны для хода, подсвечиваются синим

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/c4f36a54-2d21-4238-a56f-6db9d9e1ec24)

Те клетки куда может ходить фигура подсвечиваются красным

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/e26c4b1b-56c5-49fb-bd92-f4ef8569d7d3)

Обработка неправильного ввода

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/892a5743-ddeb-4e5a-b19e-e43dba0e701a)

Последний ход подсвечивается:
 - Откуда - коричневый
 - Куда - желтый

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/3adf5307-7ea6-40bb-b026-050bff7de4a0)

Съеденные черные фигуры отображаются на белой стороне, а съеденные белые - на черной

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/e762680e-bc5d-4cfa-9709-0b40f0e354db)

Обработка ситуаций, когда ход вызовет шах

В данном случае белый король не может ходить, так как ему угрожает вражеская королева

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/19a56fbf-a535-45b9-b5fe-5123b0c2b248)

Завершение игры при "шахе и мате" или "ничье"

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/f3f10540-acc3-4c6b-a5a7-5cb4bebc6fb7)
