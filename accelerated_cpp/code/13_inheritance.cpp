#include <string>
#include <vector>
#include <iostream>

class Core
{
  public:
    Core() : midterm(0), final(0){};
    Core(std::istream &is) { read(is); };
    std::string name() const;

    // virtual: allows dynamic-binding of read() and grade() at runtime
    virtual std::istream &read(std::istream &);
    virtual double grade() const;

  protected:
    // accessible to derived classes
    std::istream &read_common(std::istream &);
    double midterm, final;
    std::vector<double> homework;

  private:
    // accessible only in Core
    std::string n;
};

class Grad : public Core
{
  public:
    Grad() : thesis(0){};
    Grad(std::istream &is) { read(is); };

    // virtual by inheritance
    double grade() const;
    std::istream &read(std::istream &);

  private:
    double thesis;
};

std::string Core::name() const { return n; }

double Core::grade() const {
    return ::grade(midterm, final, homework)}

std::istream &Core::read_common(std::istream &in)
{
    // read and store students name and grades
    in >> n >> midterm >> final;
}

std::istream &Core::read(std::istream &in)
{
    read_common(in);
    read_hw(in, homework);
    return in;
}

std::istream &Grad::read(std::istream &in)
{
    read_common(in);
    in >> thesis;
    read_hw(in, homework);
    return in;
}

double Grad::grade() const
{
    return std::min(Core::grade(), thesis);
}

bool compare(const Core &c1, const Core &c2)
{
    return c1.name() < c2.name();
}

class Student_info
{
  public:
    // cp = 0 means object has yet to read data in
    Student_info() : cp(0) {}
    Student_info(std::istream &is) : cp(0) { read(is); }
    Student_info(const Student_info &);
    Student_info &operator=(const Student_info &);
    ~Student_info() { delete cp; }

    // operation
    std::istream &read(std::istream &is)
    {
        // free previous object
        delete cp;

        char ch;
        is >> ch;

        // allocate right kind of object
        if (ch == 'U')
        {
            cp = new Core(is);
        }
        else
        {
            cp = new Grad(is);
        }
        return is;
    }

    std::string name() const
    {
        if (cp)
            return cp->name();
        else
            throw std::runtime_error("uninitialized Student");
    }

    double grade() const
    {
        if (cp)
            return cp->grade();
        else
            throw std::runtime_error("uninitialized Student");
    }

    static bool compare(const Student_info &s1, const Student_info &s2)
    {
        return s1.name() < s2.name();
    }

  private:
    Core *cp;
};

int main()
{
    std::vector<Core> students;
    Core record;

    std::string::size_type maxlen = 0;

    while (record.read(std::cin))
    {
        maxlen = std::max(maxlen, record.name().size());
        students.push_back(record);
    }

    // sort by name
    sort(students.begin(), students.end(), compare);

    //write name and grades
    for (std::vector<Core>::size_type i = 0; i != students.size(); ++i)
    {
        std::cout << students[i].name()
                  << std::string(maxlen + 1 - students[i].name.size(), ' ');
        try
        {
            double final_grade = students[i].grade(); // Core::grade()
            std::streamsize prec = std::cout.precision();
            std::cout << setprecision(3) << final_grade
                      << set_precision(prec) << std::endl;
        }
    }
}