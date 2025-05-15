# StatusOptional
Optional with status message for c++

## What is a StatusOptional?

The idea of a StatusOptional class is to have a class that optionally represent a value, 
and when it does not, it comes with a message or some other kind of status.
It serves as a replacement for exceptions. Why?

- Exceptions can propagate across the stack in uncontrolled fashion. 
There is no enforcement mechanism integrated to force the user to deal with them.
- They do not play nice with multithreading.
- It is not always clear when a function can throw an exception.
- You cannot both return a value and a message with an exception 
(I mean you could, but this is very ugly to use a catch block to recover a return value).

StatusOptional aims to be used as return values for functions to fix these problems. How?

- If the return type of a function is StatusOptional, you know it can fail, 
you know the error message type and you have to deal with it.
- StatusOptionals behave as normal return values.
- you can use a warning StatusOptional to still return a value, while attaching an error message.

## How does it work?

The whole library is a single header file. 
Just include it in your project and you are good to go. It requires c++17.

The StatusOptional class has two template parameters. 
The first is the value type, and is mandatory. The second is the error message type,
which is set to std::string by default. A constructor is provided 
to construct a StatusOptional from a value, so values can be returned directly in functions.
To return a warning or an error, you have the static functions of the same name:

```
StatusOptional<T, MsgT> function() {
    
    if (success) {
        return T();    
    } else if (built_result_with_warnings) {
        return StatusOptional<T, MsgT>::warning(T(), MsgT("Warning"))
    }
    return StatusOptional<T, MsgT>::error(MsgT("Error"))
}
```

A specilization is provided for StatusOptional<void, MsgT>. This variant does not hold a value,
has none of the related functions, but still distinguish between warnings and errors.
