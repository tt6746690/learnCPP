
#include <iostream> 
#include <string>

int main(){
    std::cout << "Please enter your first name: ";

    // read name 
    std::string name;       // define name 
    std::cin >> name;       // read into 

    // build message intend to write 
    const std::string greeting = "Hello, " + name  + "!";

    const int pad = 1;
    const int rows = pad * 2 + 3;
    const std::string::size_type cols = greeting.size() + pad * 2 + 2;

    // separaitng output from input 
    std::cout << std::endl;

    // write row rows of output 
    for(int r = 0; r != rows; ++r){
        std::string::size_type c = 0;
        while (c != cols){
            if( r == 0 || r == rows - 1 || c == 0 || c == cols -1 ){
                std::cout << "*";
                ++c;
            } else {
                if(r == pad + 1 && c == pad + 1){
                    std::cout << greeting;
                    c += greeting.size();
                } else {
                    std::cout << " ";
                    ++c;
                }
            }

        }
        std::cout << std::endl;
    }

    return 0;
}

