#include <iostream> 
#include <string>

int main(){
    std::cout << "Please enter your first name: ";

    // read name 
    std::string name;       // define name 
    std::cin >> name;       // read into 
    
    // build message intend to write 
    const std::string greeting = "Hello, " + name  + "!";

    // build second and fourth line of output 
    const std::string spaces(greeting.size(), ' ');
    const std::string second = "* " + spaces + " *";

    // build the first and fifth line of output 
    const std::string first(second.size(), '*');

    // write it all 
    std::cout << std::endl;
    std::cout << first << std::endl;
    std::cout << second << std::endl;
    std::cout << "* " << greeting << " *" << std::endl;
    std::cout << second << std::endl;
    std::cout << first << std::endl;
    return 0;
}

