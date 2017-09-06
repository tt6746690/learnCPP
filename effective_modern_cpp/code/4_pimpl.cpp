#include <vector>
#include <utility>
#include <string>
#include "4_pimpl.h"


struct Widget::Impl{
    std::string name;
    std::vector<double> data;
};

Widget::Widget()
    : pImpl(std::make_unique<Impl>()){}

Widget::~Widget(){}


