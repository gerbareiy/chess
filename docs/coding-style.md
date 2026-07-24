# Coding Style

This covers everything `.clang-format` cannot enforce.

No free functions are allowed. If one is needed, put it into a separate class and make it `static`.

One file (one module) — one class, struct, or enum class. If a class needs another class/struct/enum class alongside it
(e.g. an enum of events it returns, or a value object it uses), move that into its own file/module and import it.
Nested classes, structs, and enum classes (declared directly inside another class's body) are forbidden.

```cpp
module; // omit this if the file has no #includes
#include "AnyOtherIncludeFile.h" // let .clang-format decide the order
export module MyNamespace.MyClass; // the module name is the namespace + class name
import OtherNamespace.OtherClass; // imports are alphabetical

namespace MyNamespace
{
    export class MyClass
    {
        friend class OtherNamespace::OtherClass; // prefer avoiding friends

        static constexpr int m_constexprFieldName = 5;
        static const int m_staticConstFieldNameOne = 5; // constants with inline initialization go first
        static const int m_staticConstFieldNameTwo;
        // there should be no static non-const fields

        constexpr int m_constexprFieldName = 5;
        int m_constFieldNameOne = 5; // constants with inline initialization go first
        int m_constFieldNameTwo;
        int m_fieldName = 5; // non-const fields must be initialized unless done by default

        static int GetMyConstStaticFieldNameOne() // same order as the field declarations
        {
            return m_staticConstFieldNameOne;
        }

        // prefer avoiding .cpp files and implementing directly in the module
        static int MyStaticMethod(int parameterOne, int paremeterTwo)
        {
            // ...
        }

        MyClass()
        {
            // ...
        }

        int GetMyFieldName() const // same order as the field declarations
        {
            return m_fieldName;
        }

        // prefer avoiding .cpp files and implementing directly in the module
        // if a method returns something, it must not change the class's state
        // exception: a method may return bool meaning whether it managed to change the state or not,
        // and its name must start with Try
        int MyConstMethod(int parameterOne, int paremeterTwo) const
        {
            // ...
        }

        // prefer avoiding .cpp files and implementing directly in the module
        void MyMethod(int parameterOne, int paremeterTwo)
        {
            // ...
        }

    protected:
        // all fields live only in the private section; if you need to get/set them,
        // add the corresponding GetMyField/SetMyField methods, and replace every internal
        // interaction with the field by going through those methods too
        // exception: structs — inside them it's best to avoid methods altogether, but not mandatory
    public:
    };
}
```

## Enum

Plain (unscoped) enums are fully forbidden, except when unavoidable (e.g. required by an external library/API). Use only `enum class`.

## Braces

```cpp
    for (const auto& object : objects) // not like this
        // ...

    for (const auto& object : objects) // like this
    {
        // ...
    }
```

## Loops over numeric ranges

All classic loops over a numeric range must be written using `std::views::iota`.

```cpp
// not like this
for (int i = 0; i < count; ++i)
{
    // ...
}

// like this
for (int i : std::views::iota(0, count))
{
    // ...
}
```

## Constructors

```cpp
class MyClass
{
    // ...
    explicit MyClass(int value); // single-argument constructors should be explicit

    MyClass(int value1, int value2)
        : m_value1(value1)
        , m_value2(value2)
    {
        // no computation in the constructor. Two options instead:
        // - do the computation ahead of time, or
        // - add an init method that's called right after the constructor
    }
};
```

### Classes wrapping a resource that needs to be configured (e.g. a socket)

If constructing the object is essentially configuring an external resource that can fail (e.g. connecting/binding a socket),
that counts as "computation" too and doesn't belong in the constructor.
Instead, make the constructor private and have it only store an already-ready resource, and add a public static factory
method that creates and configures the resource and may throw.

```cpp
class ClientSocket
{
    zmq::socket_t m_socket;

    explicit ClientSocket(zmq::socket_t socket)
        : m_socket(std::move(socket))
    {
    }

public:
    static ClientSocket Connect(const std::string& host, unsigned short port)
    {
        zmq::socket_t socket(Context(), zmq::socket_type::dealer);
        try
        {
            socket.set(zmq::sockopt::routing_id, GenerateIdentity());
            socket.connect(Endpoint(host, port));
        }
        catch (const zmq::error_t& error)
        {
            throw ConnectionError(error.what());
        }
        return ClientSocket(std::move(socket));
    }
};
```

## Indentation

All files use **4 spaces** for indentation.
```
something
[
    something
    {
        something
    }
]
```

## Passing smart pointers into lambdas

Capture smart pointers only via `weak_ptr`, otherwise you risk non-obvious memory leaks.
```cpp
const auto shared = std::make_shared<T>();
const auto lambda = [weak = std::weak_ptr(shared)]()
{
    // something
}
```

## Default initialization

We only use copy initialization and copy list initialization for containers.
```cpp
int a;          // default initialization (value is unspecified for local fundamental types)
int b{};        // value initialization
int c = 5;      // copy initialization - this
int d(5);       // direct initialization
int e{5};       // direct list initialization
std::vector f = {5};    // copy list initialization - and this
```

For aggregates (structs) you can use designated initializers from the C++20 standard (`.name = value`). The same rule applies:
fields and local variables are written through `=` (copy list initialization):
```cpp
struct Coordinate
{
    char file;
    int  rank;
};

const Coordinate coordinate = { .file = 'D', .rank = 4 }; // like this, not Coordinate coordinate{ .file = 'D', .rank = 4 };
```

## Working with std::optional

Check for a value only via `has_value()`, and read the value via `value()` (or `value_or()` if you need a default).
Checking an optional through an implicit or explicit bool conversion, or reading the value via `operator*`, is forbidden.
```cpp
std::optional<int> value = TryGetValue();

// not like this
if (value)
{
    Use(*value);
}

// like this
if (value.has_value())
{
    Use(value.value());
}

// or, if you need a default value
Use(value.value_or(0));
```
