#include <algorithm>

// Forward declaration of Observer interface
class Observer;

// Subject interface
class Subject{
    public:
        virtual void attach(Observer* observer) = 0;
        virtual void detach(Observer* observer) = 0;
        virtual void notify() = 0;
};

// Observer interface
class Observer{
    public:
        virtual void update(int result) = 0;
};

// Concrete subject class
class Coin : public Subject{
    private:
        std::vector<Observer*> observers;
        int result;
    public:
        Coin(){}
        ~Coin() = default;
        void attach(Observer* observer) override{
            observers.push_back(observer);
        }
        void detach(Observer* observer) override {
            auto it = std::find(observers.begin(), observers.end(), observer);
            if (it != observers.end()){
                observers.erase(it);
            }
        }
        void notify() override{
            for(Observer* observer : observers){
                observer -> update(result);
            }
        }

        void flip(){
            result = rand() % 2; //0 for heads, 1 for tails
            notify(); //notify results about the result
        }

};

class Player : public Observer{
    private:
        std::string name;
    public:
        Player(const std::string& playerName) : name(playerName) {}
        void update(int result) override{
            if(result == 0){
                std::cout << name << ", be informed the result is HEADS!" << "\n";
            }
            else {
                std::cout << name << ", be informed the result is TAILS!" << "\n";
            }
        }
};
