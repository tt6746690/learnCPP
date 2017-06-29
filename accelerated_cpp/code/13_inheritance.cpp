#include <string>
#include <vector>
#include <iostream>

class Core
{
  public:
    Core();
    Core(std::istream &);
    std::string name() const;
    std::istream &read(std::istream &);
    double grade() const;

  private:
    std::istream &read_common(std::istream &);
    std::string n;
    double midterm, final;
    std::vector<double> homework;
};

class Grad : public Core
{
  public:
    Grad();
    Grad(std::istream &);
    double grade() const;
    std::istream &read(std::istream &);

  private:
    double thesis;
};
