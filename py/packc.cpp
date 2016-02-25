
struct Hello {
    std::string say_hi()
    {
        return "Hello Jklou!";
    }
}

#include <boost/python.hpp>
using namespace boost::python;

BOOST_PYTHON_MODULE(mymodule)
{
    class <Hello>("Hello")
        .def("say_hi", &Hello::say_hi);
}
