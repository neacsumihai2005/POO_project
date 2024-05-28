#include <cmath>
#include <unordered_map>
#include <memory>

class Cloth{
    private:
        static std::unordered_map<std::string, int> sizeMap;
    public:
        virtual ~Cloth() = default;
        virtual double getPrice(const std::string& Size) = 0;
        int getSizeValue(const std::string &Size){
            return sizeMap[Size];
        }
};
std::unordered_map<std::string, int> Cloth::sizeMap = {
    {"XS", 1}, {"S", 2}, {"M", 3}, {"L", 4}, {"XL", 5}, {"XXL", 6}
};

// Derived class for Tshirt
class Tshirt : public Cloth{
    public:
        double getPrice(const std::string& Size) override{
            int x = static_cast<Cloth&> (*this).getSizeValue(Size);
            return 10 + 2 * x + 0.5 * x * x;
        }
};
class Shorts : public Cloth{
    public:
        double getPrice(const std::string& Size) override{
            int x = static_cast<Cloth&> (*this).getSizeValue(Size);
            return 8 + 3 * x + 0.3 * x * x + 0.1 * x * x * x;
        }
};
class Shoes : public Cloth{
    public:
        double getPrice(const std::string& Size) override{
            int x = static_cast<Cloth&> (*this).getSizeValue(Size);
            return 15 + 5 * log(x + 1);
        }
};

class ClothFactory{
    public:
        static std::unique_ptr <Cloth> createCloth(const std::string& clothType){
            if(clothType == "tshirt"){
                return std::make_unique<Tshirt>();
            }
            else if(clothType == "shorts"){

                return std::make_unique<Shorts>();
            }
            else if(clothType == "shoes"){

                return std::make_unique<Shoes>();
            }
            else {
                return nullptr;
            }
        }
};
